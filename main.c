/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:41:15 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 18:15:44 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

int	execute_node(cmd_tree *cmd_lst, bool in_parent, env_var *environ)
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

// sub*: ambiguous redirect
// << sub* dont work  and needs to be checked multiple files
// cat < test -> works onnly 1 file
// < sdgs
// bash: sdgs: No such file or directory
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

//julienkroger     65686   0.0  0.0 410059408    144 s007  R+    3:00PM   0:00.00 grep minishell
// julienkroger     65669   0.0  0.0 410209376   1824 s009  S+    3:00PM   0:00.01 ./minishell
// ps aux | grep minishell