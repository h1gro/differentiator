#ifndef RECURS_DOWN
#define RECURS_DOWN

/*Grammar:
G ::= E$
E ::= T{['+''-']T}*
T ::= D{['*''/']D}*
D ::= P{['^']P}*
P ::= '('E')' | N | F '('E')'
N ::= ['0'-'9']
V ::= 'x'
F ::= "func"
*/

node_t* GetPower           (struct expr_t* expr, struct tree_t* tree);
node_t* GetDollar          (struct expr_t* expr, struct tree_t* tree);
node_t* GetBrackets        (struct expr_t* expr, struct tree_t* tree);
node_t* GetNum             (struct expr_t* expr, struct tree_t* tree);
node_t* GetAddSub          (struct expr_t* expr, struct tree_t* tree);
node_t* GetMullDiv         (struct expr_t* expr, struct tree_t* tree);
node_t* GetFunction        (struct expr_t* expr, struct tree_t* tree);
node_t* GetVar             (struct expr_t* expr, struct tree_t* tree, char varyable);

#endif
