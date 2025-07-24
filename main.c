
#include "minishell.h"

int	g_exit_status;


// Recursive execution
int execute_node(cmd_tree *cmd_lst, bool in_parent) {
	if (!cmd_lst) 
		return -1;

	if (cmd_lst->type == NODE_EXEC)
		return exec_cmd(cmd_lst, in_parent);

	else if (cmd_lst->type == NODE_SUBSHELL){
		// printf(" if in subshell node!!!!!!!!!!!!!!!!!!!\n");

		return exec_subshell(cmd_lst);
	}

	else if (cmd_lst->type == NODE_PIPE)
		return exec_pipe(cmd_lst);

	else if (cmd_lst->type == NODE_AND)
	   return exec_and(cmd_lst);

	else if (cmd_lst->type == NODE_OR)
	   return exec_or(cmd_lst);

	return -1;
}



#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

// extern-Declaration Deiner Umgebungsvariablen
extern char **environ;  //change


// Exec-Knoten
cmd_tree *new_exec_node(char *path, char *argv[], int infile, int outfile) {
    cmd_tree *n = calloc(1, sizeof(cmd_tree));
    n->type            = NODE_EXEC;
    n->infile          = infile;
    n->outfile         = outfile;
    n->err_file        = NULL;
    n->err             = 0;
    n->env             = environ;
    n->var_lst         = NULL;
    n->exec.cmd_split  = argv;
    n->exec.cmd_path   = path;
    return n;
}

// Pipe-Knoten
cmd_tree *new_pipe_node(cmd_tree *l, cmd_tree *r) {
    cmd_tree *n = calloc(1, sizeof(cmd_tree));
    n->type       = NODE_PIPE;
    n->infile     = STDIN_FILENO;
    n->outfile    = STDOUT_FILENO;
    n->err_file   = NULL;
    n->err        = 0;
    n->env        = environ;
    n->var_lst    = NULL;
    n->pipe.left  = l;
    n->pipe.right = r;
    return n;
}

// AND-Knoten (&&)
cmd_tree *new_and_node(cmd_tree *l, cmd_tree *r) {
    cmd_tree *n = calloc(1, sizeof(cmd_tree));
    n->type      = NODE_AND;
    n->infile    = STDIN_FILENO;
    n->outfile   = STDOUT_FILENO;
    n->err_file  = NULL;
    n->err       = 0;
    n->env       = environ;
    n->var_lst   = NULL;
    n->and.left  = l;
    n->and.right = r;
    return n;
}

// OR-Knoten (||)
cmd_tree *new_or_node(cmd_tree *l, cmd_tree *r) {
    cmd_tree *n = calloc(1, sizeof(cmd_tree));
    n->type     = NODE_OR;
    n->infile   = STDIN_FILENO;
    n->outfile  = STDOUT_FILENO;
    n->err_file = NULL;
    n->err      = 0;
    n->env      = environ;
    n->var_lst  = NULL;
    n->or.left  = l;
    n->or.right = r;
    return n;
}

// Subshell-Knoten (…)
cmd_tree *new_subshell_node(cmd_tree *c) {
    cmd_tree *n = calloc(1, sizeof(cmd_tree));
    n->type           = NODE_SUBSHELL;
    n->infile         = STDIN_FILENO;
    n->outfile        = STDOUT_FILENO;
    n->err_file       = NULL;
    n->err            = 0;
    n->env            = environ;
    n->var_lst        = NULL;
    n->subshell.child = c;
    return n;
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
    
    // cmd_lst = cmd_struct(envp, 0);
    
    env = copy_env(envp);
	while (1)
		minishell(cmd_lst, &env);
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