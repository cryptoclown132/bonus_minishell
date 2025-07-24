#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

typedef enum {
    NODE_EXEC,
    NODE_PIPE,
    NODE_AND,
    NODE_OR,
    NODE_SUBSHELL
} NodeType;

typedef struct Node {
    NodeType type;
	
	int				infile;
	int				outfile;
	char			*err_file;
	int				err;
	//int				prev;
	char			**env;
	char			**var_lst;

    union {
        struct {
                char    **cmd_split;
	            char    *cmd_path;
        } exec;

        struct {
            struct Node *left;
            struct Node *right;
        } pipe;

        struct {
            struct Node *left;
            struct Node *right;
        } and;

        struct {
            struct Node *left;
            struct Node *right;
        } or;

        struct {
            struct Node *child;
        } subshell;
    };
} Node;


// // Recursive execution
// int execute_node(Node *node) {
//     if (!node) return -1;

//     if (node->type == NODE_EXEC) {

//         printf("executing: %s %s\n", node->exec.cmd_split[0], node->exec.cmd_split[1]);
//         execvp(node->exec.cmd_split[0], node->exec.cmd_split);
//         perror("execvp");
//         exit(1);
//     }

//     else if (node->type == NODE_SUBSHELL) {
//         pid_t pid = fork();
//         if (pid == 0) {
//             // Subshell context: run the child subtree in a new process
//             exit(execute_node(node->subshell.child));
//         } else {
//             int status;
//             waitpid(pid, &status, 0);
//             return WEXITSTATUS(status);
//         }
//     }

//     else if (node->type == NODE_PIPE) {
//         int pipefd[2];
//         if (pipe(pipefd) == -1) {
//             perror("pipe");
//             exit(1);
//         }

//         pid_t left_pid = fork();
//         if (left_pid == 0) {
//             // Left side of pipe
//             dup2(pipefd[1], STDOUT_FILENO);
//             close(pipefd[0]);
//             close(pipefd[1]);
//             exit(execute_node(node->pipe.left));
//         }

//         pid_t right_pid = fork();
//         if (right_pid == 0) {
//             // Right side of pipe
//             dup2(pipefd[0], STDIN_FILENO);
//             close(pipefd[1]);
//             close(pipefd[0]);
//             exit(execute_node(node->pipe.right));
//         }

//         // Parent closes pipe fds and waits for both
//         close(pipefd[0]);
//         close(pipefd[1]);

//         int status;
//         waitpid(left_pid, &status, 0);
//         waitpid(right_pid, &status, 0);
//         return WEXITSTATUS(status);
//     }

//     else if (node->type == NODE_AND) {
//         printf("executing: &&iii \n");
//         // Logical AND: only execute right if left succeeds
//         int status = execute_node(node->and.left);
//         if (status == 0) {
//             printf("left succeeded\n");
//             status = execute_node(node->and.right);
//         }
//         return status;
//     }

//     else if (node->type == NODE_OR) {
//         // Logical OR: only execute right if left fails
//         int status = execute_node(node->or.left);
//         if (status != 0) {
//             status = execute_node(node->or.right);
//         }
//         return status;
//     }

//     return -1;
// }


