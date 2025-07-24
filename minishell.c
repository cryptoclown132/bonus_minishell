
#include "minishell.h"

int	minishell(cmd_tree *cmd_lst, char ***env)
{
	char	*input;

	get_signals();
	// g_exit_status = 0;
    cmd_lst->env = copy_env(*env);
	input = user_input();
	if (!input)
	{
		printf("exit\n");
		g_exit_status = 130;
		free_env(cmd_lst);
		exit(g_exit_status);
	}
	cmd_lst = parse(cmd_lst, &input, cmd_lst->env);

	add_history(input);
	free(input);

	if (cmd_lst->err == -2 || g_exit_status == 130)
	{
		// free_cmd_lst(cmd_lst);
		return (0);
	}
	var_lst(cmd_lst);
	


	run_cmds_loop(cmd_lst);
	free_cmd_lst(cmd_lst);
	return (0);
}
