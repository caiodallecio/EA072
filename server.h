#ifndef __server__
#define __server__

struct resource{
	time_t modified;	
	char * data;
	long size;
	int code;
};
typedef struct resource resource_t;
 
resource_t * get_resource(char * path, char * resource);

void http_response(commands_t * tree);
#endif
