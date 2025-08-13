
#include "minishell.h"

int exec_subshell(cmd_tree *cmd_lst, env_var *environ)
{
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(1);
	}
	if (pid == 0) {

	// in subshell: run subtree and exit
		exit(execute_node(cmd_lst->subshell.child, true, environ));	
	}
	int status;
	waitpid(pid, &status, 0);
	return WEXITSTATUS(status);
}
