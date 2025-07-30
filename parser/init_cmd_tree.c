/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 11:48:59 by jkroger           #+#    #+#             */
/*   Updated: 2025/07/29 17:09:58 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_string_array(char **arr)
{
    int	i;

	if (!arr) 
		return;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}


void	close_file(cmd_tree *node)
{
	if (node->infile  >  0) 
		close(node->infile);
	if (node->outfile >  1)
		close(node->outfile);
	if (node->err_file)
		free(node->err_file);
}

void free_cmd_tree(cmd_tree *node)
{
    if (!node)
        return;

    switch (node->type) {
        case NODE_PIPE:
            free_cmd_tree(node->pipe.left);
            free_cmd_tree(node->pipe.right);
            break;
        case NODE_AND:
            free_cmd_tree(node->and.left);
            free_cmd_tree(node->and.right);
            break;
        case NODE_OR:
            free_cmd_tree(node->or.left);
            free_cmd_tree(node->or.right);
            break;
        case NODE_SUBSHELL:
            free_cmd_tree(node->subshell.child);
            break;
        case NODE_EXEC:
            break;
    }

	close_file(node);
    if (node->type == NODE_EXEC) {
        free_string_array(node->exec.cmd_split);
        if (node->exec.cmd_path)
            free(node->exec.cmd_path);
    }

    free_string_array(node->env);
    free_string_array(node->var_lst);

    free(node);
}

int	init_cmd_tree(t_tokens **token_lst, cmd_tree **cmd_lst, char **envp) // , char **envp
{
    t_tokens    *curr_token_lst;
    
    curr_token_lst = *token_lst;
    *cmd_lst = parse_or(&curr_token_lst, envp);    

    
	// while (*token_lst != NULL && g_exit_status != 130)
	// {
        
	// 	if ((*token_lst) != NULL && g_exit_status != 130)
	// 	{
	// 		t_tokens *tmp_t = (*token_lst)->next;
	// 		free((*token_lst)->token);
	// 		free(*token_lst);
	// 		*token_lst = tmp_t;
	// 	}
	// }
	if (g_exit_status == 130)
		return (0);
	return (1);
}

