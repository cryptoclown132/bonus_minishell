/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienkroger <julienkroger@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:34:39 by jkroger           #+#    #+#             */
/*   Updated: 2025/08/26 18:07:52 by julienkroge      ###   ########.fr       */
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
# include <termios.h>

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

typedef enum node_type
{
	NODE_EXEC,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef enum token_type
{
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
	TOK_EQUAL,
	TOK_EOF,
	TOK_WILD
}	t_token_type;

typedef struct env_var
{
	char	**env;
	char	**vars;
}	t_env_var;

typedef struct cmd_tree
{
	t_node_type	type;
	int			infile;
	int			outfile;
	char		*err_file;
	int			err;

	union
	{
		struct
		{
			char	**cmd_split;
			char	*cmd_path;
			int		idx_path;
		}	s_exec;
		struct
		{
			struct cmd_tree	*left;
			struct cmd_tree	*right;
		}	s_pipe;
		struct
		{
			struct cmd_tree	*left;
			struct cmd_tree	*right;
		}	s_and;
		struct
		{
			struct cmd_tree	*left;
			struct cmd_tree	*right;
		}	s_or;
		struct
		{
			struct cmd_tree	*child;
		}	s_subshell;
	};
}	t_cmd_tree;

typedef struct s_tokens
{
	t_token_type		type;
	char				*token;
	struct s_tokens		*next;
}	t_tokens;

/* for norm */

typedef struct s_get_var
{
	int		i;
	int		j;
	// int		k;
	// char	*var_value;
	// char	*tmp;
}	t_get_var;

typedef struct s_expan
{
	int			i;
	char		*tmp_var;
}	t_expan;

typedef struct s_itw_loop
{
	char	*tmp_str;
	char	tmp;
	int		k;
}	t_itw_loop;

typedef struct s_cmd_split
{
	char	**new_cmd_split;
	char	**wildcard_args;
	int		i;
	int		j;
	int		k;
}	t_cmd_split;

typedef struct s_exec_pipe
{
	int					pipefd[2];
	int					status;
	int					right_status;
	pid_t				left_pid;
	pid_t				right_pid;
	struct sigaction	old_int;
	struct sigaction	old_quit;
}	t_exec_pipe;

typedef struct s_run_commands
{
	int		pid;
	int		new_fds[2];
	int		old_fds[2];
	int		wait_status;
	int		err;
}	t_run_commands;

/*************/
/* execution */
/*************/

int			exec_pipe(t_cmd_tree *cmd_lst, t_env_var *environ);
int			exec_cmd(t_cmd_tree *cmd_lst, bool in_parent, t_env_var *environ);
int			exec_and(t_cmd_tree *cmd_lst, t_env_var *environ);
int			exec_or(t_cmd_tree *cmd_lst, t_env_var *environ);
int			exec_subshell(t_cmd_tree *cmd_lst, t_env_var *environ);
int			execute_node(t_cmd_tree *cmd_lst, bool in_parent, t_env_var *environ);

/************/
/* builtins */
/************/

// builtin_pwd.c
void		pwd(void);

// builtin_cd.c
void		cd(t_cmd_tree *cmd_lst, t_env_var *environ);
char		*get_home_path(char **env_var);

// builtin_echo.c
void		echo(t_cmd_tree *cmd_lst);

// builtin_env.c
void		env(t_env_var *environ);

// builtin_unset.c
void		unset(t_cmd_tree *cmd_lst, t_env_var *environ);

/* builtin_unset_2.c */
int			get_char_index(const char *s, int c);

// builtin_exit.c
void		builtin_exit(t_cmd_tree *cmd_struct, t_env_var *environ);

/* builtin_export.c */
void		builtin_export(t_cmd_tree *cmd, t_env_var *environ);
char		**sort_export(char **expo);
char		*find_var(char **vars, char *var);
void		add_env(t_env_var *environ, char *var);

/* builtin_export_2.c */
int			ft_var(char **vars, char	*var);
int			var_lst(t_cmd_tree *cmd_lst, t_env_var *environ);

void		del_var(t_env_var *environ, char *var);

int			find_var_in_env(char **vars, char *var);

/* builtin_export_3.c */
int			len_equal(char *var);
char		**put_quotes(char **expo);
void		export_without_args(t_env_var *environ);
int			export_err(t_cmd_tree *cmd, char *str, int i, t_env_var *environ);

/* check_builtins.c */
int			run_builtin(t_cmd_tree *cmd_lst, t_env_var *environ);
int			run_builtin2(t_cmd_tree *cmd_lst, t_env_var *environ);
int			is_builtin(char *cmd);

/******************/
/* lexer_expander */
/******************/

/* lexer.c */
t_tokens	*lexer(t_tokens *token_lst, char *input, t_env_var environ);

/* lex_utils.c */
int			quote_len(char *s, int *i);
int			space_len(char *s, int *i);
int			without_quote(char *s, int *i);
int			word_len(char *s, int *i);

/* lex_err.c */
int			check_spaces(char *input);
int			check_unclosed_quotes(char *input);
int			check_log_op_2(char *input, int i);
int			check_log_op(char *input);
int			lex_error_check(char *input);

/* lex_err_parenthesis */
int			check_unclosed_parenthesis(char *input);
int			check_correct_parenthesis(char *input);

/* lex_err_an_or */
int			check_or(char *input, int i);
int			check_and(char *input, int i);

/* lex_err_red_2.c */
int			skip_rs_in(char *input, int i);
int			skip_rs_out(char *input, int i);

/* lex_err_red.c */
int			check_redir_in(char *input, int i);
int			check_redir_out(char *input, int i);
int			check_redir(char *input);

/* init_token.c */
t_tokens	*init_token(char *input, int *i, int token_type);
char		*itw_loop(char *input, char *tkn, int *j, t_env_var environ);
t_tokens	*init_token_word(char *input, int *i, t_env_var environ);
t_tokens	*init_redir(char *input, int *i, int type, t_env_var environ);
void		add_token(t_tokens **token_lst, t_tokens *token);

/* expander.c */
char		*expander(char *token, t_env_var environ);

/* env_var_utils.c */
char		*var_finder(t_env_var environ, char *var);
void		var_exist(char *token, t_env_var environ, int *i, char **var_value);
void		cat_var(char *token, t_env_var environ, char **var_value, t_get_var *v);

int			get_len(char *token, t_env_var environ);
char		*get_var(char *token, t_env_var environ);

/* wildcard.c */
char		*wildcard(char *str);

/**********/
/* parser */
/**********/

/* parser.c */
t_cmd_tree	*new_node(t_node_type t);
t_cmd_tree	*parse(t_cmd_tree *cmd_lst, char **input, t_env_var environ);

/* parse_and_or.c */
t_cmd_tree	*parse_or(t_tokens **token_lst, t_env_var environ);
t_cmd_tree	*parse_and(t_tokens **token_lst, t_env_var environ);

/* parse_pipe.c */
t_cmd_tree	*parse_pipeline(t_tokens **token_lst, t_env_var environ);

/* init_cmd_tree.c */
int			init_cmd_tree(t_tokens **token_lst, t_cmd_tree **cmd_lst, t_env_var environ);

/* free_tree.c */
void		free_string_array(char **arr);
void		free_cmd_tree(t_cmd_tree *node);

/* parse_cmd.c */
void		parse_command(t_tokens **token_lst, t_env_var environ, t_cmd_tree **cmd_node);

/* heredoc_utils.c */
void		here_doc_loop(char	*limiter, int fd, t_env_var environ);

/* cmd_path.c */
char		*get_cmd_path(t_env_var environ, char *cmd, t_cmd_tree *cmd_node);

/* minishell.c */
int			minishell(t_cmd_tree *cmd_lst, t_env_var *environ);

/* error.c  */
void		lex_error(char *token);
void		set_err(char *token, int err);
void		*set_exit_status(char *str, int err);
void		set_exit_void(char *str, int err);
int			set_exit_int(char *str, int err);

/* parser/env_list.c */
int			count_env_len(char **envp);
char		**copy_env(char **envp);

/* parse_redir.c */
void		parse_redirection(t_tokens **token_lst, t_env_var environ, t_cmd_tree **cmd_node);
t_cmd_tree	*parse_cmd_sequence(t_tokens **token_lst, t_env_var environ);

/* signals.c */
void		get_signals(void);
void		get_signals_child(void);
void		ignore_signals(struct sigaction *old_int, struct sigaction *old_quit);
void		activate_signals(struct sigaction *old_int, struct sigaction *old_quit);

/* signals_2.c */
void		ctrl_c(int status);
void		ctrl_c_child(int status);

/* mini_utils.c */
char		*user_input(void);
int			ft_strcmp(const char *s1, const char *s2);
int			get_exit_status(int status);

/* mini_utils_2.c */
char		*free_both_strjoin(char *s1, char *s2);
int			valid_input(char *input);
char		*ft_strjoin_zero(char const *s1, char const *s2);
void		free_env(t_env_var *environ);

#endif
