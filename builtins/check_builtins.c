/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 19:40:38 by fjerinic          #+#    #+#             */
/*   Updated: 2025/08/26 21:51:08 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_util(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	if (cmd_lst->s_exec.cmd_split && !ft_strncmp(cmd_lst->s_exec.cmd_split[0], "exit", 5))
	{
		builtin_exit(cmd_lst, environ);
		return (1);
	}
	return (0);
}

int	unset_util(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	if (cmd_lst->s_exec.cmd_split && !ft_strncmp(cmd_lst->s_exec.cmd_split[0], "unset", 6))
	{
		unset(cmd_lst, environ);
		return (1);
	}
	return (0);
}

void	try_env(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	if (count_env_len(cmd_lst->s_exec.cmd_split) > 1)
	{
		set_exit_status("Error: env", 1);
		return ;
	}
	env(environ);
	return ;
}

int	run_builtin(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	get_signals_child();
	if (cmd_lst->s_exec.cmd_split && !is_builtin(cmd_lst->s_exec.cmd_split[0]) && cmd_lst->err != 0)
	{
		set_err(cmd_lst->err_file, cmd_lst->err);
		return (1);
	}
	if (cmd_lst->s_exec.cmd_split && !ft_strncmp(cmd_lst->s_exec.cmd_split[0], "cd", 3))
	{
		cd(cmd_lst, environ);
		return (1);
	}
	else if (cmd_lst->s_exec.cmd_split && !ft_strncmp(cmd_lst->s_exec.cmd_split[0], "export", 7))
	{
		builtin_export(cmd_lst, environ);
		return (1);
	}
	else if (unset_util(cmd_lst, environ))
		return (1);
	else if (exit_util(cmd_lst, environ))
		return (1);
	return (0);
}

int	run_builtin2(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	if (!cmd_lst->s_exec.cmd_split)
		return (0);
	if (!ft_strncmp(cmd_lst->s_exec.cmd_split[0], "echo", 5))
	{
		echo(cmd_lst);
		return (1);
	}
	else if (!ft_strncmp(cmd_lst->s_exec.cmd_split[0], "pwd", 4))
	{
		if (cmd_lst->s_exec.cmd_split[1]
			&& cmd_lst->s_exec.cmd_split[1][0] == '-')
		{
			set_exit_status("pwd error", 1);
			return (1);
		}
		pwd();
		return (1);
	}
	else if (!ft_strncmp(cmd_lst->s_exec.cmd_split[0], "env", 4))
	{
		try_env(cmd_lst, environ);
		return (1);
	}
	return (0);
}
