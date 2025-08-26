/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:08:21 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 21:47:41 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_wildcard_args(char **wildcard_args)
{
	int	i;

	i = -1;
	while (wildcard_args && wildcard_args[++i])
		free(wildcard_args[i]);
	if (wildcard_args)
		free(wildcard_args);
	return (i);
}

int	count_args(t_tokens *token_lst)
{
	int			i;
	t_tokens	*tmp;
	char		**wildcard_args;

	i = 0;
	tmp = token_lst;
	while (tmp && (tmp->type == TOK_WORD || tmp->type == TOK_EQUAL
			|| tmp->type == TOK_WILD))
	{
		if (tmp->type == TOK_WILD)
		{
			wildcard_args = ft_split(tmp->token, ' ');
			i += count_wildcard_args(wildcard_args);
		}
		else
			i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**get_cmd_split(t_tokens **token_lst)
{
	t_cmd_split	sp;

	sp.i = count_args(*token_lst);
	if (!sp.i)
		return NULL;
	sp.new_cmd_split = ft_calloc(sp.i + 1, sizeof(char *));
	if (!sp.new_cmd_split)
		return (set_exit_status("Failed to Malloc", 1));
	sp.j = 0;
	while (sp.j < sp.i)
	{
		if ((*token_lst)->type == TOK_WILD)
		{
			sp.wildcard_args = ft_split((*token_lst)->token, ' ');
			sp.k = -1;
			while (sp.wildcard_args && sp.wildcard_args[++sp.k])
				sp.new_cmd_split[sp.j++] = sp.wildcard_args[sp.k];
			free(sp.wildcard_args);
		}
		else
			sp.new_cmd_split[sp.j++] = ft_strdup((*token_lst)->token);
		*token_lst = (*token_lst)->next;
	}
	sp.new_cmd_split[sp.j] = NULL;
	return (sp.new_cmd_split);
}

void	parse_subshell(t_tokens **token_lst, t_env_var environ,
		t_cmd_tree **cmd_node)
{
	t_cmd_tree	*child;

	*token_lst = (*token_lst)->next;
	child = parse_or(token_lst, environ);
	*token_lst = (*token_lst)->next;
	*cmd_node = new_node(NODE_SUBSHELL);
	(*cmd_node)->s_subshell.child = child;
}

void	parse_command(t_tokens **token_lst, t_env_var environ,
		t_cmd_tree **cmd_node)
{
	int	i;

	if (*token_lst && (*token_lst)->type == TOK_LPAREN)
	{
		parse_subshell(token_lst, environ, cmd_node);
		return ;
	}
	(*cmd_node)->s_exec.cmd_split = get_cmd_split(token_lst);
	(*cmd_node)->s_exec.cmd_path = NULL;
	(*cmd_node)->s_exec.idx_path = 0;
	i = -1;
	while ((*cmd_node)->s_exec.cmd_split && (*cmd_node)->s_exec.cmd_split[++i])
	{
		if (!ft_strchr((*cmd_node)->s_exec.cmd_split[i], '='))
		{
			(*cmd_node)->s_exec.cmd_path = get_cmd_path(environ,
					(*cmd_node)->s_exec.cmd_split[i], *cmd_node);
			(*cmd_node)->s_exec.idx_path = i;
			break ;
		}
	}
	return ;
}
