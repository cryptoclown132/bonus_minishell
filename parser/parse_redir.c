
#include "../minishell.h"

int	input_redir(char *file, int infile, cmd_tree *cmd_node)
{
	if (infile != 0)
		close(infile);
	infile = open(file, O_RDONLY);
	if (infile == -1)
	{
		cmd_node->err = errno;
		cmd_node->err_file = ft_strdup(file);
	}
	return (infile);
}

int	output_redir(char *file, int outfile, cmd_tree *cmd_node)
{
	if (outfile != 1)
		close(outfile);
	outfile = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
	{
		cmd_node->err = errno;
		cmd_node->err_file = ft_strdup(file);
	}
	return (outfile);
}

int	ft_append(char *file, int outfile, cmd_tree *cmd_node)
{
	if (outfile != 1)
		close(outfile);
	outfile = open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (outfile == -1)
	{
		cmd_node->err = errno;
		cmd_node->err_file = ft_strdup(file);
	}
	return (outfile);
}

void	here_doc(char* limiter, int *infile, env_var environ, cmd_tree *cmd_node)
{
	int		fd[2];

	if (*infile != 0 && *infile != -1)
		close(*infile);
	if (pipe(fd) == -1)
	{
		cmd_node->err = errno;
		cmd_node->err_file = ft_strdup(limiter);
		return ;
	}
	here_doc_loop(limiter, fd[1], environ);
	close(fd[1]);
	// if (*infile != -1)
	*infile = fd[0]; //check if needs to be closed
}

cmd_tree *parse_redirection(t_tokens **token_lst, env_var environ)
{
	cmd_tree	*cmd_node;
	char		*file;
	
	cmd_node = parse_command(token_lst, environ);
	while (*token_lst && ((*token_lst)->type == TOK_IN || (*token_lst)->type == TOK_OUT
	|| (*token_lst)->type == TOK_APP || (*token_lst)->type == TOK_DOC))
    {
		// if (f->type != TOK_WORD) {
			// 	fprintf(stderr, "Parse error: expected filename\n");   check << ()
			// 	exit(1);
			// }
		
		file = (*token_lst)->token;
		if ((*token_lst)->type == TOK_IN)
			cmd_node->infile = input_redir(file, cmd_node->infile, cmd_node);
		else if ((*token_lst)->type == TOK_OUT)
			cmd_node->outfile = output_redir(file, cmd_node->outfile, cmd_node);
		else if ((*token_lst)->type == TOK_DOC)
			here_doc(file, &cmd_node->infile, environ, cmd_node);
		else
			cmd_node->outfile = ft_append(file, cmd_node->outfile, cmd_node);
		
		*token_lst = (*token_lst)->next; // check later
	}
	return cmd_node;
}
