/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_err_and_or.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:26:08 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/24 13:26:12 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_or_2(char *input, int i)
{
	if (input[i] == '&')
	{
		if (input[i + 1] == '&')
			lex_error("&&");
		else
			lex_error("&");
	}
	else if (input[i] == ')')
		lex_error(")");
	else
	{
		if ((input[i - 1] == '|' && input[i])
			|| (input[i] && input[i + 1] == '|'))
			lex_error("||");
		else
			lex_error("|");
	}
	return (-1);
}

int	check_or(char *input, int i)
{
	if (input[++i] == '|')
		i++;
	while (input[i] == ' ')
		i++;
	if (input[i] == '|' || input[i] == '&' || input[i] == ')')
		return (check_or_2(input, i));
	return (i);
}

int	check_and_2(char *input, int i)
{
	if (input[i] == '&')
	{
		if (input[i + 1] == '&')
			lex_error("&&");
		else
			lex_error("&");
	}
	else if (input[i] == ')')
		lex_error(")");
	else
	{
		if (input[i + 1] == '|')
			lex_error("||");
		else
			lex_error("|");
	}
	return (-1);
}

int	check_and(char *input, int i)
{
	if (input[++i] == '&')
		i++;
	else if (input[i] != '|')
	{
		lex_error("&");
		return (-1);
	}
	while (input[i] == ' ')
		i++;
	if (input[i] == '|' || input[i] == '&' || input[i] == ')')
		return (check_and_2(input, i));
	return (i);
}
