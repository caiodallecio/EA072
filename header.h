#ifndef __header__
#define __header__
enum request {GET = 0,POST,TRACE,HEAD,OPTIONS};
typedef enum request request_t;

struct params{
	char * param;
	struct params * next;	
};
typedef struct params params_t;

struct commands{
	char * comand;
	params_t * param;
	struct commands * next;
};
typedef struct commands commands_t;

struct http_request{
	request_t request;
	char * resource;
	commands_t * header;
	struct http_request * next;
};
typedef struct http_request http_request_t;



http_request_t * list;
#endif
