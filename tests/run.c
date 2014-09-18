#include "run.h"


int main() {
	loadTests();
	return 0;
}


/* Helper: add to path new directory */
char* pathConcater(char* right, char* left) {
	char* path = (char*)malloc(strlen(right) + strlen(left) + 1);
	strcat(path, right);
	strcat(path, "/");
	strcat(path, left);
	
	return path;
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
	char* testDirName = pathConcater(testRoot, name);

	//Loop all tags
	DIR* TAG = opendir(testDirName);
	
	if( TAG != NULL ) {
		struct dirent* tagDirent;
		int tagsIterator = 0;

		while( (tagDirent = readdir(TAG)) != NULL ) {
			if( tagsIterator > 1 ) {
				char* tagName = tagDirent->d_name;
				char* tagPath = pathConcater(testDirName, tagName);
				
				//Loop test .in and .out files
				DIR* SOURCE = opendir(tagPath);

				if( SOURCE != NULL ) {
					int filesIterator = 0;
					struct dirent* sourceDirent;

					while( (sourceDirent = readdir(SOURCE)) != NULL ) {
						if( filesIterator > 1 ) {
							char* pathToFile = pathConcater(tagPath, sourceDirent->d_name);

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

	free(testDirName);
}
