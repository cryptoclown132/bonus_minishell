
#include "minishell.h"


#include <stdio.h>

static void print_indent(int depth) {
    for (int i = 0; i < depth; ++i) putchar(' ');
}

static const char *node_type_str(node_type t) {
    switch (t) {
        case NODE_EXEC:     return "EXEC";
        case NODE_PIPE:     return "PIPE";
        case NODE_AND:      return "AND";
        case NODE_OR:       return "OR";
        case NODE_SUBSHELL: return "SUBSHELL";
        default:            return "UNKNOWN";
    }
}

static void print_str_array(char **arr) {
    if (!arr) {
        printf("NULL");
        return;
    }
    putchar('[');
    for (int i = 0; arr[i]; ++i) {
        printf("\"%s\"", arr[i]);
        if (arr[i+1]) printf(", ");
    }
    putchar(']');
}

void print_cmd_tree(cmd_tree *node, int depth) {
    if (!node) {
        print_indent(depth);
        printf("NULL node\n");
        return;
    }

    print_indent(depth);
    printf("Node %p type=%s infile=%d outfile=%d err_file=%s err=%d\n",
           (void*)node, node_type_str(node->type),
           node->infile, node->outfile,
           node->err_file ? node->err_file : "NULL",
           node->err);

    print_indent(depth+2);
    printf("env = "); print_str_array(node->env); printf("\n");
    print_indent(depth+2);
    printf("var_lst = "); print_str_array(node->var_lst); printf("\n");

    switch (node->type) {
        case NODE_EXEC:
            print_indent(depth+2);
            printf("cmd_split = "); print_str_array(node->exec.cmd_split); printf("\n");
            print_indent(depth+2);
            printf("cmd_path = %s\n", node->exec.cmd_path ? node->exec.cmd_path : "NULL");
            break;
        case NODE_PIPE:
            print_indent(depth+2); printf("left:\n");
            print_cmd_tree(node->pipe.left, depth+4);
            print_indent(depth+2); printf("right:\n");
            print_cmd_tree(node->pipe.right, depth+4);
            break;
        case NODE_AND:
            print_indent(depth+2); printf("left:\n");
            print_cmd_tree(node->and.left, depth+4);
            print_indent(depth+2); printf("right:\n");
            print_cmd_tree(node->and.right, depth+4);
            break;
        case NODE_OR:
            print_indent(depth+2); printf("left:\n");
            print_cmd_tree(node->or.left, depth+4);
            print_indent(depth+2); printf("right:\n");
            print_cmd_tree(node->or.right, depth+4);
            break;
        case NODE_SUBSHELL:
            print_indent(depth+2); printf("child:\n");
            print_cmd_tree(node->subshell.child, depth+4);
            break;
        default:
            print_indent(depth+2); printf("(unhandled node type)\n");
    }
}



int	minishell(cmd_tree *cmd_lst, char ***env)
{
	char	*input;

	get_signals();
	// g_exit_status = 0;

    // cmd_lst->env = copy_env(*env);


	input = user_input();
	if (!input)
	{
        printf("exit\n");
		g_exit_status = 130;
		free_env(cmd_lst);
		exit(g_exit_status);
	}
	cmd_lst = parse(cmd_lst, &input, *env);
    
	add_history(input);
	free(input);
	if (g_exit_status == 130 || !cmd_lst || cmd_lst->err == -2)
	{
        // free_cmd_lst(cmd_lst);
		return (0);
	}
	var_lst(cmd_lst);
	// print_cmd_tree(cmd_lst, 5);
	execute_node(cmd_lst, true);
    // printf("here\n");
    
	// run_cmds_loop(cmd_lst);
	
	// free(env)
	// *env =  copy_env(cmd_lst->env)
	// free_cmd_lst(cmd_lst);
	return (0);
}
