/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkroger <jkroger@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 15:11:27 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/26 20:36:10 by jkroger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_token(t_tokens **token_lst, t_tokens *token)
{
	t_tokens	*first;

	first = *token_lst;
	if (*token_lst == NULL)
		*token_lst = token;
	else
	{
		while (first->next != NULL)
			first = first->next;
		first->next = token;
	}
}

t_tokens	*lexer_2(char *input, int *i, t_env_var environ)
{
	t_tokens	*token;

	if (input[*i] == '(')
		token = init_token(ft_strdup("("), i, TOK_LPAREN);
	else if (input[*i] == ')')
		token = init_token(ft_strdup("("), i, TOK_RPAREN);
	else if (input[*i] == '|' && input[*i + 1] == '|')
		token = init_token(ft_strdup("||"), i, TOK_OR);
	else if (input[*i] == '|')
		token = init_token(ft_strdup("|"), i, TOK_PIPE);
	else if (input[*i] == '&' && input[*i + 1] == '&')
		token = init_token(ft_strdup("&&"), i, TOK_AND);
	else if (input[*i] == '<' && input[*i + 1] != '<')
		token = init_redir(input, i, TOK_IN, environ);
	else if (input[*i] == '<' && input[*i + 1] == '<')
		token = init_redir(input, i, TOK_DOC, environ);
	else if (input[*i] == '>' && input[*i + 1] != '>')
		token = init_redir(input, i, TOK_OUT, environ);
	else if (input[*i] == '>' && input[*i + 1] == '>')
		token = init_redir(input, i, TOK_APP, environ);
	else if (input[*i] != ' ')
		token = init_token_word(input, i, environ);
	else
		token = NULL;
	return (token);
}

t_tokens	*lexer(t_tokens *token_lst, char *input, t_env_var environ)
{
	int			i;
	t_tokens	*token;

	token_lst = NULL;
	i = -1;
	while (input[++i])
	{
		token = lexer_2(input, &i, environ);
		if (input[i] != ' ')
			add_token(&token_lst, token);
	}
	return (token_lst);
}
