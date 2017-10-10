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
							}else{//caso index.html exista 
								
								//caso tenha permissao de leitura
								if(fileStat_index.st_mode & S_IRUSR){
									//ESCREVE INDEX.HTML NA TELA*
									if (load_data == True) {
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
											if (load_data == True) {
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

char * message_header(int code){
	char * ret;
	char * http = "HTPP/1.1 ";
	char * message;
	switch (code) {
		case 200:
			message = M200;
			break;
		case 400:
			message = M400;
			break;
		case 403:
			message = M403;
			break;
		case 404:
			message = M404;
			break;
		case 405:
			message = M405;
			break;
		default:
			message = "Unkown error\n\r";
	}
	ret = malloc(sizeof(char) * (strlen(message) + strlen(http) + 1));
	strcpy(ret,http);
	strcat(ret,message);
	return ret;

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
		if (ret != NULL)
			printf("%s",ret);

		list = list->next;
	}
}

char * get_current_time(){
	#define n_characters 26
	time_t t;
	time(&t);
	struct tm * info = localtime(&t);
	char * time_s = asctime(info);
	char * aux = "Date ";
	char * ret = malloc(sizeof(char) * (strlen(time_s) + strlen(aux)+1));
	strcpy(ret,aux);
	strcat(ret,time_s);
	return ret;
}

char * get_resource_time(resource_t * p) {
	struct tm * info = localtime(&p->modified);
	char * time_s = asctime(info);
	char * aux = "Last-Modified: ";
	char * ret = malloc(sizeof(char) * (strlen(time_s) + strlen(aux)+1));
	strcpy(ret,aux);
	strcat(ret,time_s);
	return ret;
}

char * errorProc(int errorN) {
	char * error;

	switch(errorN){
		case 400://Bad Request
			error = strdup("<html>\n<head>\n<title>Error 400 Bad Request</title>\n</head>\n</html>\n");
			break;
		case 403://Forbidden
			error = strdup("<html>\n<head>\n<title>Error 403 Forbidden</title>\n</head>\n</html>\n");
			break;
		case 404://Not Found
			error = strdup("<html>\n<head>\n<title>Error 404 Not Found</title>\n</head>\n</html>\n");
			break;
		case 405:// Method Not Allowed
			error = strdup("<html>\n<head>\n<title>Error 405 Method Not Allowed</title>\n</head>\n</html>\n");
			break;
		case 500:// Internal Server Error
			error = strdup("<html>\n<head>\n<title>Error 500 Internal Server Error</title>\n</head>\n</html>\n");
	    	break;
		case 501:// Not Implemented
			error = strdup("<html>\n<head>\n<title>Error 501 Not Implemented</title>\n</head>\n</html>\n");
			break;
		default:
			error = strdup("<html>\n<head>\n<title>Unkown error</title>\n</head>\n</html>\n");
			break;
			
	}
	return error;
}

char * get_server_message(){
	return strdup("Server: Caio & Carla 0.0.02 (Linux)\n\r");
}

char * get_content_lenght_message(resource_t * r){
	char * aux = "Content-Length: ";
	char * ret;
	char buffer[30];
	sprintf(buffer,"%ld\n\r",r->size);
	ret = malloc(sizeof(char) * (strlen(buffer) + strlen(aux) + 1));
	strcpy(ret,aux);
	strcat(ret,buffer);
	return ret;
}

char * get_content_type(){
	return strdup("Content-Type: text/html\n\r");
}

char * get_connection_type(){
	return strdup("Connection: Closed\n\r");
}

char * on_get(http_request_t * get){
	printf("On get\n");
	int total_lenght = 0;
	resource_t * resource = get_resource(SERVERPATH,get->resource,True);
	char * resource_message = message_header(resource->code);
	total_lenght += strlen(resource_message);
	char * server_message = get_server_message();
	total_lenght += strlen(server_message);
	char * current_time = get_current_time();
	total_lenght += strlen(current_time);
	char * resource_time = NULL;
	char * content_lenght = NULL;
	char * content_type = get_content_type();
	total_lenght += strlen(content_type);
	char * connection_type = get_connection_type();
	char * resource_data = NULL;
	char * ret;
	printf("On get\n");
	if(resource->code = 200){
		resource_time = get_resource_time(resource);
		total_lenght += strlen(resource_time);
		content_lenght = get_content_lenght_message(resource);
		total_lenght += strlen(content_lenght);
		resource_data = resource->data;
		total_lenght += strlen(resource_data);
		total_lenght += 2; // \n\r before data
	}
	else{
		resource_data = errorProc(resource->code);
		total_lenght += strlen(resource_data);
		resource->size = strlen(resource_data);
		content_lenght = get_content_lenght_message(resource);
		total_lenght += strlen(content_lenght);
	}
	total_lenght += 3; // \n\r\0 before end 
	ret = malloc(sizeof(char) * total_lenght);
	if (resource->code == 200) {
		sprintf(ret,"%s%s%s%s%s%s%s\n\r%s\n\r",
			resource_message,
			server_message,
			current_time,
			resource_time,
			content_lenght,
			content_type,
			connection_type,
			resource_data);
			free(resource_time);
	} else {
		sprintf(ret,"%s%s%s%s%s%s\n\r%s\n\r",
			resource_message,
			server_message,
			current_time,
			content_lenght,
			content_type,
			connection_type,
			resource_data);
	}
	free(resource_message);
	free(server_message);
	free(current_time);
	free(content_lenght);
	free(content_type);
	free(connection_type);
	free(resource_data);

	return  ret;


}

char * on_head(http_request_t * head){
	return NULL;
}

char * on_trace(http_request_t * trace){
	return NULL;
}

char * on_options(http_request_t * options){
	return NULL;
}

char * on_post(http_request_t * post){
	return NULL;
}
	