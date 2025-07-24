/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 19:52:10 by fjerinic          #+#    #+#             */
/*   Updated: 2025/04/25 18:06:10 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env(cmd_tree *cmd_lst, char *str)
{
	char	*env_return;
	int		i;

	i = 0;
	env_return = NULL;
	while (str && cmd_lst->env[i] && !ft_strnstr(
			cmd_lst->env[i], str, ft_strlen(str)))
		i++;
	if (cmd_lst->env[i] && ft_strnstr(cmd_lst->env[i], str,
			ft_strlen(str)))
		env_return = ft_substr(cmd_lst->env[i], 5,
				ft_strlen(cmd_lst->env[i]) - 5);
	else
	{
		set_exit_status("cd: Error", 1);
		env_return = NULL;
	}
	return (env_return);
}

int	find_pwd_index(cmd_tree *cmd_lst)
{
	int	i;

	i = 0;
	while (cmd_lst->env[i]
		&& !ft_strnstr(cmd_lst->env[i], "PWD=", 4))
	{
		i++;
		if (cmd_lst->env[i] && ft_strnstr(cmd_lst->env[i],
				"OLDPWD=", 7))
			i++;
	}
	return (i);
}

static void	update_env(cmd_tree *cmd_lst, char *new_pwd_path,
				char *old_pwd_path)
{
	int	i;

	i = find_pwd_index(cmd_lst);
	if (cmd_lst->env[i] && ft_strnstr(cmd_lst->env[i],
			"PWD=", 4))
	{
		// free(cmd_lst->env[i]);
		cmd_lst->env[i] = ft_strjoin("PWD=", new_pwd_path);
	}
	else
		old_pwd_path = NULL;
	i = 0;
	while (cmd_lst->env[i] && !ft_strnstr(cmd_lst->env[i],
			"OLDPWD", 6))
		i++;
	if (cmd_lst->env[i] && ft_strnstr(cmd_lst->env[i],
			"OLDPWD", 6))
	{
		// free(cmd_lst->env[i]);
		cmd_lst->env[i] = ft_strjoin_zero("OLDPWD=",
				old_pwd_path);
	}
}

int	run_cd_home(cmd_tree *cmd_lst, char *old_path)
{
	char	*env_return;

	env_return = get_env(cmd_lst, "HOME=");
	if (!env_return)
		return (1);
	if (!cmd_lst->exec.cmd_split[1])
	{
		if (env_return && chdir(env_return))
		{
			set_exit_status("Error while running <cd $HOME>", 1);
			// free(old_path);
			// free(env_return);
			return (1);
		}
		(void)old_path;
		// free(old_path);
		// free(env_return);
		return (1);
	}
	return (0);
}

void	cd(cmd_tree *cmd_lst)
{
	char	*old_path;
	char	*new_path;

	old_path = getcwd(NULL, 0);
	if (run_cd_home(cmd_lst, old_path))
		return ;
	if (chdir(cmd_lst->exec.cmd_split[1]))
	{
		if (cmd_lst->exec.cmd_split[1][0] == '-')
			set_exit_status("cd: invalid option", 1);
		else
			set_exit_status("cd: No such file or directory", 1);
		// free(old_path);
		return ;
	}
	new_path = getcwd(NULL, 0);
	update_env(cmd_lst, new_path, old_path);
	// free(old_path);
	// free(new_path);
}
