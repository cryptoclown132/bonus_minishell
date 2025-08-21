/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:38:59 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/21 21:25:13 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*user_input(void)
{
	char	*prompt;
	char	*input;

	prompt = "shell$> ";
	input = readline(prompt);
	return (input);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*c;
	unsigned char	*d;

	if (!s1 || !s2)
		return (1);
	c = (unsigned char *)s1;
	d = (unsigned char *)s2;
	i = 0;
	while (s1[i] || s2[i])
	{
		if (c[i] == d[i])
			i++;
		else
			return (c[i] - d[i]);
	}
	return (0);
}
