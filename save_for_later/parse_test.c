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
    char *infile_name, *outfile_name;
    union {
        struct { char **argv; } exec;
        struct { struct Node *left, *right; } pipe, and, or;
        struct { struct Node *child; } subshell;
    };
} Node;

// --- Tokens & tokenizer stubs ---
typedef enum {
    TOK_WORD, TOK_PIPE, TOK_AND, TOK_OR,
    TOK_LPAREN, TOK_RPAREN, TOK_REDIR_IN, TOK_REDIR_OUT, TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    char *text;
} Token;

static Token *tokens;
static int pos = 0;

static Token *peek()   { return &tokens[pos]; }
static Token *consume() { return &tokens[pos++]; }
static void expect(TokenType tt) {
    if (peek()->type != tt) {
        fprintf(stderr, "Parse error: expected %d, got %d\n", tt, peek()->type);
        exit(1);
    }
    consume();
}
static Node *new_node(NodeType t) {
    Node *n = calloc(1, sizeof(*n));
    n->type = t;
    return n;
}

// --- Forward decls ---
Node *parse_or(), *parse_and(), *parse_pipeline(), *parse_redirection(), *parse_command();
void print_ast(Node *n, int indent);

// --- parse_or: handles `||` by repeatedly calling parse_and() ---
Node *parse_or() {
    Node *left = parse_and();
    while (peek()->type == TOK_OR) {
        consume();
        Node *right = parse_and();
        Node *n = new_node(NODE_OR);
        n->or.left  = left;
        n->or.right = right;
        left = n;
    }
    return left;
}

// --- parse_and: handles `&&` by repeatedly calling parse_pipeline() ---
Node *parse_and() {
    Node *left = parse_pipeline();
    while (peek()->type == TOK_AND) {
        consume();
        Node *right = parse_pipeline();
        Node *n = new_node(NODE_AND);
        n->and.left  = left;
        n->and.right = right;
        left = n;
    }
    return left;
}

// --- parse_pipeline: handles `|` ---
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
    return left;
}

// --- parse_redirection: attaches `<` and `>` to a single command node ---
Node *parse_redirection() {
    Node *cmd = parse_command();
    while (peek()->type == TOK_REDIR_IN || peek()->type == TOK_REDIR_OUT) {
        Token *r = consume(), *f = peek();
        if (f->type != TOK_WORD) {
            fprintf(stderr, "Parse error: expected filename\n");
            exit(1);
        }
        if (r->type == TOK_REDIR_IN)  cmd->infile_name  = strdup(f->text);
        else                           cmd->outfile_name = strdup(f->text);
        consume();
    }
    return cmd;
}

// --- parse_command: WORD sequences or subshell `( … )` ---
Node *parse_command() {
    if (peek()->type == TOK_LPAREN) {
        consume();
        Node *child = parse_or();
        expect(TOK_RPAREN);
        Node *n = new_node(NODE_SUBSHELL);
        n->subshell.child = child;
        return n;
    }
    if (peek()->type != TOK_WORD) {
        fprintf(stderr, "Parse error: expected word or '('\n");
        exit(1);
    }
    Node *n = new_node(NODE_EXEC);
    int cap = 4, argc = 0;
    n->exec.argv = malloc(cap * sizeof(char*));
    while (peek()->type == TOK_WORD) {
        if (argc + 1 >= cap)
            n->exec.argv = realloc(n->exec.argv, (cap *= 2) * sizeof(char*));
        n->exec.argv[argc++] = strdup(consume()->text);
    }
    n->exec.argv[argc] = NULL;
    return n;
}

// --- AST pretty-printer ---
void print_ast(Node *node, int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
    switch (node->type) {
      case NODE_EXEC: {
        printf("EXEC(");
        for (char **a = node->exec.argv; *a; ++a) {
          if (a != node->exec.argv) printf(" ");
          printf("%s", *a);
        }
        printf(")");
        if (node->infile_name)  printf("  < %s", node->infile_name);
        if (node->outfile_name) printf("  > %s", node->outfile_name);
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

// --- Two test cases in main() ---
int main() {
    // static Token example1[] = {
    //     {TOK_WORD, "echo"}, {TOK_WORD, "first"},
    //     {TOK_AND,  "&&"},
    //     {TOK_WORD, "echo"}, {TOK_WORD, "second"},
    //     {TOK_OR,   "||"},
    //     {TOK_WORD, "echo"}, {TOK_WORD, "third"},
    //     {TOK_AND,  "&&"},
    //     {TOK_WORD, "echo"}, {TOK_WORD, "fourth"},
    //     {TOK_EOF,  NULL}
    // };
    static Token example[] = {
        {TOK_WORD, "echo"}, {TOK_WORD, "one"},
        {TOK_OR,   "||"},
        {TOK_WORD, "echo"}, {TOK_WORD, "two"},
        {TOK_AND,  "&&"},
        {TOK_WORD, "echo"}, {TOK_WORD, "three"},
        {TOK_OR,   "||"},
        {TOK_WORD, "echo"}, {TOK_WORD, "four"},
        {TOK_EOF,  NULL}
    };

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
// echo start | sed s/a/A/g && (grep pattern < data.txt) || (awk {print $1} file.csv) > out.md
    // Test #1
    tokens = example; pos = 0;
    printf("=== AST for example1 ===\n");
    print_ast(parse_or(), 0);

    // Test #2
    // tokens = example2; pos = 0;
    // printf("\n=== AST for example2 ===\n");
    // print_ast(parse_or(), 0);

    return 0;
}
