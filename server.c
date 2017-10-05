#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include "header.h"
#include "server.h"
resource_t * get_resource(char * path){
	
	char *result = malloc(strlen(path)+strlen(resource)+1);//+1 for the zero-terminator
	char * index_path = malloc(strlen(result)+ strlen("index.html")+1);
	char * welcome_path = malloc(strlen(result)+ strlen("welcome.html")+1);
	char * data;
	resource_t * ret = malloc(sizeof(resource_t));
	struct stat fileStat;
	struct stat fileStat_index;
	struct stat fileStat_welcome;
	int filedesc;
	int index_status;
	int welcome_status;
	
	/*combina uma string com a outra*/
	if(path[0] == '.' && path[1] == '\0')
		strcpy(result,resource);
	else {
		strcpy(result, path);
		strcat(result, resource);
	}
	
	strcpy(index_path,result);
	strcat(index_path,"/index.html");
	
	strcpy(welcome_path,result);
	strcat(welcome_path,"/welcome.html");
	
	if (stat(result, &fileStat) < 0){
	     	//NOT FOUND
		ret->code = 404;
		free(result);
		free(index_path);
		free(welcome_path);
		return ret;
	} else{ 
		 //verifica se o arquivo possui permissao de leitura
		 if(fileStat.st_mode & S_IRUSR){
			 
			switch (fileStat.st_mode & S_IFMT){
			        case S_IFREG :
					filedesc = open(result, O_RDONLY);
					data = malloc(sizeof(char) * fileStat.st_size);
					read(filedesc, buffer, fileStat.st_size);
					close(filedesc);
					ret->code = 200;
					ret->size = fileStat.st_size;
					ret->modified = fileStat.st_mtime;
					ret->data = data;
					free(result);
					free(index_path);
					free(welcome_path);
					return ret;
						
			        case S_IFDIR : 
					
						//verifica se o Diretorio tem permissao de execucao
						if (fileStat.st_mode & S_IXUSR){
							index_status = stat(index_path, &fileStat_index);
				
							welcome_status = stat(welcome_path, &fileStat_welcome);
							
							//caso index.html n exista
							if(index_status < 0){
								//caso welcome.html tb n exista
								if(welcome_status < 0){
									//NOT FOUND
									ret->code = 404;
									return ret;
								}else{
									//caso tenha permissao de leitura
									if(fileStat_welcome.st_mode & S_IRUSR){
										
										//ESCREVE WELCOME.HTML NA TELA
										filedesc = open(welcome_path, O_RDONLY);
										data = malloc(sizeof(char) * fileStat.st_size);
										read(filedesc, buffer, fileStat_welcome.st_size);
										close(filedesc);
										ret->code = 200;
										ret->size = fileStat.st_size;
										ret->modified = fileStat.st_mtime;	
										ret->data = data;
										free(result);
										free(index_path);
										free(welcome_path);
										return ret;
									}else{
										//FORBIDDEN
										ret->code = 403;
										free(result);
										free(index_path);
										free(welcome_path);
										return ret; 
									}
								}
							}else{//caso index.html exista 
								
								//caso tenha permissao de leitura
								if(fileStat_index.st_mode & S_IRUSR){
									//ESCREVE INDEX.HTML NA TELA*
									filedesc = open(index_path, O_RDONLY);
									data = malloc(sizeof(char) * fileStat.st_size);
									read(filedesc, buffer, fileStat_index.st_size);
									close(filedesc);
									ret->code = 200;
									ret->size = fileStat.st_size;
									ret->modified = fileStat.st_mtime;
									ret->data = data;
									free(result);
									free(index_path);
									free(welcome_path);
									return ret;
								}else{//caso n tenha permissao de leitura
									
									//caso welcome.html nao exista
									if(welcome_status < 0){
										ret->code = 403;//FORBIDDEN
										free(result);
										free(index_path);
										free(welcome_path);
										return ret;
									}else{//caso welcome.html exista
										
										//caso welcome.html possua permissao de leitura
										if(fileStat_welcome.st_mode & S_IRUSR){
										
											//ESCREVE WELCOME.HTML NA TELA***************
											filedesc = open(welcome_path, O_RDONLY);
											data = malloc(sizeof(char) * fileStat.st_size);
											read(filedesc, buffer, fileStat_welcome.st_size);
											close(filedesc);
											ret->code = 200;
											ret->size = fileStat.st_size;
											ret->modified = fileStat.st_mtime;
											ret->data = data;
											free(result);
											free(index_path);
											free(welcome_path);
											return ret;
										}else{
											//FORBIDDEN
											ret->code = 403;
											free(result);
											free(index_path);
											free(welcome_path);
											return ret; 
										}
									}
								}
							
							}
						
							
						}
						else{
							//FORBIDDEN
							ret->code = 403;
							free(result);
							free(index_path);
							free(welcome_path);
							return NULL; 
						}
						break;
					default:
						//OK
						free(result);
						free(index_path);
						free(welcome_path);
						ret->code = 500;
						return ret;
		     }
		 }
		 else
			//FORBIDDEN
			free(result);
			free(index_path);
			free(welcome_path);
			ret->code = 403;
			return ret; 
		 
	 }
	
}


void http_response(commands_t * tree){
	char * path = "/home/caio/atividade7/meu-webspace";
	char deli = ' ';
	char * request = tree->comand;
	char * resource_name = strtok(tree->param->param,&deli);
	char * http = strtok(NULL,&deli);
	
	
	char * connection = NULL;
	char * resource = NULL;

	char current_time[26] ,change_time[26];

	time_t t;
	struct tm *info;

	int code;
	long size;
	while(tree != NULL){
		if(!strcmp("Connection",tree->comand))
			connection = tree->param->param;
		tree = tree->next;
	}

	time(&t);
	info =  localtime(&t);
	
	strftime(current_time, 26, "%Y-%m-%d %H:%M:%S", info);

	if(!strcmp(request,"GET")){
		resource = get_resource(path, resource_name, &code, &size, &t);
	}
	else if (!strcmp(request,"POST")){
		code = 405;
	}
	else if (!strcmp(request,"TRACE")){
		code = 405;
	}
	else if (!strcmp(request,"HEAD")){
		resource = get_resource(path, resource_name, &code, &size, &t);
		free(resource);
		resource = NULL;
	}
	else if (!strcmp(request,"OPTIONS")){
		code = 405;
	}
	else{
	}
	if (connection == NULL)
		code = 400;
	
	printf("%s\0",http);


	if (code != 200)
		printf("%s %d %s\n", http, code,"ERROR");
	else
		printf("%s %d %s\n", http, code,"OK");

	printf("Date: %s\n",current_time);
	printf("Server: Server da Carla e do Caio 0.0.001\n");
	printf("Connection: %s\n",connection);

	if (code == 200){
		info = localtime(&t);
		strftime(change_time, 26, "%Y-%m-%d %H:%M:%S", info);

		printf("Last-Modified: %s\n", change_time);
		printf("Content-Length: %ld\n", size);
		printf("Content-Type: text/html\n");
		if (resource != NULL){
			printf("%s",resource);
			free(resource);
		}	
	}		

}

