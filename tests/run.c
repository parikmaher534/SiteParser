#include "run.h"


int main() {
	loadTests();
	return 0;
}


/* Tests loader */
void loadTests() {
	char testsPath[] = "parser";
	DIR* TESTS_DIR = opendir(testsPath);

	if( TESTS_DIR != NULL ) {
		
		struct dirent* pDirent;
		int dirIterator = 0;
		
		//Loop all tests dirs
		while( (pDirent = readdir(TESTS_DIR)) != NULL ){
			if( dirIterator > 1 ) {

				if( strcmp(pDirent->d_name, "getTags") == 0 ) {
					TEST_getTags(testsPath, pDirent->d_name);
				} else {
					printf("Unknown Test. %s\n", pDirent->d_name);	
				}
			}
			dirIterator++;
		}
	}
};

/* Test parser on tag detection */
void TEST_getTags(char* testRoot, char* name) {
	char testDirName[strlen(testRoot) + strlen(name) + 1];
	testDirName[0] = 0;
	strcat(testDirName, testRoot);
	strcat(testDirName, "/");
	strcat(testDirName, name);

	//Loop all tags
	DIR* TAG = opendir(testDirName);
	
	if( TAG != NULL ) {
		struct dirent* tagDirent;
		int tagsIterator = 0;

		while( (tagDirent = readdir(TAG)) != NULL ) {
			if( tagsIterator > 1 ) {
				char* tagName = tagDirent->d_name;
				char tagPath[strlen(testDirName) + strlen(tagName) + 1];
				tagPath[0] = 0;
				strcat(tagPath, testDirName);
				strcat(tagPath, "/");
				strcat(tagPath, tagName);

				//Loop test .in and .out files
				DIR* SOURCE = opendir(tagPath);

				if( SOURCE != NULL ) {
					int filesIterator = 0;
					struct dirent* sourceDirent;

					while( (sourceDirent = readdir(SOURCE)) != NULL ) {
						if( filesIterator > 1 ) {
							char pathToFile[strlen(tagPath) + strlen(sourceDirent->d_name) + 1];
							pathToFile[0] = 0;
							strcat(pathToFile, tagPath);
							strcat(pathToFile, "/");
							strcat(pathToFile, sourceDirent->d_name);
							
							printf("Source: %s\n", sourceDirent->d_name);
							//TODO: this;
						}
						filesIterator++;
					}
				}
			}
			tagsIterator++;
		}
	}
}
