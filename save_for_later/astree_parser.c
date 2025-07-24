#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>    // for open()

// --- AST definitions ---
typedef enum {
    NODE_EXEC,
    NODE_PIPE,
    NODE_AND,
    NODE_OR,
    NODE_SUBSHELL
} NodeType;

typedef struct Node {
    NodeType type;

    // redirection filenames (NULL if none)
    char *infile_name;
    char *outfile_name;

    union {
        struct {
            char **argv;       // NULL-terminated
        } exec;
        struct {
            struct Node *left, *right;
        } pipe;
        struct {
            struct Node *left, *right;
        } and;
        struct {
            struct Node *left, *right;
        } or;
        struct {
            struct Node *child;
        } subshell;
    };
} Node;

// --- Tokens & tokenizer stubs ---
typedef enum {
    TOK_WORD,
    TOK_PIPE,       // |
    TOK_AND,        // &&
    TOK_OR,         // ||
    TOK_LPAREN,     // (
    TOK_RPAREN,     // )
    TOK_REDIR_IN,   // <
    TOK_REDIR_OUT,  // >
    TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    char *text;   // for TOK_WORD and redir symbols
} Token;

static Token *tokens;
static int pos = 0;

static Token *peek() {
    return &tokens[pos];
}
static Token *consume() {
    return &tokens[pos++];
}
static void expect(TokenType tt) {
    if (peek()->type != tt) {
        fprintf(stderr, "Parse error: expected token %d, got %d\n", tt, peek()->type);
        exit(1);
    }
    consume();
}

// --- AST helper ---
static Node *new_node(NodeType type) {
    Node *n = calloc(1, sizeof(Node));
    n->type = type;
    return n;
}

// --- Forward declarations ---
Node *parse_and_or();
Node *parse_pipeline();
Node *parse_redirection();
Node *parse_command();
void print_ast(Node *node, int indent);

// --- parse_command: handles subshell or exec ---
Node *parse_command() {
    Token *t = peek();
    if (t->type == TOK_LPAREN) {
        // subshell
        consume();               // '('
        Node *child = parse_and_or();
        expect(TOK_RPAREN);      // ')'
        Node *n = new_node(NODE_SUBSHELL);
        n->subshell.child = child;
        return n;
    }
    if (t->type != TOK_WORD) {
        fprintf(stderr, "Parse error: expected word or '('\n");
        exit(1);
    }
    // exec node
    Node *n = new_node(NODE_EXEC);
    // build argv: we take all consecutive WORD tokens as args
    int argc = 0, cap = 4;
    n->exec.argv = malloc(cap * sizeof(char*));
    while (peek()->type == TOK_WORD) {
        if (argc + 1 >= cap) {
            cap *= 2;
            n->exec.argv = realloc(n->exec.argv, cap * sizeof(char*));
        }
        n->exec.argv[argc++] = strdup(consume()->text);
    }
    n->exec.argv[argc] = NULL;
    return n;
}

// --- parse_redirection: attach <, > ---
Node *parse_redirection() {
    Node *cmd = parse_command();
    while (peek()->type == TOK_REDIR_IN || peek()->type == TOK_REDIR_OUT) {
        Token *redir = consume();
        Token *file  = peek();
        if (file->type != TOK_WORD) {
            fprintf(stderr, "Parse error: expected filename after redirection\n");
            exit(1);
        }
        // attach filename
        if (redir->type == TOK_REDIR_IN) {
            cmd->infile_name = strdup(file->text);
        } else {
            cmd->outfile_name = strdup(file->text);
        }
        consume();  // consume filename
    }
    return cmd;
}

// --- parse_pipeline: handle '|' ---
Node *parse_pipeline() {
    Node *left = parse_redirection();
    while (peek()->type == TOK_PIPE) {
        consume();
        Node *right = parse_redirection();
        Node *n = new_node(NODE_PIPE);
        n->pipe.left  = left;
        n->pipe.right = right;
        left = n;
    }
    // printf("=== PIPELINE ===\n");
    // print_ast(left, 0);
    return left;
}

// --- parse_and_or: handle '&&' and '||' ---
Node *parse_and_or() {
    Node *left = parse_pipeline();
    while (peek()->type == TOK_AND || peek()->type == TOK_OR) {
        TokenType op = consume()->type;
        Node *right = parse_pipeline();
        Node *n = new_node(op == TOK_AND ? NODE_AND : NODE_OR);
        if (op == TOK_AND) {
            n->and.left  = left;
            n->and.right = right;
        } else {
            n->or.left   = left;
            n->or.right  = right;
        }
        left = n;
    }

    return left;
}

