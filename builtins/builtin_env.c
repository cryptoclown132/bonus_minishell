/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 00:43:18 by fjerinic          #+#    #+#             */
/*   Updated: 2025/04/25 17:01:43 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env(cmd_tree *cmd_struct)
{
	int		i;

	i = 0;
	while (cmd_struct->env[i])
	{
		if (!ft_strncmp(cmd_struct->env[i], "_=", 2))
		{
			free(cmd_struct->env[i]);
			cmd_struct->env[i] = ft_strdup("_=usr/bin/env");
		}
		if (ft_strchr(cmd_struct->env[i], '='))
			ft_printf("%s\n", cmd_struct->env[i]);
		i++;
	}
}
