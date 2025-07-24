
#include "../minishell.h"

cmd_tree *parse_pipeline(t_tokens **token_lst)
{
	cmd_tree *left = parse_redirection(token_lst);
	while (*token_lst && (*token_lst)->type == TOK_PIPE)
	{
		*token_lst = (*token_lst)->next;

		cmd_tree *right = parse_redirection(token_lst);
		cmd_tree *n = new_node(NODE_PIPE);
		n->pipe.left  = left;
		n->pipe.right = right;
		left = n;
	}
	return left;
}
