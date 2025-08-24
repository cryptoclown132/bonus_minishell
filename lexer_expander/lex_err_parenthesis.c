/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_err_parenthesis.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:23:33 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/24 13:24:14 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_unclosed_parenthesis_err(char *parenthesis)
{
	lex_error(parenthesis);
	return (0);
}

int	skip_to_parenthesis(char *input, int i)
{
	if (input[i] == '\'' || input[i] == '"')
		quote_len(input, &i);
	i++;
	return (i);
}

int	skip_spaces(char *input, int i)
{
	while (input[i] && input[i] == ' ')
		i++;
	return (i);
}

int	check_unclosed_parenthesis(char *input)
{
	int	i;
	int	j;

	i = skip_spaces(input, 0);
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
			quote_len(input, &i);
		if (input[i] == '(')
		{
			j = i + 1;
			while (input[i] && input[i] != ')')
				i = skip_to_parenthesis(input, i);
			j = skip_spaces(input, j);
			if (!input[i])
				return (check_unclosed_parenthesis_err("("));
			else if (input[i - 1] == '(' || input[j] == ')')
				return (check_unclosed_parenthesis_err(")"));
		}
		else if (input[i] == ')')
			return (check_unclosed_parenthesis_err(")"));
		i++;
	}
	return (1);
}

int	check_correct_parenthesis(char *input)
{
	int	i;

	i = -1;
	while (input[++i])
	{
		if (input[i] != ' ' && input[i] != '(')
		{
			if (input[i] == '\"' || input[i] == '\'')
				quote_len(input, &i);
			while (input[i] && input[i] != '|' && input[i] != '&'
				&& input[i] != '<' && input[i] != '>'
				&& input[i] != '(' && input[i] != ')')
				i++;
			if (input[i] == '(')
			{
				lex_error("(");
				return (0);
			}
		}
	}
	return (1);
}
