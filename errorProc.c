char* errorProc(int errorN){
	char* error[300];

	switch(errorN){
		case 400://Bad Request
			strcpy(error,"<html>\n<head>\n<title>Error 400 Bad Request</title>\n</head>\n</html>\n");
			break;
		case 403://Forbidden
			strcpy(error,"<html>\n<head>\n<title>Error 403 Forbidden</title>\n</head>\n</html>\n");
			break;
		case 404://Not Found
			strcpy(error,"<html>\n<head>\n<title>Error 404 Not Found</title>\n</head>\n</html>\n");
			break;
		case 405:// Method Not Allowed
			strcpy(error,"<html>\n<head>\n<title>Error 405 Method Not Allowed</title>\n</head>\n</html>\n");
			break;
		case 500:// Internal Server Error
			strcpy(error,"<html>\n<head>\n<title>Error 500 Internal Server Error</title>\n</head>\n</html>\n");
	    	break;
		case 501:// Not Implemented
			strcpy(error,"<html>\n<head>\n<title>Error 501 Not Implemented</title>\n</head>\n</html>\n");
			break;
		default:
			strcpy(error,"<html>\n<head>\n<title>Unkown error</title>\n</head>\n</html>\n");
			break;
			
	}
	return error;
}