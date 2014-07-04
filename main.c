#include "main.h"
#include "src/helpers.c"

void splitAddressBySpash(char* address){
	printf("Input address: %s\n", address);

	char* urlWithoutProtocol = _cutProtocol(address); 
	
	printf("Normal Url: %s\n", urlWithoutProtocol);
   

	//char* normalUrl = _normalizeUrl(urlWithoutProtocol);
	
	//printf("Normal Url: %s\n", normalUrl);
	
	free(urlWithoutProtocol);
};

int main(int argc, char *argv[]) {

    //Redefine parse error handler
    xmlSetGenericErrorFunc(0, (void *)parseError);

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

    parseDocument(element, 0);
}

void parseDocument(xmlNode *node, int index) {
    const char *img = "img";
    xmlChar src[] = "src";

    if( strncmp((char *)node->name, img, 2) == 0 ) {
         getImage(xmlGetProp(node, src));
    }
    
    while(node->next) {

        //Ignore inline scripts
        if(strncmp((char *)node->name, "script", 5) == 0 ) {
            if( xmlGetProp(node, src) == 0 ) node = node->next;
        } 

        if( node->children ) {
            parseDocument(node->children, ++index);
        }

        node = node->next;
     
        if( strncmp((char *)node->name, img, 2) == 0 ) {
            getImage(xmlGetProp(node, src));
        }
    }

}


void getImage(xmlChar *_src) {
    char *src = (char *)_src;
    char* _site = _normalizeUrl(site);
  
    int imageSourceLn = strlen(_site) + strlen("/") + strlen(src);
    char path[imageSourceLn];
    path[0] = 0;

    /* If image from another domain or has a absolute path */
    if( _hasProtocol(src) ) {
        strcat(path, src);
    } else {
        strcat(path, _site);

        if( strncmp(&src[0], "/", 1) == 0 ) {
            strcat(path, &src[1]);
        } else {
            strcat(path, src);
        }
    }

    /* Get image name from src */
    char *srcToc = NULL;
    char *imageName = NULL;
 
    srcToc = strtok(src, "/");

    while( srcToc != NULL ) {
        imageName = srcToc;
        srcToc = strtok(NULL, "/");
    }
    
    /* Get image path */
    char *imgPath = malloc(strlen(siteDirData -> dirImgName) + strlen(imageName) + 1);
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
    char* url = malloc(strlen(site));
    strcpy(url, site);

	splitAddressBySpash(site);

    char* dirName = strtok(url, "://");
    dirName = strtok(0, "/");
    mkdir(dirName, 0000755);

    char* imgDirName = "/images";
    char* imgDir = (char*)malloc(strlen(dirName) + strlen(imgDirName));
    strcpy(imgDir, dirName);
    strcat(imgDir, imgDirName);
    mkdir(imgDir, 0000755);

    struct siteDir *siteDirStruct;
    siteDirStruct = (struct siteDir *) malloc(sizeof(struct siteDir));
    siteDirStruct -> dirName = dirName;
    siteDirStruct -> dirImgName = imgDir;

    return siteDirStruct;    
}


//TODO: write to error log
void parseError(void *ctx) { };
