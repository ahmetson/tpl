
/** Dolandyryş komandanyň EGER-ÝA-ÝOGSA diýen görnüşi */

/** Komandanyň üç görnüşi bolýar:
        1) if:
            Üç sany birlikden ybarat bolmaly:
                Identifikator - "eger" diýen açar sözi
                Ýaý - bir birlik saklap, ol birligem ýa deňeşdirme ýa logiki komanda bolmaly.
                Identifikator - "bolsa" diýen açar sözi
            Özbaşdak manyny gaýtarmaýar.
        2) elif
            Üç sany birlikden ybarat bolmaly:
                Identifikator - "ýa" diýen açar sözi
                Ýaý - bir birlik saklap, ol birligem ýa deňeşdirme ýa logiki komanda bolmaly.
                Identifikator - "bolsa" diýen açar sözi
            Özbaşdak manyny gaýtarmaýar
        3) else
            Bir birlikden ybarat bolmaly:
                Identifikator - "ýogsa" diýen açar sözi bolmaly.

    ŞERT #8: Komandalaryň işleýşi.

*/
#ifndef CMD_CTRL_STATEMENT_H
#define CMD_CTRL_STATEMENT_H

#include "../cmds.h"

extern int CMD_CLASS_CTRL_STTMNT_IF;
extern int CMD_CLASS_CTRL_STTMNT_IFELSE;
extern int CMD_CLASS_CTRL_STTMNT_ELSE;

int is_cmd_ctrl_sttmnt(command *cmd);
void cmd_ctrl_sttmnt_if_mod(command *cmd, int item_num);
void cmd_ctrl_sttmnt_ifelse_mod(command *cmd, int item_num);
void cmd_ctrl_sttmnt_else_mod(command *cmd, int item_num);

int semantic_cmd_ctrl_sttmnt(command *cmd);

/** Faýla degişli kody C koda ýazýar **/
void cmd_ctrl_sttmnt_c_code(command *cmd, char **l, int *llen);

#endif // CMD_CTRL_STATEMENT_H
