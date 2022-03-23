#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>

#define BUFFER_SIZE 256

int isTextFile(struct dirent*);
void Error(char*);

/* argv[1] shuold be resturant name
   prints num of orders of specipied resturant */
int main(int argc, char* argv[]) {
	int numOfFiles = 0;
	char resOrderDir[BUFFER_SIZE];
	struct dirent* dp;
	DIR* fd;

	if (argc != 2) Error("Invalid given arguments, should exactly one given\n");

	sprintf(resOrderDir, "./%s_Order", argv[1]);
	// opening order directory
	if ((fd = opendir(resOrderDir)) == NULL) {
		fprintf(stderr, "Resturant %s doesn't exists\n", argv[1]);
		return -1;
	}
	while ((dp = readdir(fd)) != NULL) {
		// checking if file is a text file => inc counter
		if (isTextFile(dp)) numOfFiles++;
	}
	closedir(fd);
	// printing num of orders
	printf("%d Orders\n", numOfFiles);
	return 0;
}

int isTextFile(struct dirent* dp) {
	char* expectedExt = ".txt";
	char* actualExt;

	actualExt = strstr(dp->d_name, expectedExt);
	// checking if the file is a .txt file
	if (actualExt != NULL && strlen(actualExt) == strlen(expectedExt))
		return 1;
	return 0;
}

void Error(char* msg) {
	fprintf(stderr, "Error:%s\n", msg);
	exit(1);
}