/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:41:15 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 20:00:24 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

int	execute_node(t_cmd_tree *cmd_lst, bool in_parent, t_env_var *environ)
{
	if (!cmd_lst)
		return (-1);
	if (cmd_lst->type == NODE_EXEC)
		return (exec_cmd(cmd_lst, in_parent, environ));
	else if (cmd_lst->type == NODE_SUBSHELL)
		return (exec_subshell(cmd_lst, environ));
	else if (cmd_lst->type == NODE_PIPE)
		return (exec_pipe(cmd_lst, environ));
	else if (cmd_lst->type == NODE_AND)
		return (exec_and(cmd_lst, environ));
	else if (cmd_lst->type == NODE_OR)
		return (exec_or(cmd_lst, environ));
	return (-1);
}

int	main(int argc, char *argv[], char **envp)
{
	t_cmd_tree	*cmd_lst;
	t_env_var		environ;

	cmd_lst = NULL;
	if (argc > 1)
		return (0);
	(void)argv;
	g_exit_status = 0;
	environ.env = copy_env(envp);
	environ.vars = NULL;
	while (1)
		minishell(cmd_lst, &environ);
}
