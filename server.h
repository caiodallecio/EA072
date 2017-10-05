#ifndef __server__
#define __server__
char * get_resource(char * path, char * resource, int * code, long * size, time_t * time);

void http_response(commands_t * tree);

struct resource{
	time_t * modified;	
	char * data;
	long size;
	int code;
};
typedef struct resource resource_t;
#endif
