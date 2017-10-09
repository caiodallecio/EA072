#ifndef __server__
#define __server__

#define SERVERPATH "/home/caio/atividade7/meu-webspace" 
#define True 1
#define False 0
#define M200 "OK\n\r"
#define M404 "Not found\n\r"
#define M403 "Forbidden\n\r"
#define M400 "Bad Request\n\r"
#define M405 "Not Allowed\n\r"
#define M500 "Internal Error\n\r"
#define M501 "Not Implemented\n\r"
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

char * message_header(int code);

char * get_current_time();

char * get_resource_time(resource_t * p);

char * get_server_message();

char * get_content_lenght_message(resource_t r);

char * get_content_type();

char * get_connection_type();

char * on_get(http_request_t get);

char * on_trace(http_request_t trace);

char * on_head(http_request_t head);

char * on_options(http_request_t options);

char * on_post(http_request_t post);
#endif
