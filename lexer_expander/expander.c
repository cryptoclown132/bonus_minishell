/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 15:01:04 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/21 11:58:14 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expander(char *token, env_var environ)
{
	t_expan	e;

	e.i = -1;
	e.tmp_var = NULL;
	while (token[++e.i])
	{
		if (token[e.i] == '$' && token[e.i + 1] != ' '
			&& token[e.i + 1] != '\0')
		{
			e.tmp_var = get_var(token, environ);
			free(token);
			break ;
		}
	}
	if (e.tmp_var == NULL)
		e.tmp_var = token;
	return (e.tmp_var);
}
