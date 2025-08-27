/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:16:41 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/27 16:46:33 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*path_finder_2(char **env)
{
	int	i;

	i = -1;
	while (env && env[++i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
			return (ft_substr(env[i], 5, ft_strlen(env[i]) - 5));
	}
	return (NULL);
}

char	*path_finder(t_env_var environ)
{
	char	*envp;

	envp = path_finder_2(environ.env);
	if (!envp)
		envp = path_finder_2(environ.vars);
	return (envp);
}

char	*get_cmd_path2(char *envp, char *cmd)
{
	int		i;
	char	*cmd_path;
	char	**path_hold;

	path_hold = ft_split(envp, ':');
	free(envp);
	i = -1;
	while (path_hold[++i])
	{
		cmd_path = edited_strjoin(ft_strjoin(path_hold[i], "/"), cmd);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_string_array(path_hold);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free_string_array(path_hold);
	return (NULL);
}

char	*get_cmd_path(t_env_var environ, char *cmd, t_cmd_tree *cmd_node)
{
	char	*envp;

	if (!cmd || !is_builtin(cmd))
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		else if (cmd_node->err == 0)
			cmd_node->err = 2;
	}
	envp = path_finder(environ);
	if (!envp)
	{
		if (is_builtin(cmd) != 0 && cmd_node->err == 0)
			cmd_node->err = 2;
	}
	if (!cmd_node->err_file && cmd_node->err == 2)
	{
		cmd_node->err_file = ft_strdup(cmd);
		return (NULL);
	}
	return (get_cmd_path2(envp, cmd));
}
