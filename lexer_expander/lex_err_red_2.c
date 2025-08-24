/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_err_red_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:48:24 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/24 12:48:32 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	skip_rs_in(char *input, int i)
{
	if (input[i] == '<')
		i++;
	while (input[i] == ' ')
		i++;
	return (i);
}

int	skip_rs_out(char *input, int i)
{
	if (input[i] == '>')
		i++;
	while (input[i] == ' ')
		i++;
	return (i);
}
