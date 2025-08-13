/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:12:06 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/12 19:50:56 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	len_equal(char *var)
{
	int	i;

	if (!var)
		return (0);
	i = -1;
	while (var[++i])
	{
		if (var[i] == '=')
			return (i);
	}
	return (0);
}

char	**put_quotes(char **expo)
{
	int		i;
	int		j;
	int		k;
	char	*tmp;

	i = -1;
	while (expo[++i])
	{
		tmp = malloc((ft_strlen(expo[i]) + 3) * sizeof(char));
		if (!tmp)
			return (set_exit_status("Failed to Malloc", 1));
		j = -1;
		k = 0;
		while (expo[i][++j] != '=')
			tmp[k++] = expo[i][j];
		tmp[k++] = expo[i][j];
		tmp[k++] = '"';
		while (expo[i][++j])
			tmp[k++] = expo[i][j];
		tmp[k] = '"';
		tmp[++k] = '\0';
		free(expo[i]);
		expo[i] = tmp;
	}
	return (expo);
}

void	export_without_args(env_var *environ)
{
	char	**expo;
	int		i;

	expo = copy_env(environ->env);
	expo = sort_export(expo);
	expo = put_quotes(expo);
	i = -1;
	while (expo[++i])
		printf("declare -x %s\n", expo[i]);
	i = -1;
	while (expo[++i])
		free(expo[i]);
	free(expo);
}

int	export_err(cmd_tree *cmd, char *str, int i, env_var *environ)
{
	if (str[0] == '-' && str[1] && i == 1)
	{
		g_exit_status = 2;
		printf("shell: export: %c%c: invalid option\n", str[0], str[1]);
		printf("export: usage: export [name[=value] ...]\n");
		return (0);
	}
	if (!valid_input(str))
	{
		g_exit_status = 1;
		printf("shell: export: `%s': not a valid identifier\n", str);
		return (1);
	}
	if (!ft_strcmp("PWD", cmd->exec.cmd_split[i])
		&& !find_var(environ->env, cmd->exec.cmd_split[i]))
	{
		add_env(environ, "PWD=");
		return (1);
	}
	return (2);
}
