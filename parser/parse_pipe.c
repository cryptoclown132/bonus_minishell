
#include "../minishell.h"

cmd_tree *parse_pipeline(t_tokens **token_lst, env_var environ)
{
	// cmd_tree *left = parse_redirection(token_lst, environ);
	cmd_tree *left = parse_cmd_sequence(token_lst, environ);
	while (*token_lst && (*token_lst)->type == TOK_PIPE)
	{
		*token_lst = (*token_lst)->next;

		// cmd_tree *right = parse_redirection(token_lst, environ);
		cmd_tree *right = parse_cmd_sequence(token_lst, environ);
		cmd_tree *n = new_node(NODE_PIPE);
		n->pipe.left  = left;
		n->pipe.right = right;
		left = n;
	}
	return left;
}
