%{
#include "y.tab.h"
#include <string.h>
%}

%%



GET/.*			{
				yylval.request = GET;
				return(REQUEST);
				}
POST/.*			{
				yylval.request = POST;
				return(REQUEST);
				}
OPTIONS/.*		{
				yylval.request = OPTIONS;
				return(REQUEST);
				}
HEAD/.*			{
				yylval.request = HEAD;
				return(REQUEST);
				}
TRACE/.*		{
				yylval.request = TRACE;
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
