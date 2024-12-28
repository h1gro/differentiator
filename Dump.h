#ifndef _DUMP_
#define _DUMP_

enum dump_calls
{
    DEFAULT_EXPR     = 1,
    FINAL_DERIVATIVE = 2,
    SIMPLIFICATION   = 3,
    DIFFERENTIATION  = 4,
};

const int NUM_SENTENCES = 11;

static const char* sentences[NUM_SENTENCES] = {{"Cathedra of MATAN told"}, {"In Bashkiria kids know"},
                                               {"This understand Australopithecus"}, {"Extremly obvious"},
                                               {"When I am sleeping this dreaming to me"},
                                               {"If you did not know that I am so sorry for you"},
                                               {"It's like 2 + 2"}, {"Be smart and learn it finally"},
                                               {"Ilsaf is FIVT, they know about that"}, {"So that's easy I think, but not for you"},
                                               {"Bolzano-Weierstrass theorem makes you think"}};

void PrintGraphHead     (FILE* graph);
void LaTexDump          (struct node_t* node, struct node_t* copy_node, FILE* dump, dump_calls command);
void PrintExprInTex     (struct node_t* node, FILE* dump);
void PrintEdge          (FILE* graph, struct node_t* node);
void PrintDefaultList   (FILE* graph, struct node_t* tree);
void WriteTree          (struct node_t* node, FILE* file_ptr);
void WriteNode          (struct node_t* node, FILE* file_ptr);
void SystemWithIntArg   (const char* str, int number_dump);

FILE* TreeDump          (struct node_t* node, struct node_t* copy_node, dump_calls command);

const char* GetSentense (int num);

#endif
