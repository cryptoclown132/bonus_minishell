
#include "minishell.h"

int	handle_builtin(cmd_tree *cmd_lst, bool in_parent)
{
	if (cmd_lst->err != 0)
	{
		set_err(cmd_lst->err_file, cmd_lst->err);
		return 1;
	}
	run_builtin(cmd_lst);
	if (in_parent)
		return 0;
	exit(g_exit_status);
}

void	redir_files(cmd_tree *cmd_lst)
{
	if (cmd_lst->infile != -1)
	{
		printf("hereY\n");
        dup2(cmd_lst->infile, STDIN_FILENO);
        close(cmd_lst->infile);
    }
    if (cmd_lst->outfile != -1)
	{
		printf("also here\n");
        dup2(cmd_lst->outfile, STDOUT_FILENO);
        close(cmd_lst->outfile);
    }
}

int exec_cmd(cmd_tree *cmd_lst, bool in_parent)
{
	if (in_parent && run_builtin(cmd_lst))
	{
		printf("first if\n");
		return (0);
	}

	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(1);
	}
	// pid_t pid = 0;
	if (pid == 0) {
		redir_files(cmd_lst);
		if (run_builtin2(cmd_lst)) //|| (!cmd_lst->exec.cmd_path && cmd_lst->err == 0)
			exit(g_exit_status);
		if (run_builtin(cmd_lst))
			handle_builtin(cmd_lst, in_parent);
		execve(cmd_lst->exec.cmd_path, cmd_lst->exec.cmd_split, cmd_lst->env);
		set_exit_status("Error: command not found", 127); // bash: dsf: command not found
		exit(1);
	}
	// parent: wait and return the child's exit code
	int status;
	waitpid(pid, &status, 0);
	return WEXITSTATUS(status);
}
