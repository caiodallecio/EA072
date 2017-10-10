char * on_trace(http_request_t * trace){
	printf("On Trace\n");
	int total_lenght = 0;
	char * server_message = get_server_message();
	total_lenght += strlen(server_message);
	char * current_time = get_current_time();
	total_lenght += strlen(current_time);
	char * content_type = get_content_type();
	total_lenght += strlen(content_type);
	char * connection_type = get_connection_type();

	char * ret;
	
	total_lenght += 3; // \n\r\0 before end 
	
	ret = malloc(sizeof(char) * total_lenght);
	
	free(resource_message);
	free(server_message);
	free(current_time);
	free(content_lenght);
	free(content_type);
	free(connection_type);
	free(resource_data);

	return  ret;
	
	return NULL;
}
