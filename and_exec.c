
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
