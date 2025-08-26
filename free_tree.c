/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:06:21 by julienkroge       #+#    #+#             */
/*   Updated: 2025/08/26 18:15:32 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_string_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

void	close_file(t_cmd_tree *node)
{
	if (node->infile > 0)
		close(node->infile);
	if (node->outfile > 1)
		close(node->outfile);
	if (node->err_file)
		free(node->err_file);
}

void	free_exec_node(t_cmd_tree *node)
{
	free_string_array(node->exec.cmd_split);
	if (node->exec.cmd_path)
		free(node->exec.cmd_path);
}

void	free_cmd_tree(t_cmd_tree *node)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		free_cmd_tree(node->pipe.left);
		free_cmd_tree(node->pipe.right);
	}
	else if (node->type == NODE_AND)
	{
		free_cmd_tree(node->and.left);
		free_cmd_tree(node->and.right);
	}
	else if (node->type == NODE_OR)
	{
		free_cmd_tree(node->or.left);
		free_cmd_tree(node->or.right);
	}
	else if (node->type == NODE_SUBSHELL)
		free_cmd_tree(node->subshell.child);
	close_file(node);
	if (node->type == NODE_EXEC)
		free_exec_node(node);
	free(node);
}
