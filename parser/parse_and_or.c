/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_or.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:45:29 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 18:12:50 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_tree	*init_node_and(t_cmd_tree *n, t_cmd_tree *left, t_cmd_tree *right)
{
	n->and.left = left;
	n->and.right = right;
	return (n);
}

t_cmd_tree	*init_node_or(t_cmd_tree *n, t_cmd_tree *left, t_cmd_tree *right)
{
	n->or.left = left;
	n->or.right = right;
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