// Recursive execution
int execute_node(Node *node) {
    if (!node) return -1;

    // —— EXEC NODE ——
    if (node->type == NODE_EXEC) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            // child: actually run the command
            // printf("execvp: %s %s\n",
            //        node->exec.cmd_split[0],
            //        node->exec.cmd_split[1] ? node->exec.cmd_split[1] : "");
            execvp(node->exec.cmd_split[0], node->exec.cmd_split);
            perror("execvp");
            exit(1);
        }
        // parent: wait and return the child's exit code
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }

    // —— SUBSHELL NODE ——
    else if (node->type == NODE_SUBSHELL) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            // in subshell: run subtree and exit
            exit(execute_node(node->subshell.child));
        } else {
            int status;
            waitpid(pid, &status, 0);
            return WEXITSTATUS(status);
        }
    }

    // —— PIPE NODE ——
    else if (node->type == NODE_PIPE) {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(1);
        }

        pid_t left_pid = fork();
        if (left_pid == 0) {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]); close(pipefd[1]);
            exit(execute_node(node->pipe.left));
        }

        pid_t right_pid = fork();
        if (right_pid == 0) {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[1]); close(pipefd[0]);
            exit(execute_node(node->pipe.right));
        }

        close(pipefd[0]); close(pipefd[1]);
        int status;
        waitpid(left_pid, &status, 0);
        waitpid(right_pid, &status, 0);
        return WEXITSTATUS(status);
    }

    // —— AND NODE (&&) ——
    else if (node->type == NODE_AND) {
        // printf("AND: evaluating left side\n");
        int status = execute_node(node->and.left);
        if (status == 0) {
            // printf("AND: left succeeded, evaluating right side\n");
            status = execute_node(node->and.right);
        } else {
            // printf("AND: left failed (%d), skipping right side\n", status);
        }
        return status;
    }

    // —— OR NODE (||) ——
    else if (node->type == NODE_OR) {
        // printf("OR: evaluating left side\n");
        int status = execute_node(node->or.left);
        if (status != 0) {
            // printf("OR: left failed (%d), evaluating right side\n", status);
            status = execute_node(node->or.right);
        } else {
            // printf("OR: left succeeded, skipping right side\n");
        }
        return status;
    }

    return -1;
}


// // Build the AST: ls | cat | grep text
// int main() {
//     char *ls_argv[] = {"ls", NULL};
//     char *cat_argv[] = {"ls", NULL};
//     char *grep_argv[] = {"grep", "test.c", NULL};

//     Node node_ls = {.type = NODE_EXEC, .exec = {.argv = ls_argv}};
//     Node node_cat = {.type = NODE_EXEC, .exec = {.argv = cat_argv}};
//     Node node_grep = {.type = NODE_EXEC, .exec = {.argv = grep_argv}};

//     // ls | cat
//     Node pipe1 = {.type = NODE_PIPE, .pipe = {.left = &node_ls, .right = &node_cat}};

//     // (ls | cat) | grep
//     Node pipe2 = {.type = NODE_PIPE, .pipe = {.left = &pipe1, .right = &node_grep}};

//     return execute_node(&pipe2);
// }

int main() {
    // 1) leaf ExecNodes
    char *ls_argv[]    = { "ldfdss", "-a",   NULL };
    char *echo_argv[]  = { "head", "main.c", NULL };
    char *grep_argv[]  = { "grep", "Created", NULL };

    Node node_ls    = { .type = NODE_EXEC,     .exec   = { .cmd_split = ls_argv   } };
    Node node_echo  = { .type = NODE_EXEC,     .exec   = { .cmd_split = echo_argv } };
    Node node_grep  = { .type = NODE_EXEC,     .exec   = { .cmd_split = grep_argv } };

    // 2) build the AND subtree:  ls -a && echo hello
    Node and1 = {
        .type = NODE_OR,
        .and  = { .left = &node_ls,  .right = &node_echo }
    };

    // 3) wrap it in a subshell:  ( ls -a && echo hello )
    Node sub1 = {
        .type     = NODE_SUBSHELL,
        .subshell = { .child = &and1 }
    };

    // 4) pipe the subshell into grep hello
    Node pipe1 = {
        .type = NODE_PIPE,
        .pipe = { .left = &sub1,     .right = &node_grep }
    };

    // 5) execute the whole tree
    return execute_node(&pipe1);
}























