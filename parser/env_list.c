/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 14:26:07 by jkroger           #+#    #+#             */
/*   Updated: 2025/07/25 17:46:02 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_env_len(char **envp)
{
	int	i;

	if (!envp)
		return (0);
	i = 0;
	while (envp[i])
		i++;
	return (i);
}

char	**copy_env(char **envp)
{
	int		i;
	char	**env;

	env = malloc((count_env_len(envp) + 1) * sizeof(char *));
	if (!env)
		return (set_exit_status("Failed to Malloc", 1));
	
	i = -1;
	while (envp[++i])
		env[i] = ft_strdup(envp[i]);
	env[i] = NULL;
	return (env);
}
