

%{
#include <stdio.h>	
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "header.h"
#include "server.h"
%}


%union{
	char * text;
	struct params * parametro;
	struct commands * comando;
	int request;
	struct http_request * http_request_list;
}

%token VIRGULA
%token LINEFEED
%token ESPACO
%token DOISPTS
%token<text> PALAVRA
%token<request> REQUEST

%type<comando> header header_list
%type<http_request_list> http http_list
%type<parametro> parametro
%%

http_list : http http_list	{
								if ($1 == NULL){
								$$ = $2;
								list = $2;
								} else if ($2 == NULL) {
									$$ = $1;
									list = $1;
								} else {
									http_request_t * ret;
									ret = $1;
									ret->next = $2;
									$$ = ret;
									list = ret;
								}
							}
	  | http	{
		  			$$ = $1;
	  				list = $1;
				}
	

http: REQUEST PALAVRA LINEFEED header_list LINEFEED	{
														http_request_t * ret = malloc(sizeof(http_request_t));
														ret->request = $1;
														ret->resource = $2;
														ret->header = $4;
														$$ = ret;
														//printf("HTTP request found\n");														
													}
header_list: header header_list	{
									commands_t * ret;
									ret = $1;
									ret->next = $2;
									$$ = ret;
								}
		   | header	{$$ = $1;}





header: PALAVRA DOISPTS parametro LINEFEED	{
												commands_t * ret;
												ret = malloc(sizeof(commands_t));
												ret->comand = malloc(sizeof(char)*256);
												strcpy(ret->comand,$1);
												ret->param = $3;
												ret->next = NULL;
												$$ = ret;
												//printf("Header found\n");	
											}
	   | PALAVRA DOISPTS					{
												commands_t * ret;
												ret = malloc(sizeof(commands_t));
												ret->comand = malloc(sizeof(char)*256);
												strcpy(ret->comand,$1);
												ret->param = NULL;
												ret->next = NULL;
												$$ = ret;
												//printf("Header found\n");
	   										} 
	   | DOISPTS parametro LINEFEED			{printf("Falta comando\n"); $$ = NULL;}

	   | DOISPTS LINEFEED 					{printf("Falta Comando\n"); $$ = NULL;}
	   

parametro: PALAVRA VIRGULA parametro 		{
											params_t * ret;
											ret = malloc(sizeof(params_t));
											ret->param = malloc(sizeof(char)*256);
											strcpy(ret->param,$1);
											ret->next = $3;
											$$ = ret;
											//printf("Parametro found\n");	
											}
		 | PALAVRA parametro				{
			 								params_t * ret;
											ret = malloc(sizeof(params_t));
											ret->param = malloc(sizeof(char)*256);
											strcpy(ret->param,$1);
											strcat(ret->param,$2->param);
											ret->next = $2->next;
											free($2);
											$$ = ret;
											//printf("Parametro found\n");
		 									}	
	     | PALAVRA							{	
				 							params_t * ret;
											ret = malloc(sizeof(params_t));
											ret->param = malloc(sizeof(char)*256);
											strcpy(ret->param,$1);
											ret->next = NULL;
											$$ = ret;
											//printf("Parametro found\n");
									 		}


%%

