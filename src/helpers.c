/**
 *	Cut protocol from url
 */
char* _cutProtocol(char* _url) {
	char url[strlen(_url)];
	strcpy(url, _url);

	char* preCutedUrl = strstr(url, "://");
	char* cutedUrl = (char*) malloc(strlen(preCutedUrl) - 3);
	strcat(cutedUrl, &preCutedUrl[3]);

	return cutedUrl;
}


/** 
 * Is url has http or https in the begin 
 */
char* _hasProtocol(char *path) {
    char* isHttpArr = "http://";
    char* isHttpsArr = "https://";

    char _path[strlen(path)];
    _path[0] = 0x00;
    strcpy(_path, path);

    if( strncmp(isHttpArr, _path, 7) == 0 ) {
        return "http://";
    }
    else if( strncmp(isHttpsArr, _path, 8) == 0 ) {
        return "https://";
    }

    return 0;
}


/**
 *  Cut files with base extensions from the url 
 */
char* pageExtensions[2] = { 
    ".html", 
    ".php" 
};

char* _normalizeUrl(char* address) {
    char _address[strlen(address)];
    strcpy(_address, address);
    
    char* protocol = _hasProtocol(_address);  

    char* chunk = (char*) malloc(strlen(_address)); 
    if( protocol != 0 ) {
        strcat(chunk, protocol);
        strcat(chunk, strtok(_address, protocol));
    }   

    char* strPart = "";
    int arrSize = sizeof(pageExtensions) / sizeof(pageExtensions[0]);

    while (strPart != NULL) {
        strPart = strtok(NULL, "/");
        
        if( strPart != NULL ) { 
            strcat(chunk, "/");

            for( int i = 0; i < arrSize; i++ ) {
                if( strstr(strPart, pageExtensions[i]) != NULL ) return chunk;
            }

            strcat(chunk, strPart);
        }
    }

    return chunk;
}


/**
 * Split url by '/' and return array of url chunks
 */
char** splitUrlBySlash(char* address){
	char* normalUrl = _normalizeUrl(address);
	
	char* urlWithoutProtocol = _cutProtocol(normalUrl); 

	char* chunk;
	chunk = strtok(urlWithoutProtocol, "/");

	int chunkIndex = 0;
	char** out = malloc(sizeof(char));
	while( chunk != NULL ) {
		realloc(out, strlen(chunk) * sizeof(char));
		out[chunkIndex] = chunk;
		chunk = strtok(NULL, "/");
		chunkIndex++;
	}

	return out;
};

