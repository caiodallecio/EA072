%{
#include "header.h"
#include "y.tab.h"
#include <string.h>
%}

%%



GET/.*			{
				yylval.req = GET;
				return(REQUEST);
				}
POST/.*			{
				yylval.req = POST;
				return(REQUEST);
				}
OPTIONS/.*		{
				yylval.req = OPTIONS;
				return(REQUEST);
				}
HEAD/.*			{
				yylval.req = HEAD;
				return(REQUEST);
				}
TRACE/.*		{
				yylval.req = TRACE;
				return(REQUEST);
				}
#.*				;
[\n]			return(LINEFEED);
,[ \t]*			return(VIRGULA);
:[ \t]			return(DOISPTS);
:$				return(DOISPTS);

[^:,\n#]+:?[^ :,\n#]+	{  
						yylval.text = malloc(sizeof(char) * strlen(yytext));
						strcpy(yylval.text,yytext);
						return (PALAVRA);
						}


%%
