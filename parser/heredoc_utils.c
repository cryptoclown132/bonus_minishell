/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 15:41:01 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/26 18:11:57 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_dollar(char *input)
{
	int	i;

	i = -1;
	while (input[++i])
	{
		if (input[i] == '$')
			return (1);
	}
	return (0);
}

void	here_doc_loop(char	*limiter, int fd, t_env_var environ)
{
	char		*input;
	char		*tmp;

	input = readline("> ");
	while (input && g_exit_status != 130)
	{
		if (ft_strcmp(input, limiter) == 0)
			break ;
		if (is_dollar(input))
		{
			tmp = input;
			input = get_var(tmp, environ);
			free(tmp);
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
		input = readline("> ");
	}
	free(input);
}
