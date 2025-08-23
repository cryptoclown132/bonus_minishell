/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:01:37 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/22 14:06:25 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_var_loop(env_var *environ, char **varcp, char *var)
{
	int	i;

	i = 0;
	while (varcp && varcp[i])
	{
		if (!ft_strncmp(varcp[i], var, len_equal(var)))
			environ->vars[i] = ft_strdup(var);
		else
			environ->vars[i] = ft_strdup(varcp[i]);
		i++;
	}
	if (!ft_var(varcp, var))
		environ->vars[i++] = ft_strdup(var);
	environ->vars[i] = NULL;
}

void	add_var(char *var, env_var *environ)
{
	int		i;
	char	**varcp;

	varcp = environ->vars;
	if (!var)
		return ;
	if (ft_var(varcp, var))
		environ->vars = malloc((count_env_len(varcp) + 1) * sizeof(char *));
	else
		environ->vars = malloc((count_env_len(varcp) + 2) * sizeof(char *));
	if (!environ->vars)
		return (set_exit_void("Failed to Malloc", 1));
	add_var_loop(environ, varcp, var);
	if (varcp)
	{
		i = -1;
		while (varcp[++i])
			free(varcp[i]);
		free(varcp);
	}
}

int	strnstr_equal(const char *haystack, const char *needle)
{
	size_t		i;

	if (!haystack || !needle)
		return (0);
	i = 0;
	while (i < ft_strlen(haystack))
	{
		if (haystack[i] == needle[i] && needle[i] == '=')
			return (1);
		else if (haystack[i] == needle[i])
			i++;
		else
			return (0);
	}
	return (0);
}

int	find_var_in_env(char **vars, char *var)
{
	int		i;

	i = 0;
	while (vars && vars[i])
	{
		if (strnstr_equal(vars[i], var))
			return (1);
		i++;
	}
	return (0);
}

int	var_lst(cmd_tree *cmd_lst, env_var *environ)
{
	int	i;

	if (cmd_lst->type != NODE_EXEC || !cmd_lst->exec.cmd_split)
		return (0);
	if (ft_strchr(cmd_lst->exec.cmd_split[0], '=')
		&& ft_isalpha(cmd_lst->exec.cmd_split[0][0]) && !cmd_lst->exec.cmd_path)
	{
		i = -1;
		while (cmd_lst->exec.cmd_split[++i])
		{
			if (find_var_in_env(environ->env, cmd_lst->exec.cmd_split[i]))
				add_env(environ, cmd_lst->exec.cmd_split[i]);
			else
				add_var(cmd_lst->exec.cmd_split[i], environ);
		}
		return (1);
	}
	return (0);
}
