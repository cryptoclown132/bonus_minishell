
#include "../minishell.h"

int	count_args(t_tokens *token_lst)
{
	int			i;
	t_tokens	*tmp;

	i = 0;
	tmp = token_lst;
	while (tmp && (tmp->type == TOK_WORD || tmp->type == TOK_EQUAL))
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**get_cmd_split(t_tokens **token_lst)
{
	char**	new_cmd_split;
	int		i;
	int		j;

	i = count_args(*token_lst);
	new_cmd_split = ft_calloc(i + 1, sizeof(char*));
	if (!new_cmd_split)
			return (set_exit_status("Failed to Malloc", 1));
	j = -1;
	while (++j < i)  //*token_lst != NULL && ((*token_lst)->type == TOK_WORD || (*token_lst)->type == TOK_EQUAL)
	{
		new_cmd_split[j] = ft_strdup((*token_lst)->token); //check later
		*token_lst = (*token_lst)->next;
	}
	new_cmd_split[j] = NULL;
	return new_cmd_split;
}

cmd_tree *parse_command(t_tokens **token_lst, env_var environ)
{
	cmd_tree	*cmd_node;
	cmd_tree	*child;

    if (*token_lst && (*token_lst)->type == TOK_LPAREN) {
		*token_lst = (*token_lst)->next;

		child = parse_or(token_lst, environ);

		*token_lst = (*token_lst)->next;

		cmd_node = new_node(NODE_SUBSHELL);
		cmd_node->subshell.child = child;
		return cmd_node;
	}
	
	// printf("type : %i\n", (*token_lst)->type);
	if ((*token_lst)->type != TOK_WORD && (*token_lst)->type != TOK_EQUAL) {
		fprintf(stderr, "Parse error: expected word or '('\n");
		exit(1);
	}
	cmd_node = new_node(NODE_EXEC);
	
	cmd_node->exec.cmd_split = get_cmd_split(token_lst);
	cmd_node->exec.cmd_path = get_cmd_path(environ,
		cmd_node->exec.cmd_split[0], cmd_node);	
	return cmd_node;
}


// cmd_tree *parse_command(t_tokens **token_lst) {
	
//     if ((*token_lst)->type == TOK_LPAREN) {
// 		*token_lst = (*token_lst)->next;

// 		cmd_tree *child = parse_or(token_lst);
// 		cmd_tree *n = new_node(NODE_SUBSHELL);
// 		n->subshell.child = child;
// 		return n;
// 	}
// 	if ((*token_lst)->type != TOK_WORD) {
// 		fprintf(stderr, "Parse error: expected word or '('\n");
// 		exit(1);
// 	}
// 	cmd_tree *n = new_node(NODE_EXEC);
// 	int cap = 4, argc = 0;
// 	n->exec.cmd_split = malloc(cap * sizeof(char*));
// 	while ((*token_lst)->type == TOK_WORD) {
// 		if (argc + 1 >= cap)
// 			n->exec.cmd_split = realloc(n->exec.cmd_split, (cap *= 2) * sizeof(char*));
// 		n->exec.cmd_split[argc++] = strdup(consume()->text);
// 	}
// 	n->exec.cmd_split[argc] = NULL;
// 	return n;
// }



