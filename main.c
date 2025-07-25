
#include "minishell.h"

int	g_exit_status;


// Recursive execution
int execute_node(cmd_tree *cmd_lst, bool in_parent) {
	if (!cmd_lst) 
		return -1;

    // printf("type shit : %i\n", cmd_lst->type);

	if (cmd_lst->type == NODE_EXEC)
    {

        // printf("in exec nodesdfsgsf\n");
		return exec_cmd(cmd_lst, in_parent);
    }

	else if (cmd_lst->type == NODE_SUBSHELL){
		// printf(" if in subshell node!!!!!!!!!!!!!!!!!!!\n");

		return exec_subshell(cmd_lst);
	}

	else if (cmd_lst->type == NODE_PIPE)
		return exec_pipe(cmd_lst);

	else if (cmd_lst->type == NODE_AND)
	   return exec_and(cmd_lst);

	else if (cmd_lst->type == NODE_OR)
    {
        // printf("in ord node??????????\n");
	    return exec_or(cmd_lst);
    }

	return -1;
}












int main(int argc, char *argv[], char **envp)
{
    cmd_tree	*cmd_lst;
    char        **env;

    cmd_lst = NULL;
	if (argc > 1)
		return (0);
	(void)argv;
	g_exit_status = 0;    
    
    env = copy_env(envp);
	while (1)
		minishell(cmd_lst, &env);

    //free(env)
}


// int main(int argc, char *argv[], char **envp)
// {
//     cmd_tree	*cmd_lst;

// 	if (argc > 1)
// 		return (0);
// 	(void)argv;
// 	g_exit_status = 0;
// 	cmd_lst = cmd_struct(envp, 0);
    
// 	while (1)
// 		minishell(cmd_lst);
// }