/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 19:48:05 by fjerinic          #+#    #+#             */
/*   Updated: 2025/08/27 11:40:13 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	echo_check_args(t_cmd_tree *cmd_struct)
{
	if (!cmd_struct->s_exec.cmd_split[cmd_struct->s_exec.idx_path + 1])
	{
		ft_printf("\n");
		return (1);
	}
	else if (!ft_strncmp(cmd_struct->s_exec.cmd_split[cmd_struct->s_exec.idx_path + 1], "-n", 3)
		&& !cmd_struct->s_exec.cmd_split[cmd_struct->s_exec.idx_path + 2])
		return (1);
	return (0);
}

static void	echo_no_n(t_cmd_tree *cmd_struct)
{
	int	n;

	n = cmd_struct->s_exec.idx_path + 2;
	while (cmd_struct->s_exec.cmd_split[n])
	{
		if (n > 2)
			ft_printf(" ");
		ft_printf("%s", cmd_struct->s_exec.cmd_split[n++]);
	}
}

static void	echo_n(t_cmd_tree *cmd_struct)
{
	int	n;

	n = cmd_struct->s_exec.idx_path + 1;
	while (cmd_struct->s_exec.cmd_split[n])
	{
		if (n - cmd_struct->s_exec.idx_path > 1)
			ft_printf(" ");
		ft_printf("%s", cmd_struct->s_exec.cmd_split[n++]);
	}
	ft_printf("\n");
}

void	echo(t_cmd_tree *cmd_struct)
{
	if (echo_check_args(cmd_struct))
		return ;
	g_exit_status = 0;
	if (cmd_struct->s_exec.cmd_split[cmd_struct->s_exec.idx_path + 1]
		&& !ft_strncmp(cmd_struct->s_exec.cmd_split[cmd_struct->s_exec.idx_path + 1], "-n", 3))
		echo_no_n(cmd_struct);
	else
		echo_n(cmd_struct);
}
