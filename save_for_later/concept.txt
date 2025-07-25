





int run_commands(t_cmds **cmd_lst, int *prev_par_num)
{
    t_run_commands	r;
	int				tmp_log_op;

	r.wait_status = 0;
	r.pid = 0;
  
    while (*cmd_lst)
    {
        if ((*cmd_lst)->para_num == *prev_par_num && !is_para)
        {
            if ((*cmd_lst)->cmd_split && is_builtin((*cmd_lst))) // AND
            {
                if (!pipe_builtin(*cmd_lst, r.old_fds, r.new_fds, 1)
                    || !(*cmd_lst)->next)
                    return 0;
                if ((*cmd_lst)->log_op == AND || (*cmd_lst)->log_op == OR)
                    break ;
                *cmd_lst = (*cmd_lst)->next;
                continue ;
            }
            else if (!pipe_builtin((*cmd_lst), r.old_fds, r.new_fds, 0))
                return 0;
            r.pid = fork();
            if (fork_failed(r.pid))
                return 0;

            execute_redirect(r.pid, r.old_fds, r.new_fds, (*cmd_lst));
            r.err = (*cmd_lst)->err;
            
            if (*cmd_lst && ((*cmd_lst)->log_op == AND || (*cmd_lst)->log_op == OR))
                break;

            (*cmd_lst) = (*cmd_lst)->next;
            continue;
        }
        else if ((*cmd_lst)->para_num > *prev_par_num && is_para)
        {
            r.pid = fork();
            if (r.pid == -1)
                exit(EXIT_FAILURE); // Handle fork error
            if (r.pid == 0) // Child process (subshell)
            {
                g_exit_status = 0;
                *prev_par_num = current_para;
                // Process commands within subshell
                while (*cmd_lst && (*cmd_lst)->para_num >= current_para)
                {
                    run_commands(cmd_lst, prev_par_num);
                }
                exit(g_exit_status);
            }
            else // Parent process
            {
                waitpid(r.pid, &r.wait_status, 0);
                g_exit_status = WEXITSTATUS(r.wait_status);
                // Advance cmd_lst past subshell
                while (*cmd_lst && (*cmd_lst)->para_num >= current_para)
                    *cmd_lst = (*cmd_lst)->next;
                continue;
            }
        }
    }

	wait_for_children(r.pid, &r.wait_status, r.err);
		
	if (*cmd_lst && ((*cmd_lst)->log_op == AND || (*cmd_lst)->log_op == OR))
	{
		tmp_log_op = (*cmd_lst)->log_op;
		*prev_par_num = (*cmd_lst)->para_num;  // ?????
		(*cmd_lst) = (*cmd_lst)->next;
		// (*cmd_lst)->prev = 0;
		return tmp_log_op;
	}
	return (0);
	
}




function execute_node(node):
    switch node.type:

        case 'ExecNode':
            // simple command
            return run_command(node.command)

        case 'PipeNode':
            // chain all children with pipes
            inputs  = null
            processes = []
            for cmdNode in node.children:
                proc = spawn(cmdNode.command, stdin=inputs)
                processes.append(proc)
                inputs = proc.stdout
            // wait for all, return last exit status
            wait_all(processes)
            return processes[-1].exit_status

        case 'AndNode', 'OrNode':
            // for both && and ||, iterate children in order
            for child in node.children:
                status = execute_node(child)

                if node.type == 'AndNode' and status != SUCCESS:
                    // &&: on first failure, stop
                    break

                if node.type == 'OrNode' and status == SUCCESS:
                    // ||: on first success, stop
                    break

            return status

        case 'SubshellNode':
            // isolate in a subprocess
            pid = fork()
            if pid == 0:
                // child: run its subtree and exit
                code = execute_node(node.child)
                exit(code)
            else:
                // parent: wait for subshell
                return wait(pid)



run_commands(cmd)
{
    if execute_node
        execute_node()
        set_exit_code()
    else if pipe_node
        fork()
        setuppipe()
        run_commands(cmd.child)
    else if and_node
        run_commands(cmd.child)
    else if subshell_node
        // fork()
        run_commands(cmd.child)
        wait()
}