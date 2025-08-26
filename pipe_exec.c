/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:27:02 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 18:16:21 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_parent(int *pipefd, t_cmd_tree *cmd_lst, t_env_var *environ)
{
	get_signals_child();
	if (cmd_lst->infile != 0 && cmd_lst->infile != -1)
	{
		close(pipefd[1]);
		dup2(cmd_lst->infile, STDIN_FILENO);
		close(cmd_lst->infile);
		close(pipefd[0]);
		exit(execute_node(cmd_lst->pipe.right, false, environ));
	}
	else
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		exit(execute_node(cmd_lst->pipe.right, false, environ));
	}
}

void	redirect_child(int *pipefd, t_cmd_tree *cmd_lst, t_env_var *environ)
{
	get_signals_child();
	if (cmd_lst->outfile != 1 && cmd_lst->outfile != -1)
	{
		close(pipefd[0]);
		dup2(cmd_lst->outfile, STDOUT_FILENO);
		close(cmd_lst->outfile);
		close(pipefd[1]);
		exit(execute_node(cmd_lst->pipe.left, false, environ));
	}
	else
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(execute_node(cmd_lst->pipe.left, false, environ));
	}
}

void	pipe_error(void)
{
	perror("pipe");
	exit(1);
}

int	exec_pipe(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	t_exec_pipe	p;

	if (pipe(p.pipefd) == -1)
		pipe_error();
	ignore_signals(&p.old_int, &p.old_quit);
	p.left_pid = fork();
	if (p.left_pid == 0)
		redirect_child(p.pipefd, cmd_lst, environ);
	p.right_pid = fork();
	if (p.right_pid == 0)
		redirect_parent(p.pipefd, cmd_lst, environ);
	close(p.pipefd[0]);
	close(p.pipefd[1]);
	waitpid(p.left_pid, &p.status, 0);
	waitpid(p.right_pid, &p.right_status, 0);
	if (WTERMSIG(p.status) == 13 || WTERMSIG(p.right_status) == 13)
		write(1, "\n", 1);
	activate_signals(&p.old_int, &p.old_quit);
	return (get_exit_status(p.right_status));
}
