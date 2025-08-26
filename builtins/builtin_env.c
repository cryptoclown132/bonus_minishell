/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 00:43:18 by fjerinic          #+#    #+#             */
/*   Updated: 2025/08/26 18:05:08 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env(t_env_var *environ)
{
	int		i;

	i = 0;
	while (environ->env[i])
	{
		if (!ft_strncmp(environ->env[i], "_=", 2))
		{
			free(environ->env[i]);
			environ->env[i] = ft_strdup("_=usr/bin/env");
		}
		if (ft_strchr(environ->env[i], '='))
			ft_printf("%s\n", environ->env[i]);
		i++;
	}
}
