#ifndef SEMANTIC_H
#define SEMANTIC_H

int check_semantics(command *cmd);

int semantic_cmd_assign(command *cmd);

int add_global_used_var(token *tok, char *tok_name);
#endif // SEMANTIC_H
