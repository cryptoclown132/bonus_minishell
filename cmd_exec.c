/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:41:10 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 21:57:00 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin(t_cmd_tree *cmd_lst, bool in_parent, t_env_var *environ)
{
	if (cmd_lst->err != 0)
	{
		set_err(cmd_lst->err_file, cmd_lst->err);
		return (1);
	}
	run_builtin(cmd_lst, environ);
	if (in_parent)
		return (0);
	exit(g_exit_status);
}

void	redir_files(t_cmd_tree *cmd_lst)
{
	if (cmd_lst->infile != -1)
	{
		dup2(cmd_lst->infile, STDIN_FILENO);
		close(cmd_lst->infile);
	}
	if (cmd_lst->outfile != -1)
	{
		dup2(cmd_lst->outfile, STDOUT_FILENO);
		close(cmd_lst->outfile);
	}
}

int	check_path(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
			return (1);
	}
	return (0);
}

void	fork_correct(t_cmd_tree *cmd_lst, bool in_parent, t_env_var *environ)
{
	get_signals_child();
	redir_files(cmd_lst);
	if (run_builtin2(cmd_lst, environ))
		exit(g_exit_status);
	if (run_builtin(cmd_lst, environ))
		handle_builtin(cmd_lst, in_parent, environ);
	if (!cmd_lst->s_exec.cmd_path && !cmd_lst->s_exec.cmd_split)
		exit(0);
	if (check_path(environ->env))
		execve(cmd_lst->s_exec.cmd_path, cmd_lst->s_exec.cmd_split
			+ cmd_lst->s_exec.idx_path, environ->env);
	else
		execve(cmd_lst->s_exec.cmd_path, cmd_lst->s_exec.cmd_split
			+ cmd_lst->s_exec.idx_path, environ->vars);
	set_exit_status("Error: command not found", 127);
	exit(127);
}

int	exec_cmd(t_cmd_tree *cmd_lst, bool in_parent, t_env_var *environ)
{
	pid_t				pid;
	int					status;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	if ((in_parent && run_builtin(cmd_lst, environ))
		|| var_lst(cmd_lst, environ))
		return (0);
	ignore_signals(&old_int, &old_quit);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
		fork_correct(cmd_lst, in_parent, environ);
	activate_signals(&old_int, &old_quit);
	waitpid(pid, &status, 0);
	return (get_exit_status(status));
}
