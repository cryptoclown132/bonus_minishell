/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 18:26:12 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/26 19:32:54 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*free_both_strjoin(char *s1, char *s2)
{
	char	*ptr;
	size_t	i;
	size_t	j;

	ptr = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!ptr)
		return (set_exit_status("Failed to Malloc", 1));
	i = 0;
	while (s1 && s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	if (s1)
		free(s1);
	j = 0;
	while (s2 && s2[j])
		ptr[i++] = s2[j++];
	ptr[i] = '\0';
	if (s2)
		free(s2);
	return (ptr);
}

int	valid_input(char *input)
{
	int	i;

	i = 0;
	if (!ft_isalpha(input[i]))
		return (0);
	while (input[++i] && input[i] != '=')
	{
		if (!ft_isalnum(input[i]) && input[i] != '_')
			return (0);
	}
	return (1);
}

void	free_env(t_env_var *environ)
{
	int	i;

	i = 0;
	if (environ && environ->env)
	{
		while (environ->env[i])
			free(environ->env[i++]);
		free(environ->env);
	}
	i = 0;
	if (environ && environ->vars)
	{
		while (environ->vars[i])
			free(environ->vars[i++]);
		free(environ->vars);
	}
}

char	*ft_strjoin_zero(char const *s1, char const *s2)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1)
		return (0);
	new_str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new_str)
	{
		set_exit_status("Error\n", 1);
		return (NULL);
	}
	while (s1[i] != 0)
	{
		new_str[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] != 0)
	{
		new_str[i++] = s2[j++];
	}
	new_str[i] = 0;
	return (new_str);
}
