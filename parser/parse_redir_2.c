/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:11:57 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/27 13:59:08 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	input_redir(char *file, int infile, t_cmd_tree *cmd_node)
{
	if (infile != 0)
		close(infile);
	if (cmd_node->err_file)
		return (-1);
	infile = open(file, O_RDONLY);
	if (infile == -1)
	{
		cmd_node->err = errno;
		cmd_node->err_file = ft_strjoin(file, " ");
		cmd_node->err_file = edited_strjoin(cmd_node->err_file, strerror(errno));
		set_exit_status(cmd_node->err_file, 1);
	}
	return (infile);
}

int	output_redir(char *file, int outfile, t_cmd_tree *cmd_node)
{
	if (outfile != 1)
		close(outfile);
	if (cmd_node->err_file)
		return (-1);
	outfile = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
	{
		cmd_node->err = errno;
		cmd_node->err_file = ft_strjoin(file, " ");
		cmd_node->err_file = edited_strjoin(cmd_node->err_file, strerror(errno));
		set_exit_status(cmd_node->err_file, 1);
	}
	return (outfile);
}

int	ft_append(char *file, int outfile, t_cmd_tree *cmd_node)
{
	if (outfile != 1)
		close(outfile);
	if (cmd_node->err_file)
		return (-1);
	outfile = open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (outfile == -1)
	{
		cmd_node->err = errno;
		cmd_node->err_file = ft_strjoin(file, " ");
		cmd_node->err_file = edited_strjoin(cmd_node->err_file, strerror(errno));
		set_exit_status(cmd_node->err_file, 1);
	}
	return (outfile);
}

void	here_doc(char *limiter, int *infile,
	t_env_var environ, t_cmd_tree *cmd_node)
{
	int		fd[2];

	if (*infile != 0 && *infile != -1)
		close(*infile);
	if (pipe(fd) == -1)
	{
		cmd_node->err = errno;
		cmd_node->err_file = ft_strdup(limiter);
		set_exit_status(cmd_node->err_file, 1);
		return ;
	}
	here_doc_loop(limiter, fd[1], environ);
	close(fd[1]);
	*infile = fd[0];
}

int	is_directory(const char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == -1)
		return (-1);
	if (S_ISDIR(sb.st_mode))
		return (1);
	else
		return (0);
}

void	parse_redirection(t_tokens **token_lst,
		t_env_var environ, t_cmd_tree **cmd_node)
{
	char			*file;
	t_token_type	prev;

	prev = 100;
	while (*token_lst && ((*token_lst)->type == TOK_IN
			|| (*token_lst)->type == TOK_OUT || (*token_lst)->type == TOK_APP
			|| (*token_lst)->type == TOK_DOC || ((*token_lst)->type == TOK_WORD
				&& ((*cmd_node)->s_exec.cmd_path || ((*cmd_node)->s_exec.cmd_split)))))
	{
		file = (*token_lst)->token;
		if ((*token_lst)->type == TOK_IN || ((*token_lst)->type == TOK_WORD && prev == TOK_IN))
			(*cmd_node)->infile
				= input_redir(file, (*cmd_node)->infile, *cmd_node);
		else if ((*token_lst)->type == TOK_OUT || ((*token_lst)->type == TOK_WORD && prev == TOK_OUT))
			(*cmd_node)->outfile
				= output_redir(file, (*cmd_node)->outfile, *cmd_node);
		else if ((*token_lst)->type == TOK_DOC || ((*token_lst)->type == TOK_WORD && prev == TOK_DOC))
			here_doc(file, &(*cmd_node)->infile, environ, *cmd_node);
		else
			(*cmd_node)->outfile
				= ft_append(file, (*cmd_node)->outfile, *cmd_node);
		prev = (*token_lst)->type;
		*token_lst = (*token_lst)->next;
	}
}
