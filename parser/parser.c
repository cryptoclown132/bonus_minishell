
#include "../minishell.h"

cmd_tree	*new_node(node_type t)
{
	cmd_tree *n;

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
		//check errors
		input = ft_strjoin(input, tmp_input); //check mem leaks
		if (!lex_error_check(tmp_input)) 
		{
			free(tmp_input);
			tmp_input = NULL;
			return (input); // set err code
		}
		// free input

		free(tmp_input);
		tmp_input = NULL;

		trim_input = ft_strtrim(input, " \t\n");
		if((trim_input[ft_strlen(trim_input) - 1] == '&' 
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

const char *token_type_to_str(token_type type)
{
	switch (type)
	{
		case TOK_WORD:   return "TOK_WORD";
		case TOK_PIPE:   return "TOK_PIPE (|)";
		case TOK_AND:    return "TOK_AND (&&)";
		case TOK_OR:     return "TOK_OR (||)";
		case TOK_LPAREN: return "TOK_LPAREN (()";
		case TOK_RPAREN: return "TOK_RPAREN ())";
		case TOK_IN:     return "TOK_IN (<)";
		case TOK_OUT:    return "TOK_OUT (>)";
		case TOK_APP:    return "TOK_APP (>>)";
		case TOK_DOC:    return "TOK_DOC (<<)";
		case TOK_EQUAL:  return "TOK_EQUAL (=)";
		case TOK_EOF:    return "TOK_EOF";
		default:         return "UNKNOWN";
	}
}


void print_tokens(t_tokens *head)
{
	int i = 0;
	while (head)
	{
		printf("Token %d:\n", i);
		printf("  type : %s\n", token_type_to_str(head->type));
		printf("  value: %s\n", head->token ? head->token : "(null)");
		head = head->next;
		i++;
	}
}

// char *expand_wildcar(char *input)
// {

// }

// char	*check_wildcard(char *input)
// {
// 	int		i;
// 	char	quote;

// 	i = -1;
// 	while (input[++i])
// 	{
// 		if (input[i] == '\'' || input[i] == '\"')
// 		{
// 			quote = input[i++];
// 			while (input[i] && quote == '\'' && quote == '\"')
// 				i++;
// 		}
// 		if (input[i] == '<' && input[i + 1] == '<')
// 		{
// 			while (input[i] && input[i] != ' ' && input[i] != '|'
// 				&& input[i] != '<' && input[i] != '>' && input[i] != '&')
// 			{
// 				if (input[i] == '\'' || input[i] == '\"')
// 				{
// 					quote = input[i++];
// 					while (input[i] && quote == '\'' && quote == '\"')
// 						i++;
// 				}
// 				i++;
// 			}
// 			if (input[i] == '<' && input[i + 1] == '<')
// 			{
// 				i--;
// 				continue;
// 			}
// 		}
// 		if (input[i] == '*')
// 			return (1);
// 	}
// 	return (0);
// }

cmd_tree	*parse(cmd_tree *cmd_lst, char **input, env_var environ)
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
	if (g_exit_status == 2 || g_exit_status == 130) // check mem leaks
	{
		if (cmd_lst)
			cmd_lst->err = -2;
		return (cmd_lst);
	}

		//handle error free etc.
	g_exit_status = prev_exit_status;

	token_lst = lexer(token_lst, *input, environ);	
	g_exit_status = 0;
	if (init_cmd_tree(&token_lst, &cmd_lst, environ) != 1)//, envp
		cmd_lst->err = -2;	
	//free(token_lst)
	return (cmd_lst);
}
