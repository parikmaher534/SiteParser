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
void getSource(char* address, char *filename);
void parseDocument(xmlNode *node);
void getImage(xmlChar *src);


int main(int argc, char *argv[]) {
    site = argv[1];

    getSource(site, "index.html");

    xmlDoc *doc = htmlReadFile("index.html", "UTF-8", 0);
    xmlNode *node = xmlDocGetRootElement(doc);
    xmlNode *element = xmlFirstElementChild(node);

    parseDocument(element);
    
    return 0;
}

void parseDocument(xmlNode *node) {
    const char *img = "img";
    xmlChar src[] = "src";

    if( strncmp((const char*)node->name, img, 2) == 0 ) {
        getImage(xmlGetProp(node, src));
    }
    
    while(node->next) {
        node = node->next;
     
        if( strncmp((const char*)node->name, img, 2) == 0 ) {
            getImage(xmlGetProp(node, src));
        }

        if( node->children ) {
            parseDocument(node->children);
        }
    }
}


void getImage(xmlChar *_src) {
    char *src = (char *)_src;
    int imageSourceLn = strlen(site) + strlen("/") + strlen(src);
    char path[imageSourceLn];
    path[0] = 0;

    strcat(path, site);
    strcat(path, "/");
    strcat(path, src);
    puts(path);

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

    char commandName[] = "curl -o ";
    int commandLn =  strlen(commandName) + strlen(filename) + strlen(address) + 1;
    char command[commandLn];
    command[0] = 0;

    printf("\nDownload ---> %s\n\n", address);
    
    strcat(command, commandName);
    strcat(command, filename);
    strcat(command, " ");
    strcat(command, address);
    puts(command);  
 
    if( !(pipe = popen(command, "r")) ) {
        exit(1);
    } 

    pclose(pipe);
}





