/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:18:36 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 18:14:40 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_tree	*new_node(t_node_type t)
{
	t_cmd_tree	*n;

	n = ft_calloc(1, sizeof(*n));
	n->type = t;
	n->infile = -1;
	n->outfile = -1;
	n->err = 0;
	n->err_file = NULL;
	return (n);
}

char	*and_op_loop(char *input)
{
	char	*tmp_input;
	char	*trim_input;

	tmp_input = readline("> ");
	while (tmp_input && g_exit_status != 130)
	{
		input = edited_strjoin(input, tmp_input);
		if (!lex_error_check(tmp_input))
		{
			free(tmp_input);
			tmp_input = NULL;
			return (input);
		}
		free(tmp_input);
		tmp_input = NULL;
		trim_input = ft_strtrim(input, " \t\n");
		if ((trim_input[ft_strlen(trim_input) - 1] == '&'
				&& trim_input[ft_strlen(trim_input) - 2] == '&')
			|| trim_input[ft_strlen(trim_input) - 1] == '|')
			tmp_input = readline("> ");
		free(trim_input);
	}
	free(tmp_input);
	return (input);
}

char	*check_for_open_and(char *input)
{
	char	*trim_input;

	g_exit_status = 0;
	trim_input = ft_strtrim(input, " \t\n");
	if ((trim_input[ft_strlen(trim_input) - 1] == '&'
			&& trim_input[ft_strlen(trim_input) - 2] == '&')
		|| trim_input[ft_strlen(trim_input) - 1] == '|')
		input = and_op_loop(input);
	free(trim_input);
	return (input);
}

t_cmd_tree	*parse(t_cmd_tree *cmd_lst, char **input, t_env_var environ)
{
	t_tokens	*token_lst;
	int			prev_exit_status;

	token_lst = NULL;
	if (!lex_error_check(*input))
	{
		if (cmd_lst)
			cmd_lst->err = -2;
		return (cmd_lst);
	}
	prev_exit_status = g_exit_status;
	*input = check_for_open_and(*input);
	if (g_exit_status == 2 || g_exit_status == 130)
	{
		// free(*input);
		if (cmd_lst)
			cmd_lst->err = -2;
		return (cmd_lst);
	}
	g_exit_status = prev_exit_status;
	token_lst = lexer(token_lst, *input, environ);
	g_exit_status = 0;
	if (init_cmd_tree(&token_lst, &cmd_lst, environ) != 1)
		cmd_lst->err = -2;
	return (cmd_lst);
}
