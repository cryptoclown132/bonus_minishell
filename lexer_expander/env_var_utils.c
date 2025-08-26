/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 20:51:29 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/26 18:10:24 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*var_finder_2(char **env, char *var)
{
	int	j;
	int	i;

	i = -1;
	while (env && env[++i])
	{
		if (ft_strnstr(env[i], var, ft_strlen(var)))
		{
			j = ft_strlen(var);
			return (ft_substr(env[i], j, ft_strlen(env[i]) - j + 1));
		}
	}
	return (NULL);
}

char	*var_finder(t_env_var environ, char *var)
{
	char	*var_found;

	var = edited_strjoin(var, "=");
	if (!var)
		return (set_exit_status("Failed to Malloc", 1));
	var_found = var_finder_2(environ.env, var);
	if (!var_found)
		var_found = var_finder_2(environ.vars, var);
	free(var);
	return (var_found);
}

void	var_exist(char *token, t_env_var environ, int *i, char **var_value)
{
	int		j;

	j = *i + 1;
	while ((token[j] >= 'a' && token[j] <= 'z')
		|| (token[j] >= 'A' && token[j] <= 'Z')
		|| (token[j] >= '0' && token[j] <= '9')
		|| token[j] == '_' || token[j] == '/')
		j++;
	if (token[*i] == '$' && token[*i + 1] == '?')
	{
		*var_value = ft_itoa(g_exit_status);
		j++;
	}
	else
		*var_value = var_finder(environ, ft_substr(token, *i + 1, j - *i - 1));
	*i = j;
}

int	get_len(char *token, t_env_var environ)
{
	int		i;
	int		var_len;
	char	*var_value;

	i = -1;
	var_len = 0;
	while (token[++i])
	{
		if (token[i] != '$' || (token[i] == '$' && token[i + 1] == '$'))
			var_len++;
		else
		{
			var_exist(token, environ, &i, &var_value);
			var_len += ft_strlen(var_value);
			free(var_value);
		}
	}
	return (var_len);
}

void	cat_var(char *token, t_env_var environ, char **var_value, t_get_var *v)
{
	char	*tmp;
	int		k;

	tmp = NULL;
	var_exist(token, environ, &v->i, &tmp);
	k = 0;
	if (tmp)
	{
		while (tmp && tmp[k])
			(*var_value)[((*v).j)++] = tmp[k++];
		free(tmp);
	}
}
