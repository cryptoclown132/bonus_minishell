/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 17:56:19 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/15 14:55:05 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_var(char **vars, char *var)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return (set_exit_status("Failed to Malloc", 1));
	while (vars && vars[i])
	{
		if (ft_strnstr(vars[i], tmp, ft_strlen(tmp)))
		{
			free(tmp);
			return (vars[i]);
		}
		i++;
	}
	free(tmp);
	return (NULL);
}

void	add_env_loop(env_var *environ, char **envcp, char *var)
{
	int	i;

	i = 0;

	// int k = -1;
	// while(envcp && envcp[++k] && k < 6)
	// 	printf("cpenv = %s\n", envcp[k]);
	while (envcp && envcp[i])
	{
		if (!ft_strncmp(envcp[i], var, len_equal(var)))
		{
			// printf("varrrrr envcp= %s && var = %s\n", envcp[i], var);
			environ->env[i] = ft_strdup(var);
		}
		else
			environ->env[i] = ft_strdup(envcp[i]);
		i++;
	}
	// int j = -1;
	// while(environ->env && environ->env[++j] && j < 6)
	// 	printf("env = %s\n", environ->env[j]);
	if (!ft_var(envcp, var))
		environ->env[i++] = ft_strdup(var);
	environ->env[i] = NULL;
}

void	add_env(env_var *environ, char *var)
{
	int		i;
	char	**envcp;

	if (!var)
		return ;
	envcp = environ->env;
	
	// if (find_var_in_env(environ->vars, var))
	// 	del_var(environ, var);

	if (ft_var(envcp, var))
		environ->env = malloc((count_env_len(envcp) + 1) * sizeof(char *));
	else
		environ->env = malloc((count_env_len(envcp) + 2) * sizeof(char *));
	if (!environ->env)
		return (set_exit_void("Failed to Malloc", 1));

	add_env_loop(environ, envcp, var);
	if (envcp)
	{
		i = -1;
		while (envcp[++i])
			free(envcp[i]);
		free(envcp);
	}
}

char	**sort_export(char **expo)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (expo[++i])
	{
		j = -1;
		while (expo[++j])
		{
			if (ft_strncmp(expo[j], expo[j + 1], ft_strlen(expo[j])) > 0)
			{
				tmp = expo[j];
				expo[j] = expo[j + 1];
				expo[j + 1] = tmp;
			}
		}
	}
	return (expo);
}

void	builtin_export(cmd_tree *cmd, env_var *environ)
{
	int	i;
	int	j;

	if (!cmd->exec.cmd_split[1])
		export_without_args(environ);	
	i = 0;
	while (cmd->exec.cmd_split[++i])
	{
		j = export_err(cmd, cmd->exec.cmd_split[i], i, environ);
		if (j == 0 && i == 1)
			return ;
		if (j == 1)
			continue ;
		if (ft_strchr(cmd->exec.cmd_split[i], '='))
			add_env(environ, cmd->exec.cmd_split[i]);
		else
		{
			if (find_var(environ->vars, cmd->exec.cmd_split[i]))
			{
				add_env(environ, find_var(environ->vars, cmd->exec.cmd_split[i]));
				del_var(environ, cmd->exec.cmd_split[i]);
			}
		}
	}
}
