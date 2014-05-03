/** 
 * Is url has http or https in the begin 
 */
int _hasProtocol(char *path) {
    char *isHttp = strstr(path, "http://");
    char *isHttps = strstr(path, "https://");
    
    if( isHttp != NULL || isHttps != NULL ) return 1;

    return 0;
}
