/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 11:10:15 by raweber           #+#    #+#             */
/*   Updated: 2025/08/26 18:05:17 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_numeric(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	builtin_exit(t_cmd_tree *cmd_struct, t_env_var *environ)
{
	ft_printf("exit\n");
	if (cmd_struct->exec.cmd_split[1]
		&& !check_numeric(cmd_struct->exec.cmd_split[1]))
	{
		ft_putstr_fd("shell: exit: numeric argument required\n", 2);
		g_exit_status = 255;
	}
	else if (cmd_struct->exec.cmd_split[1] && cmd_struct->exec.cmd_split[2])
	{
		ft_putstr_fd("shell: exit: too many arguments\n", 2);
		g_exit_status = 1;
		return ;
	}
	else if (cmd_struct->exec.cmd_split[1]
		&& check_numeric(cmd_struct->exec.cmd_split[1]))
		g_exit_status = ft_atoi(cmd_struct->exec.cmd_split[1]);
	else
		g_exit_status = 0;
	free_cmd_tree(cmd_struct);
	free_env(environ);
	exit(g_exit_status);
}
