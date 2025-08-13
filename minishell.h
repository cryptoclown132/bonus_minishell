/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:34:39 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/13 17:44:26 by julienkroge      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// #define _GNU_SOURCE 


# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <string.h> 
# include <sys/ioctl.h>
# include <readline/history.h>
# include <readline/readline.h>
// # include </opt/homebrew/opt/readline/include/readline/readline.h>
// # include </opt/homebrew/opt/readline/include/readline/history.h>
// # include </opt/homebrew/opt/readline/include/readline/rlstdc.h>

# include <sys/wait.h>
# include <stdbool.h>
#include <termios.h>


# ifdef __linux__
// Linux
#  define PATH_SIZE 4096
# else
// Mac
#  define PATH_SIZE 1024
# endif

# define READ_END 0
# define WRITE_END 1

extern int	g_exit_status;


typedef enum {
	NODE_EXEC,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
} node_type;

typedef enum {
	TOK_WORD, 
	TOK_PIPE, 
	TOK_AND, 
	TOK_OR,
	TOK_LPAREN, 
	TOK_RPAREN,
	TOK_IN, 
	TOK_OUT,
	TOK_APP,
	TOK_DOC,
	TOK_EQUAL, //check later
	TOK_EOF  //check later
} token_type;

typedef struct env_var
{
	char	**env;
	char	**vars;
} env_var;

typedef struct cmd_tree {
	node_type       type;
	
	int				infile;
	int				outfile;
	char			*err_file;
	int				err;

	// char			**env;
	// char			**var_lst;

	union {
		struct {
			char    **cmd_split;
			char    *cmd_path;
		} exec;

		struct {
			struct cmd_tree  *left;
			struct cmd_tree *right;
		} pipe;

		struct {
			struct cmd_tree *left;
			struct cmd_tree *right;
		} and;

		struct {
			struct cmd_tree *left;
			struct cmd_tree *right;
		} or;

		struct {
			struct cmd_tree *child;
		} subshell;
	};
} cmd_tree;


typedef struct s_tokens
{
	token_type		type;
	char			*token;
	struct s_tokens	*next;
}t_tokens;



/* for norm */

// typedef struct s_get_var
// {
// 	int		i;
// 	int		j;
// 	int		k;
// 	char	*var_value;
// 	char	*tmp;
// }t_get_var;

typedef struct s_expan
{
	int			i;
	char		*tmp_var;
}t_expan;

typedef struct s_itw_loop
{
	char	*tmp_str;
	char	tmp;
	int		k;
}t_itw_loop;

// typedef struct s_here_loop
// {
// 	char	*input;
// 	char	*limiter;
// 	char	*tmp;
// }t_here_loop;

// typedef struct s_init_cmd_struct
// {
// 	t_tokens	*tmp_t;
// 	t_cmds		*cmd;
// 	int			prev;
// }t_init_cmd_struct;

typedef struct s_run_commands
{
	int		pid;
	int		new_fds[2];
	int		old_fds[2];
	int		wait_status;
	int		err;
}t_run_commands;


/*************/
/* execution */
/*************/


int exec_pipe(cmd_tree *cmd_lst, env_var *environ);
// int exec_cmd(cmd_tree *cmd_lst, bool in_parent);
int exec_cmd(cmd_tree *cmd_lst, bool in_parent, env_var *environ);
int exec_and(cmd_tree *cmd_lst, env_var *environ);
int exec_or(cmd_tree *cmd_lst, env_var *environ);
int exec_subshell(cmd_tree *cmd_lst, env_var *environ);


int execute_node(cmd_tree *cmd_lst, bool in_parent, env_var *environ);




/************/
/* builtins */
/************/

// builtin_pwd.c
void		pwd(void);

// builtin_cd.c
void		cd(cmd_tree *cmd_lst, env_var *environ);
char		*get_home_path(char **env_var);

// builtin_echo.c
void		echo(cmd_tree *cmd_lst);

// builtin_env.c
void		env(env_var *environ);

// builtin_unset.c
void		unset(cmd_tree *cmd_lst, env_var *environ);
// int				valid_input_helper_unset(char *cur_cmd, int n);
int			valid_input_helper(char *cur_cmd, int n);

// builtin_exit.c
void		builtin_exit(cmd_tree *cmd_lst);

/* builtin_export.c */
void	builtin_export(cmd_tree *cmd, env_var *environ);
char		**sort_export(char **expo);
char		*find_var(char **vars, char *var);
void		add_env(env_var *environ, char *var);

/* builtin_export_2.c */
int			ft_var(char **vars, char	*var);
// int			var_lst(cmd_tree *cmd_lst);
int	var_lst(cmd_tree *cmd_lst, env_var *environ);

void	del_var(env_var *environ, char *var);

int	find_var_in_env(char **vars, char *var);

