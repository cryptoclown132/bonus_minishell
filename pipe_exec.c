
#include "minishell.h"

void    redirect_parent(int *pipefd, cmd_tree *cmd_lst)
{
	if (cmd_lst->infile != 0 && cmd_lst->infile != -1)
	{
		close(pipefd[1]);
		dup2(cmd_lst->infile, STDIN_FILENO);
		close(cmd_lst->infile);
		close(pipefd[0]);
		exit(execute_node(cmd_lst->pipe.right, false));
	}
	else
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]); 
		close(pipefd[0]);
		exit(execute_node(cmd_lst->pipe.right, false));
	}
}

void	redirect_child(int *pipefd, cmd_tree *cmd_lst)
{
    if (cmd_lst->outfile != 1 && cmd_lst->outfile != -1)
    {
        close(pipefd[0]);
        dup2(cmd_lst->outfile, STDOUT_FILENO);
        close(cmd_lst->outfile);
        close(pipefd[1]);
        exit(execute_node(cmd_lst->pipe.left, false));
    }
    else
    {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]); 
        close(pipefd[1]);
        exit(execute_node(cmd_lst->pipe.left, false));
    }
}

int exec_pipe(cmd_tree *cmd_lst)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t left_pid = fork();
    if (left_pid == 0)
        redirect_child(pipefd, cmd_lst);

    pid_t right_pid = fork();
    if (right_pid == 0)
        redirect_parent(pipefd, cmd_lst);

    close(pipefd[0]);
    close(pipefd[1]);
    int status;
    waitpid(left_pid, &status, 0);
    waitpid(right_pid, &status, 0);
    return WEXITSTATUS(status);
}
