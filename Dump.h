#ifndef DUMP
#define DUMP

void TreeDump         (struct node_t* tree);
void PrintEdge        (FILE* graph, struct node_t* node);
void PrintGraphHead   (FILE* graph);
void PrintDefaultList (FILE* graph, struct node_t* tree);
void WriteTree        (struct node_t* node, FILE* file_ptr);
void WriteNode        (struct node_t* node, FILE* file_ptr);
void SystemWithIntArg (const char* str, int number_dump);

#endif
