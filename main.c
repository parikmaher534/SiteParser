#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>



/* Global varibles */
char *site;

/* Define functions */
void parseError(void *ctx);
void getSite(char *URL);
void getSource(char* address, char *filename);
void parseDocument(xmlNode *node);
void getImage(xmlChar *src);


int main(int argc, char *argv[]) {

    //Redefine parse error handler
    //libxml can't parse HTML5 and we emitt error to error.log 
    xmlSetGenericErrorFunc(NULL, (void *)parseError);
  
    /* First argument must be the site URL */
    if( argv[1] ) {
        site = argv[1];
        getSite(site);
    } else {
        printf("\nPlease set site URL.\n");
    }

    return 0;
}

void getSite(char *url) {
    getSource(url, "index.html");

    xmlDoc *doc = htmlReadFile("index.html", "UTF-8", 0);
    xmlNode *node = xmlDocGetRootElement(doc);
    xmlNode *element = xmlFirstElementChild(node);

    parseDocument(element);
}

void parseDocument(xmlNode *node) {
    const char *img = "img";
    xmlChar src[] = "src";

    if( strncmp((const char*)node->name, img, 2) == 0 ) {
         getImage(xmlGetProp(node, src));
    }
    
    while(node->next) {
        if( node->children ) {
            parseDocument(node->children);
        }

        node = node->next;
     
        if( strncmp((const char*)node->name, img, 2) == 0 ) {
            getImage(xmlGetProp(node, src));
        }
    }
}


void getImage(xmlChar *_src) {
    char *src = (char *)_src;
    char *isHttp = strstr(src, "http://");
    char *isHttps = strstr(src, "https://");
    int imageSourceLn = strlen(site) + strlen("/") + strlen(src);
    char path[imageSourceLn];
    path[0] = 0;

    /* If image from another domain or has a absolute path */
    if( isHttp != NULL || isHttps != NULL ) {
        strcat(path, src);
    } else {
        strcat(path, site);
        strcat(path, "/");
        strcat(path, src);
    }

    /* Get image name from src */
    char *srcToc;
    char *imageName;
    srcToc = strtok(src, "/");

    while( srcToc != NULL ) {
        imageName = srcToc;
        srcToc = strtok(NULL, "/");
    }

    getSource(path, imageName);
}


void getSource(char* address, char *filename) {
    FILE *pipe;
    char commandName[] = "curl -s -o ";
    int commandLn =  strlen(commandName) + strlen(filename) + strlen(address) + 1;
    char command[commandLn];
    command[0] = 0;

    strcat(command, commandName);
    strcat(command, filename);
    strcat(command, " ");
    strcat(command, address);
 
    if( !(pipe = popen(command, "r")) ) {
        exit(1);
    } 

    pclose(pipe);
    
    printf("\nDownload: %s\n\n", address);
}

//TODO: write to error log
void parseError(void *ctx) { };
