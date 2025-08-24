/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_err_red.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 18:59:13 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/24 12:48:39 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_redir_in_err(char *input, int i)
{
	if (input[i] == '\0' || input[i] == '>')
		lex_error("newline");
	else if (input[i] == '<' && input[i + 1] != '<')
		lex_error("<");
	else if (input[i] == '<' && input[i + 1] == '<')
		lex_error("<<");
	else if (input[i] == '|')
		lex_error("|");
	else if (input[i] == '(')
		lex_error("(");
	else if (input[i] == ')')
		lex_error(")");
	else if (input[i] == '&')
		lex_error("&");
	return (0);
}

int	check_redir_in(char *input, int i)
{
	while (input[i] == ' ')
		i++;
	if (input[i - 1] == ' ' && input[i] == '<')
	{
		lex_error("<");
		return (0);
	}
	i = skip_rs_in(input, i);
	if (input[i] == '>' || input[i] == '\0'
		|| input[i] == '<' || input[i] == '|'
		|| input[i] == '(' || input[i] == ')'
		|| input[i] == '&')
		return (check_redir_in_err(input, i));
	return (i);
}

int	check_redir_out_err(char *input, int i)
{
	if (input[i] == '\0')
		lex_error("newline");
	else if (input[i] == '>' && input[i + 1] != '>')
		lex_error(">");
	else if (input[i] == '<')
		lex_error("<");
	else if (input[i] == '>' && input[i + 1] == '>')
		lex_error(">>");
	else if (input[i] == '|')
		lex_error("|");
	else if (input[i] == '(')
		lex_error("(");
	else if (input[i] == ')')
		lex_error(")");
	else if (input[i] == '&')
		lex_error("&");
	return (0);
}

int	check_redir_out(char *input, int i)
{
	while (input[i] == ' ')
		i++;
	if (input[i - 1] == ' ' && input[i] == '>')
	{
		lex_error(">");
		return (0);
	}
	i = skip_rs_out(input, i);
	if (input[i] == '<' || input[i] == '\0'
		|| input[i] == '>' || input[i] == '|'
		|| input[i] == '(' || input[i] == ')'
		|| input[i] == '&')
		return (check_redir_out_err(input, i));
	return (i);
}

int	check_redir(char *input)
{
	int	i;

	i = -1;
	while (input[++i])
	{
		if (input[i] == '\'' || input[i] == '"')
			quote_len(input, &i);
		if (input[i] == '<')
		{
			i = check_redir_in(input, ++i);
			if (i == 0)
				return (0);
		}
		else if (input[i] == '>')
		{
			i = check_redir_out(input, ++i);
			if (i == 0)
				return (0);
		}
	}
	return (1);
}
