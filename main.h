#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>


extern int errno ;


/* Global varibles */
struct siteDir *siteDirData; 
char* site = NULL;
const char* indexPage = "/index.html";

/* Define structures */
struct siteDir {
    char* dirName;
    char* dirImgName;
};

/* Define functions */
struct siteDir *createSiteDirectories();
void parseError(void *ctx);
void getSite(void);
void getSource(char* address, char* filename);
void parseDocument(xmlNode *node, int index);
void getImage(xmlChar *src);

