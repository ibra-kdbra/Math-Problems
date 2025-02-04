#ifndef MAIN_CONFIG_H
#define MAIN_CONFIG_H

#include <stdlib.h>

static const int NUMBER_OF_OWLS = 10;

static const size_t MAX_FORMULA_LENGTH = 65536;

static const size_t MAX_NAME_LENGTH = 1024;
static const char DEFAULT_DB_NAME[] = "simple.math";
static const char DEFAULT_ART_FOLDER[] = "article/";
static const char ART_MAIN_NAME[] = "main.tex";
static const char ART_CONTENT_NAME[] = "content.tex";
static const char ART_ASSET_FOLD_NAME[] = "assets";

static const char ARTICLE_PREFIX[] = "\\documentclass{article}\n\n"
"\\usepackage[utf8]{inputenc}\n"
"\\usepackage{tikz}\n"
"\\usepackage{pgfplots}\n\n"
"\\title{Generated Article}\n\n"
"\\begin{document}\n\n"
"\\maketitle\n\n"
"\\section{Function analysis}\n\n";

#define ARTICLE_GRAPH_PREFIX_TEMPLATE "\\begin{tikzpicture}\n\\begin{axis}[xmin = %lf-3, xmax = %lf+3]\n"
#define ARTICLE_GRAPH_SUFFIX "\\end{axis}\n\\end{tikzpicture}\n"
#define ARTICLE_GRAPH_TEMPLATE "    \\addplot[domain=%lf-3:%lf+3,samples=200,smooth,thick,%s]\n        {%s};\n"

static const char ARTICLE_POSTFIX[] = "\n\n\\end{document}\n";

static const char* TRANSITION_PHRASES[] = {
    "This is not a big deal to assume that\n",
    "As we know\n",
    "It is obvious to a five-year-old that\n",
    "According to the not-an-easter-egg transition\n",
    "As Sun Tzu once said:\n",
    "You should really reconsider reading this paper if it is'n obvious to you that\n",
    "We will later explain why\n",
    "Here goes a tight part where we for no apparent reason assume that\n",
    "Only the chosen one will understand that\n",
    "It is easy for me to type that\n",
    "You will need to learn for the exam that\n",
    "According to our previous calculations\n",
    "Roses are red,\\newline\nI'm not really a dancer.\\newline\nIt is hard to explain,"
    "\\newline\nSo here is just the answer:\\newline\n",
    "For well-known reason\n",
    "It is not hard to get that\n",
    "It took seven days for the God to create the Earth, so it should not take that long for you to understand that\n",
    "[redacted]\n",
    "[censored]\n",
    "Curious reader have already got to the point where\n",
    "At this point we would like to remind you to call your parents "
    "and drink a glass of water to stay hydrated. If you did this, we shall continue with\n",
    "As the void tells us\n",
    "Th3re y is a *cat o\\_n my ke\\&board!\n",
    "I don't even care what to write in this article, so here a nice-looking formula:\n",
    "You shall not pass through the transition\n",
    "You will get a free cake after completing the mathematical test of understanding that\n",
    "What if we assume that\n",
    "Then we get\n",
    "And the hero then goes all \\textbf{PEW PEW PEW} and says: "
    "\\emph{\'This is the time you die, you, very big bad math guy!\'}, "
    "but than this formula comes out of nowhere and shouts "
    "\\emph{\'No, this is the time YOU die!\'} and starts transforming into\n",
};

static const size_t TRANSITION_PHRASE_COUNT = sizeof(TRANSITION_PHRASES) / sizeof(*TRANSITION_PHRASES);

#endif