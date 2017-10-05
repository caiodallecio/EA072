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