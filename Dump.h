#ifndef DUMP
#define DUMP

void PrintGraphHead   (FILE* graph);
FILE* TreeDump         (struct node_t* tree);
void LaTexDump        (struct node_t* node, FILE* dump);
void PrintExprInTex   (struct node_t* node, FILE* dump);
void PrintEdge        (FILE* graph, struct node_t* node);
void PrintDefaultList (FILE* graph, struct node_t* tree);
void WriteTree        (struct node_t* node, FILE* file_ptr);
void WriteNode        (struct node_t* node, FILE* file_ptr);
void SystemWithIntArg (const char* str, int number_dump);

#endif
