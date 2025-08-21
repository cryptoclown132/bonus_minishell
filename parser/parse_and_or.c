
#include "../minishell.h"

cmd_tree	*parse_and(t_tokens **token_lst, env_var environ)
{
	cmd_tree	*left;
	cmd_tree	*right;
	cmd_tree	*n;

	left = parse_pipeline(token_lst, environ);
	while (*token_lst && (*token_lst)->type == TOK_AND)
	{
		*token_lst = (*token_lst)->next;
		right = parse_pipeline(token_lst, environ);
		n = new_node(NODE_AND);
		n->and.left = left;
		n->and.right = right;
		left = n;
	}
	return (left);
}

cmd_tree	*parse_or(t_tokens **token_lst, env_var environ)
{
	cmd_tree	*left;
	cmd_tree	*right;
	cmd_tree	*n;

	left = parse_and(token_lst, environ);
	while (*token_lst && (*token_lst)->type == TOK_OR)
	{
		*token_lst = (*token_lst)->next;
		right = parse_and(token_lst, environ);
		n = new_node(NODE_OR);
		n->or.left = left;
		n->or.right = right;
		left = n;
	}
	return (left);
}
