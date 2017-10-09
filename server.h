#ifndef __server__
#define __server__

#define SERVERPATH "/home/caio/atividade7/meu-webspace" 
#define True 1
#define False 0
struct resource{
	time_t modified;	
	char * data;
	long size;
	int code;
};
typedef struct resource resource_t;
 
resource_t * get_resource(char * path, char * resource, unsigned char data);

void http_response(http_request_t * list);

char * get_current_time();

char * get_resource_time(resource_t * p);

char * errorProc(int errorN);
#endif
