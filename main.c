#include "main.h"
#include "src/helpers.c"


int main(int argc, char *argv[]) {

    //Redefine parse error handler
    xmlSetGenericErrorFunc(NULL, (void *)parseError);

    /* First argument must be the site URL */
    if( argv[1] ) {
        site = argv[1];
        getSite();
    } else {
        printf("\nPlease set site URL.\n");
    }

    return 0;
}

void getSite() {

    //Create own directory for current site
    siteDirData = createSiteDirectories();

    char pathToHTML[strlen(siteDirData -> dirName) + strlen(indexPage)];
    pathToHTML[0] = 0;
    strcat(pathToHTML, siteDirData -> dirName);
    strcat(pathToHTML, indexPage);

    getSource(site, pathToHTML);

    xmlDoc *doc = htmlReadFile(pathToHTML, "UTF-8", 0);
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
    int imageSourceLn = strlen(site) + strlen("/") + strlen(src);
    char path[imageSourceLn];
    path[0] = 0;

    /* If image from another domain or has a absolute path */
    if( hasProtocol(src) ) {
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

    /* Get image path */
    char imgPath[strlen(siteDirData -> dirImgName) + strlen(imageName) + 1];
    strcat(imgPath, siteDirData -> dirImgName);
    strcat(imgPath, "/");
    strcat(imgPath, imageName);
    
    getSource(path, imgPath);
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



struct siteDir *createSiteDirectories(void) {
    char *url = malloc(strlen(site));
    strcpy(url, site);

    char *result = strtok(url, "://");
    result = strtok(NULL, "://");
    mkdir(result, 0000755);

    char *imgDirName = "/images";
    char *imgDir = (char*)malloc(strlen(result) + strlen(imgDirName));
    strcpy(imgDir, result);
    strcat(imgDir, imgDirName);
    mkdir(imgDir, 0000755);

    struct siteDir *siteDirStruct;
    siteDirStruct = (struct siteDir *) malloc(sizeof(struct siteDir));
    siteDirStruct -> dirName = result;
    siteDirStruct -> dirImgName = imgDir;

    return siteDirStruct;    
}


//TODO: write to error log
void parseError(void *ctx) { };