//**new */
int main(void) {
    int status;

    // 1) AND-Kette: echo foo && echo bar
    char *argv1[] = { "echo", "foo", NULL };
    cmd_tree *e1 = new_exec_node("/usr/bin/echo", argv1,  STDIN_FILENO, STDOUT_FILENO);
    char *argv2[] = { "echo", "bar", NULL };
    cmd_tree *e2 = new_exec_node("/usr/bin/echo", argv2,  STDIN_FILENO, STDOUT_FILENO);
    cmd_tree *and_tree = new_and_node(e1, e2);
    status = execute_node(and_tree, true);
    printf("Status echo foo && echo bar: %d\n", status);

    // 2) OR-Kette: false || echo fallback
    char *argv_false[] = { "false", NULL };
    cmd_tree *false_node = new_exec_node("/usr/bin/false", argv_false, STDIN_FILENO, STDOUT_FILENO);
    char *argv_fall[]  = { "echo", "fallback", NULL };
    cmd_tree *echo_fall = new_exec_node("/usr/bin/echo", argv_fall, STDIN_FILENO, STDOUT_FILENO);
    cmd_tree *or_tree   = new_or_node(false_node, echo_fall);
    status = execute_node(or_tree, true);
    printf("Status false || echo fallback: %d\n", status);

    // 3) Subshell: (cd .. && ls) | grep minishell.c
    //   3a) Sub-Baum cd .. && ls
    char *argv_cd[]  = { "cd", "..", NULL };
    cmd_tree *cd_n   = new_exec_node("/bin/ls", argv_cd, STDIN_FILENO, STDOUT_FILENO);

	char *argv_cd2[]  = { "cd", "new_creations", NULL };
    cmd_tree *cd_n2   = new_exec_node("/bin/ls", argv_cd2, STDIN_FILENO, STDOUT_FILENO);

    char *argv_ls[]  = { "ls", NULL, NULL };
    cmd_tree *ls_n   = new_exec_node("/bin/ls", argv_ls, STDIN_FILENO, STDOUT_FILENO);
    
	cmd_tree *inner   = new_and_node(cd_n, cd_n2);

	cmd_tree *life = new_and_node(inner, ls_n);
	cmd_tree *damso = new_and_node(life, ls_n);
    cmd_tree *subsh   = new_subshell_node(damso);


    //   3b) grep-Knoten
    char *argv_grep[] = { "grep", "main.c", NULL };
    cmd_tree *grep_n  = new_exec_node("/usr/bin/grep", argv_grep, STDIN_FILENO, STDOUT_FILENO);

    //   3c) ganze Pipeline
    cmd_tree *pipe_tree = new_pipe_node(subsh, grep_n);
    status = execute_node(pipe_tree, true);
    printf("Status (cd..&&ls)|grep: %d\n", status);

    // Aufräumen …
    // free(e1); free(e2); free(and_tree); … etc.

    return 0;
}



//  // 1) leaf ExecNodes
//     char *ls_argv[]    = { "ldfdss", "-a",   NULL };
//     char *echo_argv[]  = { "head", "main.c", NULL };
//     char *grep_argv[]  = { "grep", "Created", NULL };

//     Node node_ls    = { .type = NODE_EXEC,     .exec   = { .cmd_split = ls_argv   } };
//     Node node_echo  = { .type = NODE_EXEC,     .exec   = { .cmd_split = echo_argv } };
//     Node node_grep  = { .type = NODE_EXEC,     .exec   = { .cmd_split = grep_argv } };

//     // 2) build the AND subtree:  ls -a && echo hello
//     Node and1 = {
//         .type = NODE_OR,
//         .and  = { .left = &node_ls,  .right = &node_echo }
//     };

//     // 3) wrap it in a subshell:  ( ls -a && echo hello )
//     Node sub1 = {
//         .type     = NODE_SUBSHELL,
//         .subshell = { .child = &and1 }
//     };

//     // 4) pipe the subshell into grep hello
//     Node pipe1 = {
//         .type = NODE_PIPE,
//         .pipe = { .left = &sub1,     .right = &node_grep }
//     };

//     // 5) execute the whole tree
//     return execute_node(&pipe1);