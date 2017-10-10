%{
#include "header.h"
#include "y.tab.h"
#include <string.h>
%}

%%



GET/.*			{
				yylval.request = GET;
				printf("GET was found\n");
				return(REQUEST);
				}
POST/.*			{
				yylval.request = POST;
				printf("POST was found\n");
				return(REQUEST);
				}
OPTIONS/.*		{
				yylval.request = OPTIONS;
				printf("OPTIONS was found\n");
				return(REQUEST);
				}
HEAD/.*			{
				yylval.request = HEAD;
				printf("HEAD was found\n");
				return(REQUEST);
				}
TRACE/.*		{
				yylval.request = TRACE;
				printf("TRACE was found\n");
				return(REQUEST);
				}
HTTP.*			;
#.*				;
[\n]			return(LINEFEED);
,[ \t]*			return(VIRGULA);
:[ \t]			return(DOISPTS);
:$				return(DOISPTS);
[ ]				;

[^ :,\n#]+:?[^ :,\n#]*	{  
						yylval.text = malloc(sizeof(char) * strlen(yytext));
						strcpy(yylval.text,yytext);
						printf("Palavra was found %s\n",yytext);
						return (PALAVRA);
						}


%%
