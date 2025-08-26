/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:12:31 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 21:02:15 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	disable_echo(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	enable_echo(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	ctrl_c(int status)
{
	if (status == SIGINT)
	{
		g_exit_status = 130;
		disable_echo();
		if (ioctl(STDIN_FILENO, TIOCSTI, "\n") == -1)
			strerror(errno);
		write(1, "\n", 1);
		enable_echo();
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

void	ctrl_c_child(int status)
{
	if (status == SIGINT)
	{
		g_exit_status = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
	}
	if (status == SIGQUIT)
	{
		g_exit_status = 128 + status;
		ft_putstr_fd("Quit\n", 1);
	}
}
