/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:41:03 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 18:14:52 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_and(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	int	status;

	status = execute_node(cmd_lst->and.left, true, environ);
	if (status == 0 && g_exit_status == 0)
		status = execute_node(cmd_lst->and.right, true, environ);
	return (status);
}

int	exec_or(cmd_tree *cmd_lst, env_var *environ)
{
	int	status;

	status = execute_node(cmd_lst->or.left, true, environ);
	if (status != 0 && g_exit_status != 0)
		status = execute_node(cmd_lst->or.right, true, environ);
	return (status);
}
