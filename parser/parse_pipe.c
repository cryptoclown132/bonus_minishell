
#include "../minishell.h"

cmd_tree	*parse_pipeline(t_tokens **token_lst, env_var environ)
{
	cmd_tree	*left;
	cmd_tree	*right;
	cmd_tree	*n;

	left = parse_cmd_sequence(token_lst, environ);
	while (*token_lst && (*token_lst)->type == TOK_PIPE)
	{
		*token_lst = (*token_lst)->next;
		right = parse_cmd_sequence(token_lst, environ);
		n = new_node(NODE_PIPE);
		n->pipe.left = left;
		n->pipe.right = right;
		left = n;
	}
	return (left);
}
