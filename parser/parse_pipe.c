/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:39:19 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/27 12:17:26 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_tree	*parse_pipeline(t_tokens **token_lst, t_env_var environ)
{
	t_cmd_tree	*left;
	t_cmd_tree	*right;
	t_cmd_tree	*n;

	left = parse_cmd_sequence(token_lst, environ);
	while (*token_lst && (*token_lst)->type == TOK_PIPE)
	{
		*token_lst = (*token_lst)->next;
		right = parse_cmd_sequence(token_lst, environ);
		n = new_node(NODE_PIPE);
		n->s_pipe.left = left;
		n->s_pipe.right = right;
		left = n;
	}
	return (left);
}
