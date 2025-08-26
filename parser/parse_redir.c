/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:10:19 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 18:14:15 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_tree	*parse_cmd_sequence(t_tokens **token_lst, t_env_var environ)
{
	t_cmd_tree	*cmd_node;

	cmd_node = new_node(NODE_EXEC);
	parse_redirection(token_lst, environ, &cmd_node);
	parse_command(token_lst, environ, &cmd_node);
	parse_redirection(token_lst, environ, &cmd_node);
	return (cmd_node);
}
