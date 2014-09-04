#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


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
void getImage(char* src);
char* getFileNameFromSrc(char* _src);

