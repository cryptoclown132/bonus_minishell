
NAME = minishell

SRC_DIR = .

SRC     = and_exec.c cmd_exec.c error.c free_tree.c main.c \
	mini_utils_2.c mini_utils.c minishell.c pipe_exec.c \
	signals_2.c signals.c subshell_exec.c \
	builtins/builtin_cd.c builtins/builtin_echo.c builtins/builtin_env.c builtins/builtin_exit.c \
	builtins/builtin_export_2.c builtins/builtin_export_3.c builtins/builtin_export_4.c \
	builtins/builtin_export.c builtins/builtin_pwd.c builtins/builtin_unset.c \
	builtins/builtin_unset2.c builtins/check_builtin_2.c builtins/check_builtins.c \
	lexer_expander/env_var_utils.c lexer_expander/expander.c lexer_expander/init_token.c \
	lexer_expander/lex_err_and_or.c lexer_expander/lex_err_parenthesis.c \
	lexer_expander/lex_err_red_2.c lexer_expander/lex_err_red.c lexer_expander/lex_err.c \
	lexer_expander/lex_utils.c lexer_expander/lexer.c lexer_expander/wildcard.c \
	parser/cmd_path.c parser/env_list.c parser/heredoc_utils.c parser/init_cmd_tree.c \
	parser/parse_and_or.c parser/parse_cmd.c parser/parse_pipe.c parser/parse_redir_2.c \
	parser/parse_redir.c parser/parser.c

OBJ = $(patsubst %.c, %.o, $(SRC))

HEADER = minishell.h

LIBFT = ./libft/libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror -I/opt/homebrew/opt/readline/include 

LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline

all: $(NAME)

$(NAME) : $(OBJ) $(HEADER)
	$(MAKE) -C libft
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(LIBFT)  -o $(NAME) 

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJ)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus

