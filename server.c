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
resource_t * get_resource(char * path, char * resource, unsigned char load_data){
	
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
						if(load_data == True) {
							filedesc = open(result, O_RDONLY);
							data = malloc(sizeof(char) * fileStat.st_size);
							read(filedesc, data, fileStat.st_size);
							close(filedesc);
							ret->data = data;
						} else {
							ret->data = NULL;
						}
						ret->code = 200;
						ret->size = fileStat.st_size;
						ret->modified = fileStat.st_mtime;
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
										
										
										if(load_data == True) {
											filedesc = open(welcome_path, O_RDONLY);
											data = malloc(sizeof(char) * fileStat_welcome.st_size);
											read(filedesc, data, fileStat_welcome.st_size);
											lose(filedesc);
											ret->data = data;
										} else {
											ret->data = NULL;
										}
										ret->code = 200;
										ret->size = fileStat_welcome.st_size;
										ret->modified = fileStat_welcome.st_mtime;	
										
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
									if (get_resource == True) {
										filedesc = open(index_path, O_RDONLY);
										data = malloc(sizeof(char) * fileStat_index.st_size);
										read(filedesc, data, fileStat_index.st_size);
										close(filedesc);
										ret->data = data;
									} else {
										ret->data = NULL;
									}
									ret->code = 200;
									ret->size = fileStat_index.st_size;
									ret->modified = fileStat_index.st_mtime;
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
											if (get_resource == True) {
												filedesc = open(welcome_path, O_RDONLY);
												data = malloc(sizeof(char) * fileStat_welcome.st_size);
												read(filedesc, data, fileStat_welcome.st_size);
												close(filedesc);
												ret->data = data;
											} else {
												ret->data = NULL;
											}
											ret->code = 200;
											ret->size = fileStat_welcome.st_size;
											ret->modified = fileStat_welcome.st_mtime;
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


void http_response(http_request_t * list){
	char * ret;
	while(list != NULL){
		switch (list->request){
			case GET:
				ret = on_get(list);
				break;
			case HEAD:
				ret = on_head(list);
				break;
			case TRACE:
				ret = on_trace(list);
				break;
			case OPTIONS:
				ret = on_options(list);
				break;
			case POST:
				ret = on_post(list);
				break;

		
		}
	}
}

char * get_current_time(){
	#define n_characters 26
	time_t time;
	time(&time);
	struct tm * info = localtime(&time);
	char * time_s = asctime(info);
	char * ret = malloc(sizeof(char) * strlen(time_s));
	strcpy(ret,time_s);
	return ret;
}

char * get_resource_time(resource_t * p) {
	struct tm * info = localtime(&p->modified);
	char * time_s = asctime(info);
	char * ret = malloc(sizeof(char) * strlen(time_s));
	strcpy(ret,time_s);
	return ret;
}

char * errorProc(int errorN) {
	char* error[300];

	switch(errorN){
		case 400://Bad Request
			strcpy(error,"<html>\n<head>\n<title>Error 400 Bad Request</title>\n</head>\n</html>\n");
			break;
		case 403://Forbidden
			strcpy(error,"<html>\n<head>\n<title>Error 403 Forbidden</title>\n</head>\n</html>\n");
			break;
		case 404://Not Found
			strcpy(error,"<html>\n<head>\n<title>Error 404 Not Found</title>\n</head>\n</html>\n");
			break;
		case 405:// Method Not Allowed
			strcpy(error,"<html>\n<head>\n<title>Error 405 Method Not Allowed</title>\n</head>\n</html>\n");
			break;
		case 500:// Internal Server Error
			strcpy(error,"<html>\n<head>\n<title>Error 500 Internal Server Error</title>\n</head>\n</html>\n");
	    	break;
		case 501:// Not Implemented
			strcpy(error,"<html>\n<head>\n<title>Error 501 Not Implemented</title>\n</head>\n</html>\n");
			break;
		default:
			strcpy(error,"<html>\n<head>\n<title>Unkown error</title>\n</head>\n</html>\n");
			break;
			
	}
	return error;
}

	