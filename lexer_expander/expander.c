/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 15:01:04 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/22 14:50:06 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_var(char *token, env_var environ)
{
	char		*var_value;
	t_get_var	v;

	var_value = malloc((get_len(token, environ) + 1) * sizeof(char));
	if (!var_value)
		return (set_exit_status("Failed to Malloc", 1));
	v.i = 0;
	v.j = 0;
	while (token[v.i])
	{
		if (token[v.i] == '$' && token[v.i + 1] != '$')
			cat_var(token, environ, &var_value, &v);
		else
			var_value[v.j++] = token[v.i++];
	}
	var_value[v.j] = '\0';
	return (var_value);
}

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
