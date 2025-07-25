
#include "minishell.h"

int exec_and(cmd_tree *cmd_lst)
{
    int status = execute_node(cmd_lst->and.left, true);
    if (status == 0) {
        status = execute_node(cmd_lst->and.right, true);
    } else {
        // printf("AND: left failed (%d), skipping right side\n", status);
    }
    return status;
}

int exec_or(cmd_tree *cmd_lst)
{
    int status = execute_node(cmd_lst->or.left, true);

    if (status != 0) {
        status = execute_node(cmd_lst->or.right, true);
    } else {
        // printf("OR: left succeeded, skipping right side\n");
    }
    return status;
}






// int execute_node(Node *node) {
//     if (!node) return -1;

//     // —— EXEC NODE ——
//     if (node->type == NODE_EXEC) {
//         pid_t pid = fork();
//         if (pid < 0) {
//             perror("fork");
//             exit(1);
//         }
//         if (pid == 0) {
//             // child: actually run the command
//             // printf("execvp: %s %s\n",
//             //        node->exec.cmd_split[0],
//             //        node->exec.cmd_split[1] ? node->exec.cmd_split[1] : "");
//             execvp(node->exec.cmd_split[0], node->exec.cmd_split);
//             perror("execvp");
//             exit(1);
//         }
//         // parent: wait and return the child's exit code
//         int status;
//         waitpid(pid, &status, 0);
//         return WEXITSTATUS(status);
//     }

//     // —— SUBSHELL NODE ——
//     else if (node->type == NODE_SUBSHELL) {
//         pid_t pid = fork();
//         if (pid < 0) {
//             perror("fork");
//             exit(1);
//         }
//         if (pid == 0) {
//             // in subshell: run subtree and exit
//             exit(execute_node(node->subshell.child));
//         } else {
//             int status;
//             waitpid(pid, &status, 0);
//             return WEXITSTATUS(status);
//         }
//     }

//     // —— PIPE NODE ——
//     else if (node->type == NODE_PIPE) {
//         int pipefd[2];
//         if (pipe(pipefd) == -1) {
//             perror("pipe");
//             exit(1);
//         }

//         pid_t left_pid = fork();
//         if (left_pid == 0) {
//             dup2(pipefd[1], STDOUT_FILENO);
//             close(pipefd[0]); close(pipefd[1]);
//             exit(execute_node(node->pipe.left));
//         }

//         pid_t right_pid = fork();
//         if (right_pid == 0) {
//             dup2(pipefd[0], STDIN_FILENO);
//             close(pipefd[1]); close(pipefd[0]);
//             exit(execute_node(node->pipe.right));
//         }

//         close(pipefd[0]); close(pipefd[1]);
//         int status;
//         waitpid(left_pid, &status, 0);
//         waitpid(right_pid, &status, 0);
//         return WEXITSTATUS(status);
//     }

//     // —— AND NODE (&&) ——
//     else if (node->type == NODE_AND) {
//         // printf("AND: evaluating left side\n");
//         int status = execute_node(node->and.left);
//         if (status == 0) {
//             // printf("AND: left succeeded, evaluating right side\n");
//             status = execute_node(node->and.right);
//         } else {
//             // printf("AND: left failed (%d), skipping right side\n", status);
//         }
//         return status;
//     }

//     // —— OR NODE (||) ——
//     else if (node->type == NODE_OR) {
//         // printf("OR: evaluating left side\n");
//         int status = execute_node(node->or.left);
//         if (status != 0) {
//             // printf("OR: left failed (%d), evaluating right side\n", status);
//             status = execute_node(node->or.right);
//         } else {
//             // printf("OR: left succeeded, skipping right side\n");
//         }
//         return status;
//     }

//     return -1;
// }