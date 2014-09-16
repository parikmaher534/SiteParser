#include "main.h"
#include "lib/helpers.c"
#include "lib/parser.c"


int main(int argc, char *argv[]) {

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

	char** tagsArr = HTMLgetTags(pathToHTML, "img");
	int tagsAmount = sizeof(tagsArr) / sizeof(char*) + 1;

	//Loop all images
	for( int i = 0; i <= tagsAmount; i++ ) {
		char* attr = HTMLgetAttr(tagsArr[i], "src"); 
        getImage(attr);
	}
}

void getImage(char* src) {
    int imageSourceLn = strlen(site) + strlen(src);
    char path[imageSourceLn];
    
	path[0] = 0;
	strcat(path, site);
	if( strncmp(&src[0], "/", 1) != 0 ) strcat(path, "/");
	strcat(path, src);

	char* filename = getFileNameFromSrc(src);
	
	/* Get image path */
    char *imgPath = malloc(strlen(siteDirData -> dirImgName) + strlen(filename) + 1);
    strcat(imgPath, siteDirData -> dirImgName);
    strcat(imgPath, "/");
    strcat(imgPath, filename);

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
	
	//Get site url splited by '/'. Loop them and create sub directories
	char** splitedUrl = splitUrlBySlash(site);

	//TODO: Create sub directories;

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


char* getFileNameFromSrc(char* _src) {
	char src[strlen(_src)];
	src[0] = 0;
	
	strcat(src, _src);

	char* strToc = strtok(src, "/");
	char* fileName = (char*)malloc(1);

	while(strToc != NULL) {
		fileName = (char*)realloc(fileName, strlen(strToc) + 1);
		strcpy(fileName, strToc);
		strToc = strtok(NULL, "/");
	}

	return fileName;
}