// --- print_ast: recursive AST pretty-print ---
void print_ast(Node *node, int indent) 
{
    for (int i = 0; i < indent; i++) printf("  ");
    switch (node->type) {
    case NODE_EXEC: {
        printf("EXEC");
        // print argv
        printf("(");
        for (char **a = node->exec.argv; *a; ++a) {
            if (a != node->exec.argv) printf(" ");
            printf("%s", *a);
        }
        printf(")");
        // print redirections if any
        if (node->infile_name)
            printf("  < %s", node->infile_name);
        if (node->outfile_name)
            printf("  > %s", node->outfile_name);
        printf("\n");
        break;
    }
    case NODE_PIPE:
        printf("PIPE\n");
        print_ast(node->pipe.left,  indent+1);
        print_ast(node->pipe.right, indent+1);
        break;
    case NODE_AND:
        printf("AND\n");
        print_ast(node->and.left,  indent+1);
        print_ast(node->and.right, indent+1);
        break;
    case NODE_OR:
        printf("OR\n");
        print_ast(node->or.left,   indent+1);
        print_ast(node->or.right,  indent+1);
        break;
    case NODE_SUBSHELL:
        printf("SUBSHELL\n");
        print_ast(node->subshell.child, indent+1);
        break;
    }
}

int main() {
    // Tokens for: (echo hi | grep h) && cat < file.txt || echo fail > out.txt
    // static Token example[] = {
    //     // {TOK_LPAREN,     "("},
    //     {TOK_WORD,       "echo"}, {TOK_WORD, "hi"},
    //     {TOK_PIPE,       "|"},
    //     {TOK_WORD,       "grep"}, {TOK_WORD, "h"},
    //     // {TOK_RPAREN,     ")"},
    //     {TOK_AND,        "&&"},
    //     {TOK_WORD,       "cat"},
    //     {TOK_REDIR_IN,   "<"},     {TOK_WORD, "file.txt"},
    //     {TOK_OR,         "||"},
    //     {TOK_WORD,       "echo"},  {TOK_WORD, "fail"},
    //     {TOK_REDIR_OUT,  ">"},     {TOK_WORD, "out.txt"},
    //     {TOK_EOF,        NULL}
    // };

//     static Token example[] = {
//     {TOK_WORD,       "cat"},
//     {TOK_REDIR_IN,   "<"},     {TOK_WORD, "input.txt"},
//     {TOK_PIPE,       "|"},
//     {TOK_WORD,       "grep"},  {TOK_WORD, "foo"},
//     {TOK_AND,        "&&"},
//     {TOK_LPAREN,     "("},
//       {TOK_WORD,     "echo"},  {TOK_WORD, "success"},
//       {TOK_OR,       "||"},
//       {TOK_WORD,     "echo"},  {TOK_WORD, "fail"},
//     {TOK_RPAREN,     ")"},
//     {TOK_REDIR_OUT,  ">"},     {TOK_WORD, "result.txt"},
//     {TOK_EOF,        NULL}
// };
//cat < input.txt | grep foo && (echo success || echo fail) > result.txt





// Example 3: Nested subshells with pipes, &&, ||, and output redirection
// static Token example[] = {
//     {TOK_LPAREN,     "("},
//       {TOK_LPAREN,   "("},
//         {TOK_WORD,   "find"},    {TOK_WORD, "."},       {TOK_WORD, "-type"},
//         {TOK_WORD,   "f"},
//         {TOK_PIPE,   "|"},
//         {TOK_WORD,   "xargs"},   {TOK_WORD, "grep"},     {TOK_WORD, "TODO"},
//       {TOK_RPAREN,   ")"},
//       {TOK_AND,      "&&"},
//       {TOK_LPAREN,   "("},
//         {TOK_WORD,   "echo"},    {TOK_WORD, "done"},
//         {TOK_OR,      "||"},
//         {TOK_WORD,   "echo"},    {TOK_WORD, "retry"},
//       {TOK_RPAREN,   ")"},
//     {TOK_RPAREN,     ")"},
//     {TOK_PIPE,       "|"},
//     {TOK_WORD,       "tee"},    {TOK_WORD, "out.log"},
//     {TOK_REDIR_OUT,  ">"},      {TOK_WORD, "final.log"},
//     {TOK_EOF,        NULL}
// };

// ((find . -type f -exec grep TODO {} +) && (echo done || echo retry)) | tee out.log > final.log

// Example 4: Redirections inside and outside subshells, mixing && and ||
// static Token example[] = {
//     {TOK_WORD,       "echo"},   {TOK_WORD, "alpha"},
//     {TOK_AND,        "&&"},
//     {TOK_LPAREN,     "("},
//       {TOK_WORD,     "grep"},   {TOK_WORD, "beta"},    {TOK_WORD, "file.txt"},
//       {TOK_OR,       "||"},
//       {TOK_LPAREN,   "("},
//         {TOK_WORD,   "wc"},     {TOK_WORD, "-l"},
//         {TOK_REDIR_IN,"<"},     {TOK_WORD, "file.txt"},
//       {TOK_RPAREN,   ")"},
//     {TOK_RPAREN,     ")"},
//     {TOK_AND,        "&&"},
//     {TOK_WORD,       "echo"},   {TOK_WORD, "gamma"},
//     {TOK_REDIR_OUT,  ">"},      {TOK_WORD, "log.txt"},
//     {TOK_EOF,        NULL}
// };
// echo alpha && (grep beta file.txt || (wc -l < file.txt)) && echo gamma > log.txt

// Example 5: Long pipeline with multiple stages, redirections, and trailing subshell
// static Token example[] = {
//     {TOK_LPAREN,     "("},
//       {TOK_WORD,     "cmd1"},   {TOK_WORD, "arg1"},   {TOK_WORD, "arg2"},
//       {TOK_PIPE,     "|"},
//       {TOK_WORD,     "cmd2"},   {TOK_WORD, "arg3"},
//     {TOK_RPAREN,     ")"},
//     {TOK_AND,        "&&"},
//     {TOK_WORD,       "cmd3"},   {TOK_REDIR_IN, "<"},   {TOK_WORD, "in.txt"},
//     {TOK_PIPE,       "|"},
//     {TOK_WORD,       "cmd4"},   {TOK_WORD, "arg"},
//     {TOK_PIPE,       "|"},
//     {TOK_WORD,       "cmd5"},   {TOK_REDIR_OUT, ">"},  {TOK_WORD, "out.txt"},
//     {TOK_OR,         "||"},
//     {TOK_LPAREN,     "("},
//       {TOK_WORD,     "cmd6"},
//       {TOK_AND,      "&&"},
//       {TOK_WORD,     "cmd7"},
//       {TOK_PIPE,     "|"},
//       {TOK_WORD,     "cmd8"},
//     {TOK_RPAREN,     ")"},
//     {TOK_EOF,        NULL}
// };
// (cmd1 arg1 arg2 | cmd2 arg3) && cmd3 < in.txt | cmd4 arg | cmd5 > out.txt || (cmd6 && cmd7 | cmd8)

// // Example 6: Complex mix of all operators with arguments
// static Token example[] = {
//     {TOK_WORD,       "echo"},   {TOK_WORD, "start"},
//     {TOK_PIPE,       "|"},
//     {TOK_WORD,       "sed"},    {TOK_WORD, "s/a/A/g"},
//     {TOK_AND,        "&&"},
//     {TOK_LPAREN,     "("},
//       {TOK_WORD,     "grep"},   {TOK_WORD, "pattern"}, {TOK_REDIR_IN, "<"},
//       {TOK_WORD,     "data.txt"},
//     {TOK_RPAREN,     ")"},
//     {TOK_OR,         "||"},
//     {TOK_LPAREN,     "("},
//       {TOK_WORD,     "awk"},    {TOK_WORD, "{print $1}"}, {TOK_WORD, "file.csv"},
//     {TOK_RPAREN,     ")"},
//     {TOK_REDIR_OUT,  ">"},      {TOK_WORD, "out.md"},
//     {TOK_EOF,        NULL}
// };


// static Token example[] = {
//     {TOK_WORD,       "echo"},   {TOK_WORD, "first"},
//     {TOK_AND,        "&&"},
//     {TOK_WORD,       "echo"},   {TOK_WORD, "second"},
//     {TOK_OR,         "||"},
//     {TOK_WORD,       "echo"},   {TOK_WORD, "third"},
//     {TOK_AND,        "&&"},
//     {TOK_WORD,       "echo"},   {TOK_WORD, "fourth"},
//     {TOK_EOF,        NULL}
// };
//echo first && echo second || echo third && echo fourth

static Token example[] = {
    {TOK_WORD,       "echo"},   {TOK_WORD, "one"},
    {TOK_OR,         "||"},
    {TOK_WORD,       "echo"},   {TOK_WORD, "two"},
    {TOK_AND,        "&&"},
    {TOK_WORD,       "echo"},   {TOK_WORD, "three"},
    {TOK_OR,         "||"},
    {TOK_WORD,       "echo"},   {TOK_WORD, "four"},
    {TOK_EOF,        NULL}
};
// echo one || echo two && echo three || echo four

    tokens = example;
    pos = 0;

    Node *ast = parse_and_or();
    printf("=== AST ===\n");

    print_ast(ast, 0);
    return 0;
}

// (echo hi | grep h) && cat < file.txt || echo fail > out.txt

