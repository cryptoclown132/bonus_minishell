/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 15:11:27 by jkroger           #+#    #+#             */
/*   Updated: 2025/04/26 12:56:37 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tokens	*lexer(t_tokens *token_lst, char *input, char **envp)
{
	int			i;
	t_tokens	*token;

	token_lst = NULL;
	i = -1;


	while (input[++i])
	{
		if (input[i] == '(')
			token = init_token(ft_strdup("("), &i, TOK_LPAREN);
		else if (input[i] == ')')
			token = init_token(ft_strdup("("), &i, TOK_RPAREN);
			
		else if (input[i] == '|' && input[i + 1] == '|') //i++
			token = init_token(ft_strdup("||"), &i, TOK_OR);
		else if (input[i] == '|')
			token = init_token(ft_strdup("|"), &i, TOK_PIPE);
		else if (input[i] == '&' && input[i + 1] == '&') // i++
			token = init_token(ft_strdup("&&"), &i, TOK_AND);
		else if (input[i] == '<' && input[i + 1] != '<')
			token = init_redir(input, &i, TOK_IN, envp);
		else if (input[i] == '<' && input[i + 1] == '<')
			token = init_redir(input, &i, TOK_DOC, envp);
		else if (input[i] == '>' && input[i + 1] != '>')
			token = init_redir(input, &i, TOK_OUT, envp);
		else if (input[i] == '>' && input[i + 1] == '>')
			token = init_redir(input, &i, TOK_APP, envp);

			
		else if (input[i] != ' ')
		{
			token = init_token_word(input, &i, envp); // echo )
		}
		if (input[i] != ' ')
			add_token(&token_lst, token);
	}
	return (token_lst);
}
