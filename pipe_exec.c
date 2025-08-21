/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:27:02 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/21 21:27:04 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_parent(int *pipefd, cmd_tree *cmd_lst, env_var *environ)
{
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

void	redirect_child(int *pipefd, cmd_tree *cmd_lst, env_var *environ)
{
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

int	exec_pipe(cmd_tree *cmd_lst, env_var *environ)
{
	int		pipefd[2];
	int		status;
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	left_pid = fork();
	if (left_pid == 0)
		redirect_child(pipefd, cmd_lst, environ);
	right_pid = fork();
	if (right_pid == 0)
		redirect_parent(pipefd, cmd_lst, environ);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	return (WEXITSTATUS(status));
}
