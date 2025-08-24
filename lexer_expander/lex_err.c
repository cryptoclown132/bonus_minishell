/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_err.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:45:26 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/24 13:26:25 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_spaces(char *input)
{
	int	i;

	i = -1;
	while (input[++i] != '\0')
	{
		if (input[i] != ' ')
			return (1);
	}
	return (0);
}

int	check_unclosed_quotes(char *input)
{
	int	i;
	int	j;

	i = -1;
	while (input[++i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			j = i + 1;
			while (input[j] && input[j] != input[i])
				j++;
			if (!input[j])
			{
				lex_error("unclosed quotes");
				return (0);
			}
			i = j;
		}
	}
	return (1);
}

int	check_log_op_2(char *input, int i)
{
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
			quote_len(input, &i);
		if (input[i] == '|')
			i = check_or(input, i);
		else if (input[i] == '&')
			i = check_and(input, i);
		if (i == -1)
			return (0);
		i++;
	}
	return (1);
}

int	check_log_op(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ')
		i++;
	if (input[i] == '|')
	{
		if (input[i + 1] == '|')
			lex_error("||");
		else
			lex_error("|");
		return (0);
	}
	else if (input[i] == '&')
	{
		if (input[i + 1] == '&')
			lex_error("&&");
		else
			lex_error("&");
		return (0);
	}
	if (check_log_op_2(input, i) == 0)
		return (0);
	return (1);
}

int	lex_error_check(char *input)
{
	if (!check_spaces(input))
		return (0);
	else if (!check_unclosed_quotes(input))
		return (0);
	else if (!check_unclosed_parenthesis(input))
		return (0);
	else if (!check_correct_parenthesis(input))
		return (0);
	else if (!check_redir(input))
		return (0);
	else if (!check_log_op(input))
		return (0);
	return (1);
}
