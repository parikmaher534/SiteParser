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


/* Cut files with base extensions from the url */
char* pageExtensions[2] = { 
    ".html", 
    ".php" 
};

char* normalizeUrl(char* address) {
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
                if( strstr(strPart, pageExtensions[i]) != NULL ) goto parseEnd;
            }

            strcat(chunk, strPart);
        }
    }
    parseEnd:

    return chunk;
}

