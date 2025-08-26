/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_or.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:45:29 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 19:40:26 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_tree	*init_node_and(t_cmd_tree *n, t_cmd_tree *left, t_cmd_tree *right)
{
	n->s_and.left = left;
	n->s_and.right = right;
	return (n);
}

t_cmd_tree	*init_node_or(t_cmd_tree *n, t_cmd_tree *left, t_cmd_tree *right)
{
	n->s_or.left = left;
	n->s_or.right = right;
	return (n);
}

t_cmd_tree	*parse_or(t_tokens **token_lst, t_env_var environ)
{
	t_cmd_tree	*left;
	t_cmd_tree	*right;
	t_cmd_tree	*n;
	int			op;

	left = parse_pipeline(token_lst, environ);
	while (*token_lst && ((*token_lst)->type == TOK_AND
			|| (*token_lst)->type == TOK_OR))
	{
		op = (*token_lst)->type;
		*token_lst = (*token_lst)->next;
		right = parse_pipeline(token_lst, environ);
		if (op == TOK_AND)
			n = new_node(NODE_AND);
		else
			n = new_node(NODE_OR);
		if (op == TOK_AND)
			n = init_node_and(n, left, right);
		else
			n = init_node_or(n, left, right);
		left = n;
	}
	return (left);
}