/* builtin_export_3.c */
int			len_equal(char *var);
char		**put_quotes(char **expo);
void		export_without_args(env_var *environ);
// int			export_err(cmd_tree *cmd, char *str, int i);
int	export_err(cmd_tree *cmd, char *str, int i, env_var *environ);

/* check_builtins.c */
// int	run_builtin(cmd_tree *cmd_lst);
int	run_builtin(cmd_tree *cmd_lst, env_var *environ); 
int	run_builtin2(cmd_tree *cmd_lst, env_var *environ);
int	is_builtin(char *cmd);



/******************/
/* lexer_expander */
/******************/

/* lexer.c */
// t_tokens	*lexer(t_tokens *token_lst, char *input, char **envp);
t_tokens	*lexer(t_tokens *token_lst, char *input, env_var environ);

/* lex_utils.c */
int	quote_len(char *s, int *i);
int	space_len(char *s, int *i);
int	without_quote(char *s, int *i);
int	word_len(char *s, int *i);

/* lex_err.c */
int	check_spaces(char *input);
int	check_unclosed_quotes(char *input);
int	check_or(char *input, int i);
int	check_and(char *input, int i);
int	check_log_op_2(char *input, int i);
int	check_log_op(char *input);
int	check_unclosed_parenthesis(char *input);
int	lex_error_check(char *input);

/* lex_err_red.c */
int	skip_rs_in(char *input, int i);
int	skip_rs_out(char *input, int i);
int	check_redir_in(char *input, int i);
int	check_redir_out(char *input, int i);
int	check_redir(char *input);

/* init_token.c */
t_tokens	*init_token(char *input, int *i, int token_type);
// char	*itw_loop(char *input, char *tkn, int *j, char **envp);
char	*itw_loop(char *input, char *tkn, int *j, env_var environ);
// t_tokens	*init_token_word(char *input, int *i, char **envp);
t_tokens	*init_token_word(char *input, int *i, env_var environ);
// t_tokens	*init_redir(char *input, int *i, int type, char **envp);
t_tokens	*init_redir(char *input, int *i, int type, env_var environ);
void	add_token(t_tokens **token_lst, t_tokens *token);

/* expander.c */
// char	*expander(char *token, char **envp);
char	*expander(char *token, env_var environ);

/* env_var_utils.c */
char	*var_finder(env_var environ, char *var);
void	var_exist(char *token, env_var environ, int *i, char **var_value);

// char	*var_exist(char *token, char **envp, int *i, char **var_value);
int	get_len(char *token, env_var environ);
// char	*get_var(char *token, char **envp);
char	*get_var(char *token, env_var environ);



/**********/
/* parser */
/**********/

/* parser.c */
// cmd_tree *new_node(node_type t, char **envp);
cmd_tree *new_node(node_type t);
// cmd_tree	*parse(cmd_tree *cmd_lst, char **input, char **envp);
// cmd_tree	*parse(cmd_tree *cmd_lst, char **input, char **envp, char **var_lst);
cmd_tree	*parse(cmd_tree *cmd_lst, char **input, env_var environ);

/* parse_and_or.c */
cmd_tree *parse_or(t_tokens **token_lst, env_var environ);
cmd_tree *parse_and(t_tokens **token_lst, env_var environ);

/* parse_pipe.c */
cmd_tree *parse_pipeline(t_tokens **token_lst, env_var environ);

/* init_cmd_tree.c */
void	free_string_array(char **arr);
// int	init_cmd_tree(t_tokens **token_lst, cmd_tree **cmd_lst, char **envp);
int	init_cmd_tree(t_tokens **token_lst, cmd_tree **cmd_lst, env_var environ);

/* parse_cmd.c */
cmd_tree *parse_command(t_tokens **token_lst, env_var environ);

/* heredoc_utils.c */
// void	here_doc_loop(char	*limiter, int fd, char **env);
void	here_doc_loop(char	*limiter, int fd, env_var environ);

/* cmd_path.c */
char	*get_cmd_path(env_var environ, char *cmd, cmd_tree *cmd_node);

/* minishell.c */
// int	minishell(cmd_tree *cmd_lst, char ***env);
int	minishell(cmd_tree *cmd_lst, env_var *environ);

/* error.c  */
void	lex_error(char *token);
void	set_err(char *token, int err);
void	*set_exit_status(char *str, int err);
void	set_exit_void(char *str, int err);
int     set_exit_int(char *str, int err);


/* parser/env_list.c */
int	count_env_len(char **envp);
char	**copy_env(char **envp);


/* parse_redir.c */
cmd_tree *parse_redirection(t_tokens **token_lst, env_var environ);

/* signals.c */
void	get_signals(void);
void	get_signals_child(void);



/* mini_utils.c */
char	*user_input(void);
int	ft_strcmp(const char *s1, const char *s2);

/* mini_utils_2.c */
char	*free_both_strjoin(char *s1, char *s2); 
int		valid_input(char *input);
char	*ft_strjoin_zero(char const *s1, char const *s2);
void	free_env(env_var *environ);

#endif
