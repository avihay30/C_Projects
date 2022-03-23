#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFFER_SIZE 256
/* max size of row in the menu,
   will be used for going backwards (lseek) in file (in case of half reading a row). */
#define MAX_ROW_LEN 100

int getRowPrice(char*);
void Error(char*);

/* argv[1] should be restaurant name
   argv[2] should be dish name
   returns the specified restaurant's dish price */
int main(int argc, char* argv[]) {
	int fd_from, rbytes;
	int totalRead = 0, dishPrice = -1;
	char resFileName[BUFFER_SIZE];
	char buff[BUFFER_SIZE];
	char dishToSearch[BUFFER_SIZE];
	char* dishRow;

	if (argc != 3) {
		fprintf(stderr, "Error: Invalid given arguments, should exactly two given\n");
		return -1;
	}

	// adding filename extension (BBB -> BBB.txt)
	sprintf(resFileName, "%s.txt", argv[1]);
	if ((fd_from = open(resFileName, O_RDONLY)) == -1) Error("Restaurant Not Found!");

	// reading from opened restaurant menu file
	do {
		if ((rbytes = read(fd_from, buff, BUFFER_SIZE)) == -1) {
			perror("read restaurant menu"); return -1;
		}
		totalRead += rbytes;
		// creating a search string (based on the format of CreateMenu)
		sprintf(dishToSearch, "  %s .", argv[2]);
		dishRow = strstr(buff, dishToSearch);
		// row was found
		if (dishRow != NULL) {
			dishPrice = getRowPrice(dishRow);
			if (dishPrice != -1) { // price was found
				close(fd_from);
				return dishPrice;
			}
		}
		// if row wasn't found and/or price, going backwards in case of half reading
		lseek(fd_from, totalRead - MAX_ROW_LEN, SEEK_SET);
	} while (rbytes > 0);

	close(fd_from);
	Error("dish Not Found!");
}

int getRowPrice(char* dishRow) {
	int i;
	char* endOfDish = strstr(dishRow, "NIS\n");
	if (endOfDish == NULL) return -1;
	// terminating the dishRow at the first occurrence of NIS
	// => dishRow points on only one row.
	*endOfDish = '\0';

	// iterating backwards (from price) until meeting space (end of price)
	i = strlen(dishRow) - 1;
	while (dishRow[i] != ' ') i--;

	// dishRow will now point only on the price string
	dishRow += i + 1;
	return atoi(dishRow);
}

void Error(char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(-1);
}