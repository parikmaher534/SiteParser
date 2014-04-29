#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>


/* Global varibles */
char *site = NULL;
char *dirName = NULL;

/* Define functions */
void parseError(void *ctx);
void getSite(void);
void getSource(char* address, char *filename);
void parseDocument(xmlNode *node);
void getImage(xmlChar *src);

char *createSiteDirectories();

