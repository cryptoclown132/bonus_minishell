/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:25:54 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/27 14:49:47 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell(t_cmd_tree *cmd_lst, t_env_var *environ)
{
	char	*input;

	get_signals();
	input = user_input();
	if (!input)
	{
		printf("exit\n");
		g_exit_status = 130;
		free_env(environ);
		exit(g_exit_status);
	}
	cmd_lst = parse(cmd_lst, &input, *environ);
	add_history(input);
	free(input);
	if (g_exit_status == 130 || g_exit_status == 2 || !cmd_lst)
	{
		free_cmd_tree(cmd_lst);
		return (0);
	}
	execute_node(cmd_lst, true, environ);
	free_cmd_tree(cmd_lst);
	return (0);
}
