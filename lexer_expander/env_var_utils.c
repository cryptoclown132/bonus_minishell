/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 20:51:29 by jkroger           #+#    #+#             */
/*   Updated: 2025/05/01 12:49:53 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*var_finder(char **envp, char *var)
{
	int	i;
	int	j;

	i = -1;
	var = edited_strjoin(var, "=");
	if (!var)
		return (set_exit_status("Failed to Malloc", 1));
	while (envp && envp[++i])
	{
		if (ft_strnstr(envp[i], var, ft_strlen(var)))
		{
			j = ft_strlen(var);
			free(var);
			return (ft_substr(envp[i], j, ft_strlen(envp[i]) - j + 1));
		}
	}
	free(var);
	return (NULL);
}

char	*var_exist(char *token, char **envp, int *i, char **var_value)
{
	int		j;

	j = *i + 1;
	while ((token[j] >= 'a' && token[j] <= 'z')
		|| (token[j] >= 'A' && token[j] <= 'Z')
		|| (token[j] >= '0' && token[j] <= '9')
		|| token[j] != '_')
		j++;
	if (token[*i] == '$' && token[*i + 1] == '?')
	{
		*var_value = ft_itoa(g_exit_status);
		j++;
	}
	else
		*var_value = var_finder(envp, ft_substr(token, *i + 1, j - *i - 1));
	*i = j - 1;
	return *var_value;
}

int	get_len(char *token, char **envp)
{
	int		i;
	int		var_len;
	char	*var_value;

	i = -1;
	var_len = 0;
	while (token[++i])
	{
		if (token[i] != '$' || token[i] == '$' && token[i + 1] == '$')
			var_len++;
		else
		{
			var_exist(token, envp, &i, &var_value);
			var_len += ft_strlen(var_value);
			free(var_value);
		}
	}
	return (var_len);
}

void	concat_var(char *token, char **envp, char **var_value, int *i, int *j)
{
	char	*tmp;
	int		k;
	
	tmp = var_exist(token, envp, i, tmp);
	k = 0;
	if (tmp)
	{
		while (tmp && tmp[k])
			*var_value[*j++] = tmp[k++];
		free(tmp);
	}
}

char	*get_var(char *token, char **envp)
{
	char	*var_value;
	int		i;
	int		j;

	var_value = malloc((get_len(token, envp) + 1) * sizeof(char));
	if (!var_value)
		return (set_exit_status("Failed to Malloc", 1));
	i = 0;
	j = 0;
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] != '$')
			concat_var(token, envp, &var_value, &i, &j);
		else
			var_value[j++] = token[i++];
	}
	var_value[j] = '\0';
	return (var_value);
}
