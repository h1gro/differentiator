#ifndef DIFFR_FUNCS
#define DIFFR_FUNCS

node_t* DiffrAdd     (struct node_t* node, struct tree_t* tree);
node_t* DiffrSub     (struct node_t* node, struct tree_t* tree);
node_t* DiffrMull    (struct node_t* node, struct tree_t* tree);
node_t* DiffrDiv     (struct node_t* node, struct tree_t* tree);
node_t* DiffrDeg     (struct node_t* node, struct tree_t* tree);

node_t* DiffrSin     (struct node_t* node, struct tree_t* tree);
node_t* DiffrCos     (struct node_t* node, struct tree_t* tree);
node_t* DiffrTan     (struct node_t* node, struct tree_t* tree);
node_t* DiffrCot     (struct node_t* node, struct tree_t* tree);

node_t* DiffrShx     (struct node_t* node, struct tree_t* tree);
node_t* DiffrChs     (struct node_t* node, struct tree_t* tree);
node_t* DiffrThx     (struct node_t* node, struct tree_t* tree);
node_t* DiffrCth     (struct node_t* node, struct tree_t* tree);

node_t* DiffrLn      (struct node_t* node, struct tree_t* tree);
node_t* DiffrDegExp  (struct node_t* node, struct tree_t* tree);
node_t* DiffrMullExp (struct node_t* node, struct tree_t* tree);

#endif
