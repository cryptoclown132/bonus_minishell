/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:08:21 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/21 21:09:56 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_args(t_tokens *token_lst)
{
	int			i;
	t_tokens	*tmp;

	i = 0;
	tmp = token_lst;
	while (tmp && (tmp->type == TOK_WORD || tmp->type == TOK_EQUAL))
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**get_cmd_split(t_tokens **token_lst)
{
	char	**new_cmd_split;
	int		i;
	int		j;

	i = count_args(*token_lst);
	new_cmd_split = ft_calloc(i + 1, sizeof(char*));
	if (!new_cmd_split)
		return (set_exit_status("Failed to Malloc", 1));
	j = -1;
	while (++j < i)
	{
		new_cmd_split[j] = ft_strdup((*token_lst)->token);
		*token_lst = (*token_lst)->next;
	}
	new_cmd_split[j] = NULL;
	return (new_cmd_split);
}

void	parse_command(t_tokens **token_lst, env_var environ, cmd_tree **cmd_node)
{
	cmd_tree	*child;
	int			i;

	if (*token_lst && (*token_lst)->type == TOK_LPAREN)
	{
		*token_lst = (*token_lst)->next;
		child = parse_or(token_lst, environ);
		*token_lst = (*token_lst)->next;
		*cmd_node = new_node(NODE_SUBSHELL);
		(*cmd_node)->subshell.child = child;
		return ;
	}
	(*cmd_node)->exec.cmd_split = get_cmd_split(token_lst);
	(*cmd_node)->exec.cmd_path = NULL;
	(*cmd_node)->exec.idx_path = 0;
	i = -1;
	while ((*cmd_node)->exec.cmd_split && (*cmd_node)->exec.cmd_split[++i])
	{
		if (!ft_strchr((*cmd_node)->exec.cmd_split[i], '='))
		{
			(*cmd_node)->exec.cmd_path = get_cmd_path(environ,
					(*cmd_node)->exec.cmd_split[i], *cmd_node);
			(*cmd_node)->exec.idx_path = i;
			break ;
		}
	}
	return ;
}
