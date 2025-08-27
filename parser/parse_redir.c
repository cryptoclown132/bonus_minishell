/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:10:19 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/27 16:23:47 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_tree	*parse_cmd_sequence(t_tokens **token_lst, t_env_var environ)
{
	t_cmd_tree	*cmd_node;

	cmd_node = new_node(NODE_EXEC);
	while (*token_lst && (*token_lst)->type != TOK_AND && (*token_lst)->type != TOK_OR && (*token_lst)->type != TOK_PIPE)
	{
		if ((*token_lst)->type == TOK_IN || (*token_lst)->type == TOK_OUT
		|| (*token_lst)->type == TOK_APP || (*token_lst)->type == TOK_DOC)
			parse_redirection(token_lst, environ, &cmd_node);
		else
			parse_command(token_lst, environ, &cmd_node);
	}
	return (cmd_node);
}
