#include "run.h"


int main() {
	loadTests();
	return 0;
}


/* Helper: add to path new directory */
char* pathConcater(char* right, char* left) {
	char* path = (char*)malloc(strlen(right) + strlen(left) + 1);
	path[0] = 0;
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
					char** tags;
					char* inResult = (char*)malloc(0);
					char* outResult = (char*)malloc(0);

					while( (sourceDirent = readdir(SOURCE)) != NULL ) {
						if( filesIterator > 1 ) {
							char* pathToFile = pathConcater(tagPath, sourceDirent->d_name);

							//Get array of tags
							if( strcmp(sourceDirent->d_name, "source.in") == 0  ) {
								tags = HTMLgetTags(pathToFile, tagName);
								
								int arrLn = sizeof(tags) / sizeof(char*) + 1;
								int totalLn = 0;
								
								for( int i = 0; i < arrLn; i++ ) {
									totalLn += strlen(tags[i]);
									inResult = (char*)realloc(inResult, totalLn);
									strcat(inResult, tags[i]);
									strcat(inResult, "\n");
								}
							} 
							
							//Get text .of out file
							else if( strcmp(sourceDirent->d_name, "source.out") == 0  ) {
								outResult = fileToString(pathToFile);					
								free(pathToFile);
							}
						}
						filesIterator++;
					}

					if( strlen(inResult) > 0 & strlen(outResult) > 0 ){
						if( strcmp(inResult, outResult) == 0 ) {
							printf("Test 'getTags' for tag '%s', status: SUCCESS.\n", tagName);
						} else {
							printf("Test 'getTags' for tag '%s', status: FAIL!!!\n", tagName);
						}
					} else {
						printf("ERROR: empty source files\n");
					}	

					free(inResult);
					free(outResult);
					closedir(SOURCE);
				}
			}

			tagsIterator++;
		}
	}

	free(testDirName);
}
