/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 23:03:13 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/26 18:05:25 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_var(char **vars, char *var)
{
	int	i;

	if (!vars)
		return (0);
	i = 0;
	while (vars[i])
	{
		if (!ft_strncmp(vars[i], var, len_equal(var)))
			return (1);
		i++;
	}
	return (0);
}

void	del_var(t_env_var *environ, char *var)
{
	int		i;
	int		j;
	char	**varcp;

	varcp = environ->vars;
	environ->vars = malloc(count_env_len(varcp) * sizeof(char *));
	if (!environ->vars)
		return (set_exit_void("Failed to Malloc", 1));
	i = 0;
	j = 0;
	while (varcp[j])
	{
		if (ft_strncmp(varcp[j], var, ft_strlen(var)) == 0)
			free(varcp[j++]);
		else
		{
			environ->vars[i++] = ft_strdup(varcp[j]);
			free(varcp[j++]);
		}
	}
	environ->vars[i] = NULL;
	free(varcp);
}
