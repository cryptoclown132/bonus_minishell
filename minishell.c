/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:25:54 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/24 15:34:42 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd()
{
	int fd;

	fd = 2;
	while (++fd <= 1048575)
		close(fd);
}

int	minishell(cmd_tree *cmd_lst, env_var *environ)
{
	char	*input;

	get_signals();
	// g_exit_status = 0;
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
	if (g_exit_status == 130 || !cmd_lst || cmd_lst->err == -2)
	{
		free_cmd_tree(cmd_lst);
		return (0);
	}
	execute_node(cmd_lst, true, environ);
	free_cmd_tree(cmd_lst);
	// close_fd();
	return (0);
}
