/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 11:48:59 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/24 23:14:35 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_cmd_tree(t_tokens **token_lst, cmd_tree **cmd_lst, env_var environ)
{
	t_tokens	*curr_token_lst;
	t_tokens	*tmp_t;

	curr_token_lst = *token_lst;
	*cmd_lst = parse_or(&curr_token_lst, environ);
	while (*token_lst != NULL)
	{
		tmp_t = (*token_lst)->next;
		free((*token_lst)->token);
		free(*token_lst);
		*token_lst = tmp_t;
	}
	if (g_exit_status == 130)
		return (0);
	return (1);
}
