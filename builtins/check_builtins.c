/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 19:40:38 by fjerinic          #+#    #+#             */
/*   Updated: 2025/08/21 21:03:36 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_util(cmd_tree *cmd_lst)
{
	if (!ft_strncmp(cmd_lst->exec.cmd_split[0], "exit", 5))
	{
		builtin_exit(cmd_lst);
		return (1);
	}
	return (0);
}

int	unset_util(cmd_tree *cmd_lst, env_var *environ)
{
	if (!ft_strncmp(cmd_lst->exec.cmd_split[0], "unset", 6))
	{
		unset(cmd_lst, environ);
		return (1);
	}
	return (0);
}

void	try_env(cmd_tree *cmd_lst, env_var *environ)
{
	if (count_env_len(cmd_lst->exec.cmd_split) > 1)
	{
		set_exit_status("Error: env", 1);
		return ;
	}
	env(environ);
	return ;
}

int	run_builtin(cmd_tree *cmd_lst, env_var *environ)
{
	get_signals_child();
	if (!is_builtin(cmd_lst->exec.cmd_split[0]) && cmd_lst->err != 0)
	{
		set_err(cmd_lst->err_file, cmd_lst->err);
		return (1);
	}
	if (!ft_strncmp(cmd_lst->exec.cmd_split[0], "cd", 3))
	{
		cd(cmd_lst, environ);
		return (1);
	}
	else if (!ft_strncmp(cmd_lst->exec.cmd_split[0], "export", 7))
	{
		builtin_export(cmd_lst, environ);
		return (1);
	}
	else if (unset_util(cmd_lst, environ))
		return (1);
	else if (exit_util(cmd_lst))
		return (1);
	return (0);
}

int	run_builtin2(cmd_tree *cmd_lst, env_var *environ)
{
	if (!cmd_lst->exec.cmd_split)
		return (0);
	if (!ft_strncmp(cmd_lst->exec.cmd_split[0], "echo", 5))
	{
		echo(cmd_lst);
		return (1);
	}
	else if (!ft_strncmp(cmd_lst->exec.cmd_split[0], "pwd", 4))
	{
		if (cmd_lst->exec.cmd_split[1]
			&& cmd_lst->exec.cmd_split[1][0] == '-')
		{
			set_exit_status("pwd error", 1);
			return (1);
		}
		pwd();
		return (1);
	}
	else if (!ft_strncmp(cmd_lst->exec.cmd_split[0], "env", 4))
	{
		try_env(cmd_lst, environ);
		return (1);
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (0);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "export") == 0)
		return (0);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (0);
	else if (ft_strcmp(cmd, "env") == 0)
		return (0);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (0);
	return (1);
}
