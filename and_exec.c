/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:41:03 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 19:30:41 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_and(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	int	status;

	status = execute_node(cmd_lst->s_and.left, true, environ);
	if (status == 0 && g_exit_status == 0)
		status = execute_node(cmd_lst->s_and.right, true, environ);
	return (status);
}

int	exec_or(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	int	status;

	status = execute_node(cmd_lst->s_or.left, true, environ);
	if (status != 0 && g_exit_status != 0)
		status = execute_node(cmd_lst->s_or.right, true, environ);
	return (status);
}
