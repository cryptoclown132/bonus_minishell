/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 00:59:06 by fjerinic          #+#    #+#             */
/*   Updated: 2025/08/22 14:28:30 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_input_helper_unset(char *cur_cmd, int n)
{
	if (!n)
	{
		if (!ft_isalpha(cur_cmd[n]) && !ft_strchr(cur_cmd, '_')
			&& !ft_strchr(cur_cmd, '\"') && !ft_strchr(cur_cmd, '\''))
		{
			set_exit_status("Error: invalid input to minishell unset", 1);
			return (0);
		}
		return (1);
	}
	else
	{
		if (!ft_isalnum(cur_cmd[n]) && !ft_strchr(cur_cmd, '_')
			&& !ft_strchr(cur_cmd, '\"') && !ft_strchr(cur_cmd, '\''))
		{
			set_exit_status("Error: invalid input to minishell unset", 1);
			return (0);
		}
		return (1);
	}
}

int	check_valid_unset_variable(char *cur_cmd)
{
	int	n;
	int	char_index;

	if (cur_cmd[0] == '-')
	{
		set_exit_status("Error", 2);
		return (0);
	}
	if (!valid_input_helper_unset(cur_cmd, 0))
		return (0);
	n = 1;
	char_index = get_char_index(cur_cmd, '=');
	while (cur_cmd[n] && n < char_index)
	{
		if (!valid_input_helper_unset(cur_cmd, n++))
			return (0);
	}
	return (1);
}

char	**reorder_array(char **env, int n)
{
	while (env[n + 1])
	{
		free(env[n]);
		env[n] = ft_strdup(env[n + 1]);
		n++;
	}
	free(env[n]);
	env[n] = NULL;
	return (env);
}

void	arrange_env(cmd_tree *cmd_struct, env_var *environ, int n, int j)
{
	if (environ->env && environ->env[n] && !ft_strncmp(environ->env[n],
			cmd_struct->exec.cmd_split[j],
			ft_strlen(cmd_struct->exec.cmd_split[j])))
		environ->env = reorder_array(environ->env, n);
	else
	{
		n = 0;
		while (environ->vars && environ->vars[n] && ft_strncmp(environ->vars[n],
				cmd_struct->exec.cmd_split[j],
				ft_strlen(cmd_struct->exec.cmd_split[j])))
			n++;
		if (environ->vars && environ->vars[n] && !ft_strncmp(environ->vars[n],
				cmd_struct->exec.cmd_split[j],
				ft_strlen(cmd_struct->exec.cmd_split[j])))
			environ->vars = reorder_array(environ->vars, n);
	}
}

void	unset(cmd_tree *cmd_struct, env_var *environ)
{
	int	n;
	int	j;

	j = -1;
	while (cmd_struct->exec.cmd_split[++j])
	{
		if (!check_valid_unset_variable(cmd_struct->exec.cmd_split[j]))
			return ;
		n = 0;
		while (environ->env && environ->env[n] && ft_strncmp(environ->env[n],
				cmd_struct->exec.cmd_split[j],
				ft_strlen(cmd_struct->exec.cmd_split[j])))
			n++;
		arrange_env(cmd_struct, environ, n, j);
	}
}
