%{
#include <stdio.h>
#include <stdlib.h>
#include "gvlogo.tab.h"

extern void yyerror(const char* s);
%}

%option noinput nounput 

DIGIT [0-9]
CHAR [a-zA-Z]

%%

penup				{ return PENUP; }
pendown				{ return PENDOWN; }
color				{ return CHANGE_COLOR; }
clear				{ return CLEAR; }

move				{ return MOVE; }
turn				{ return TURN; }

goto				{ return GOTO; }
where				{ return WHERE; }

save				{ return SAVE; }
print				{ return PRINT; }

[0-9]*\.?[0-9]*			{ yylval.f = strtof(yytext, NULL); return NUMBER; }
\"[^\"]*\"			{ /* Want to ignore quotes */
				  yylval.s = strdup(yytext + 1);
				  yylval.s[strlen(yylval.s)-1] = '\0';
				  return STRING;
				}

[a-zA-Z0-9]+\.bmp		{ yylval.s = strdup(yytext); return FILE_PATH; }

\+				{ return PLUS; }
\-				{ return SUB; }
\/				{ return DIV; }
\*				{ return MULT; }

\=				{ return EQ; }
[a-zA-Z][a-zA-Z0-9]*		{ /* Was having problem here, so stdup */
				  yylval.s = strdup(yytext);
				  return ID;
				}

\;				{ return SEP; }

<<EOF>>				{ return END; }
[ \t\n\r]			{ ; }
.				{ yyerror("Lexing Syntax error.\n"); } 

%%

