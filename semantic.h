#ifndef SEMANTIC_H
#define SEMANTIC_H

int check_semantics(command *cmd);

void work_with_unknown_tokens();
void work_with_unknown_cmds();
void work_with_unknown_parens();


/*
typedef struct{
    char l[MAX_TOKEN_LEN];      // Çep identifikator
    token r;                    // Sag identifikator. Eger ýalňyşlyk peýda bolsa, tokeniň maglumatlaryny ulanmak üçin.
}compare_ident;

void compare_idents_add_new(char *l, token tok);
void check_a_to_b_b_to_b();*/
#endif // SEMANTIC_H
