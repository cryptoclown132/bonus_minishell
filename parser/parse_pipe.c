
#include "../minishell.h"

cmd_tree *parse_pipeline(t_tokens **token_lst, char **envp)
{
	cmd_tree *left = parse_redirection(token_lst, envp);
	while (*token_lst && (*token_lst)->type == TOK_PIPE)
	{
		*token_lst = (*token_lst)->next;

		cmd_tree *right = parse_redirection(token_lst, envp);
		cmd_tree *n = new_node(NODE_PIPE, envp);
		n->pipe.left  = left;
		n->pipe.right = right;
		left = n;
	}
	return left;
}
