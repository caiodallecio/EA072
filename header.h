#ifndef __header__
#define __header__

#define GET 0
#define POST 1
#define TRACE 2
#define HEAD 3
#define OPTIONS 4

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
	int  request;
	char * resource;
	commands_t * header;
	struct http_request * next;
};
typedef struct http_request http_request_t;



http_request_t * list;
#endif
