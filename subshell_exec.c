/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:27:14 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/21 21:27:16 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_subshell(cmd_tree *cmd_lst, env_var *environ)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
		exit(execute_node(cmd_lst->subshell.child, true, environ));
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
