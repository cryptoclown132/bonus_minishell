/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 19:52:10 by fjerinic          #+#    #+#             */
/*   Updated: 2025/08/26 19:35:21 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env(t_env_var *environ, char *str)
{
	char	*env_return;
	int		i;

	i = 0;
	env_return = NULL;
	while (str && environ->env[i] && !ft_strnstr(
			environ->env[i], str, ft_strlen(str)))
		i++;
	if (environ->env[i] && ft_strnstr(environ->env[i], str,
			ft_strlen(str)))
		env_return = ft_substr(environ->env[i], 5,
				ft_strlen(environ->env[i]) - 5);
	else
	{
		set_exit_status("cd: Error", 1);
		env_return = NULL;
	}
	return (env_return);
}

int	find_pwd_index(t_env_var *environ)
{
	int	i;

	i = 0;
	while (environ->env[i]
		&& !ft_strnstr(environ->env[i], "PWD=", 4))
	{
		i++;
		if (environ->env[i] && ft_strnstr(environ->env[i],
				"OLDPWD=", 7))
			i++;
	}
	return (i);
}

static void	update_env(t_env_var *environ, char *new_pwd_path,
				char *old_pwd_path)
{
	int	i;

	i = find_pwd_index(environ);
	if (environ->env[i] && ft_strnstr(environ->env[i],
			"PWD=", 4))
	{
		free(environ->env[i]);
		environ->env[i] = ft_strjoin("PWD=", new_pwd_path);
	}
	else
		old_pwd_path = NULL;
	i = 0;
	while (environ->env[i] && !ft_strnstr(environ->env[i],
			"OLDPWD", 6))
		i++;
	if (environ->env[i] && ft_strnstr(environ->env[i],
			"OLDPWD", 6))
	{
		free(environ->env[i]);
		environ->env[i] = ft_strjoin_zero("OLDPWD=",
				old_pwd_path);
	}
}

int	run_cd_home(t_cmd_tree *cmd_lst, char *old_path, t_env_var *environ)
{
	char	*env_return;

	env_return = get_env(environ, "HOME=");
	if (!env_return)
		return (1);
	if (!cmd_lst->s_exec.cmd_split[1])
	{
		if (env_return && chdir(env_return))
		{
			set_exit_status("Error while running <cd $HOME>", 1);
			free(old_path);
			free(env_return);
			return (1);
		}
		(void)old_path;
		free(old_path);
		free(env_return);
		return (1);
	}
	free(env_return);
	return (0);
}

void	cd(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	char	*old_path;
	char	*new_path;

	old_path = getcwd(NULL, 0);
	if (run_cd_home(cmd_lst, old_path, environ))
		return ;
	if (chdir(cmd_lst->s_exec.cmd_split[1]))
	{
		if (cmd_lst->s_exec.cmd_split[1][0] == '-')
			set_exit_status("cd: invalid option", 1);
		else
			set_exit_status("cd: No such file or directory", 1);
		free(old_path);
		return ;
	}
	new_path = getcwd(NULL, 0);
	update_env(environ, new_path, old_path);
	free(old_path);
	free(new_path);
}
