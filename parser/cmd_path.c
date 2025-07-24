/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:16:41 by jkroger           #+#    #+#             */
/*   Updated: 2025/04/29 18:32:21 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*path_finder(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
			return (ft_substr(env[i], 5, ft_strlen(env[i]) - 5));
	}
	return (NULL);
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

char	*get_cmd_path(char **env, char *cmd, cmd_tree *cmd_node)
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
	envp = path_finder(env);
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
