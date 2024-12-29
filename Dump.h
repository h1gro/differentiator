#ifndef _DUMP_
#define _DUMP_

enum dump_calls
{
    DEFAULT_EXPR     = 1,
    FINAL_DERIVATIVE = 2,
    SIMPLIFICATION   = 3,
    DIFFERENTIATION  = 4,
};

const int NUM_SENTENCES = 23;

static const char* sentences[NUM_SENTENCES] = {{"Cathedra of MATAN told"},
                                               {"In Bashkiria kids know"},
                                               {"This understand Australopithecus"},
                                               {"Extremly obvious"},
                                               {"When I am sleeping this dreaming to me"},
                                               {"If you did not know that I am so sorry for you"},
                                               {"It's like 2 + 2"},
                                               {"Be smart and learn it finally"},
                                               {"Ilsaf is FIVT, they know about that"},
                                               {"So that's easy I think, but not for you"},
                                               {"Bolzano-Weierstrass theorem makes you think"},
                                               {"My dog can solve it, come on man"},
                                               {"Note that"},
                                               {"In some intelectual grops of people it mean: Base"},
                                               {"Shut the f*ck up and calculate it!"},
                                               {"Not your level, I guess"},
                                               {"I solve my lovely-lovely crocodile!)"},
                                               {"Didn't even break a sweat"},
                                               {"Who could I be if I hadn't do math?"},
                                               {"What's the meaning of life?"},
                                               {"Who am I?"},
                                               {"Diffucult quastions, but easy derivative!)))"},
                                               {"Yeeeh get it!"}};

#define _WRITE_TEX_BRACKETS_(node, bracket1, bracket2) \
{                                                      \
    fprintf(dump, bracket1);                           \
                                                       \
    PrintExprInTex(node, dump);                        \
                                                       \
    fprintf(dump, bracket2);                           \
}                                                      \
while(0)

#define _WRITE_TEX_ARTICLE_()                                                                                                                             \
{                                                                                                                                                         \
    fprintf(tex_dump, "\\documentclass[a4paper,12pt]{article}\n\\usepackage{cmap}\n\\usepackage[T2A]{fontenc}\n\\usepackage[utf8]{inputenc}\n"            \
                      "\\usepackage[english,russian]{babel}\n\\usepackage{graphicx}\n\\graphicspath{{noiseimages/}}\n"                                    \
                      "\\usepackage{geometry}\n\\geometry{papersize={25 cm, 25 cm}}\n\\geometry{left=2cm}\n\\geometry{right=2cm}\n\\geometry{top=2cm}\n"  \
                      "\\geometry{bottom=2cm}\\usepackage{enumitem}\n\\date{}\n\n\n\\newtheorem{task}{Задача}\n\\begin{document}\n\\begin{titlepage}\n"   \
                      "\\begin{center}\n\t\\textsc{MOSKOW INSTITUTE OF PHYSICS AND TECHNOLOGY}\n\t\\vspace{2ex}\n\n"                                      \
                      "\\end{center}\n\\vspace{10ex}\n\\begin{center}\n\t\\vspace{24ex}\n"                                                                \
                      "\n\t\\vspace{2ex}\n\t\\textbf{\\Large{Differentiator}}\n\t\\vspace{34ex}\n"                                                        \
                      "\t\\begin{flushright}\n\t\\noindent\n\tDone by:\n\t\\textit{Komarov Artem}\n\t\\end{flushright}\n\t\\vfill\n\tDolgoprudny, 2024\n" \
                      "\\end{center}\n\\end{titlepage}\n\\newpage\n");                                                                                    \
}                                                                                                                                                         \
while(0)

void PrintGraphHead     (FILE* graph);
void LaTexDump(struct tree_t* tree, struct node_t* node, struct node_t* copy_node, FILE* dump, dump_calls command);
void PrintExprInTex     (struct node_t* node, FILE* dump);
void PrintEdge          (FILE* graph, struct node_t* node);
void PrintDefaultList   (FILE* graph, struct node_t* tree);
void WriteTree          (struct node_t* node, FILE* file_ptr);
void WriteNode          (struct node_t* node, FILE* file_ptr);
void SystemWithIntArg   (const char* str, int number_dump);

FILE* TreeDump          (struct tree_t* tree, struct node_t* node, struct node_t* copy_node, dump_calls command);

const char* GetSentense (int num);

#endif
