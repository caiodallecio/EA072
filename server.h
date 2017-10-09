#ifndef __server__
#define __server__
resource_t * get_resource(char * path);

void http_response(commands_t * tree);

struct resource{
	time_t * modified;	
	char * data;
	long size;
	int code;
};
typedef struct resource resource_t;
#endif
