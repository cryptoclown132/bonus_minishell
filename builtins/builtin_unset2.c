/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 20:37:34 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/22 14:27:02 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_char_index(const char *s, int c)
{
	int		n;

	if (!s)
		return (0);
	n = 0;
	while (s[n] != 0 && s[n] != (char)c)
		n++;
	return (n);
}
