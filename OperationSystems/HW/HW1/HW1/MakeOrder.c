#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

#define BUFFER_SIZE 256

typedef struct Dish {
	char name[BUFFER_SIZE];
	int amount;
	int price;
} Dish;

void parseDish(char[BUFFER_SIZE], Dish*, char[BUFFER_SIZE]);
void removeEnding(char*);
char* trim(char*);
int getTotalPrice(Dish[BUFFER_SIZE], int);
void printDate(int);
void Error(char*);

// argv[1] shuold be resturant name
// argv[2] shuold be customer name
int main(int argc, char* argv[]) {
	int fd_to, wbytes, i;
	char resFileName[BUFFER_SIZE];
	char orderFilePath[BUFFER_SIZE];
	char orderInfo[BUFFER_SIZE] = { '\0' };
	char buffer[BUFFER_SIZE] = { '\0' };
	Dish orderDishes[BUFFER_SIZE];
	Dish* currDishPtr;
	int dishesAmount = 0;
	int totalPrice = 0;

	if (argc != 3) {
		fprintf(stderr, "Error: Invalid given arguments, should exactly two given\n");
		return(1);
	}

	// adding file postfix (BBB -> BBB.txt)
	sprintf(resFileName, "%s.txt", argv[1]);
	// checking only if restaurant is exist.
	if (open(resFileName, O_RDONLY) == -1) Error("Restaurant Not Found!");

	// get order input
	fprintf(stdout, "Insert your order (Finish to finish):\n");
	while (1) {
		fgets(orderInfo, BUFFER_SIZE, stdin);
		// trim all white spaces and '\n' in the end
		strcpy(orderInfo, trim(orderInfo));

		// stop getting order from user
		if (strcmp(orderInfo, "Finish") == 0) break;
		
		currDishPtr = &(orderDishes[dishesAmount]);
		parseDish(orderInfo, currDishPtr, argv[1]);

		// if item doesn't exists in the given restaurant.
		// the relevant error message will be shown by getPrice procces
		if (currDishPtr->price == -1) Error("Order is canceled");

		dishesAmount++;
	}
	// calculating total price
	totalPrice = getTotalPrice(orderDishes, dishesAmount);
	// getting confirm/cancel order from user
	fprintf(stdout, "Total Price: %d NIS (Confirm to approve/else cancel)\n", totalPrice);
	fgets(orderInfo, BUFFER_SIZE, stdin);
	// trim all white spaces and '\n' in the end
	strcpy(orderInfo, trim(orderInfo));

	// canceling order if not Confirmed 
	if (strcmp(orderInfo, "Confirm") != 0) Error("Order is canceled!");

	// if order is confirmed creating new order file
	sprintf(orderFilePath, "%s_Order/%s.txt", argv[1], argv[2]);
    if ((fd_to = open(orderFilePath, O_WRONLY | O_CREAT, 0664)) == -1) {
        perror("create/open order"); return -1;
    }

	// writing title to order file
	sprintf(buffer, "%s Order\n\n", argv[1]);
	if ((wbytes = write(fd_to, buffer, strlen(buffer))) == -1) {
		perror("write order title"); return -1;
	}
	// writing dish orders to file
	for (i = 0; i < dishesAmount; i++) {
		sprintf(buffer, "%s %d\n", orderDishes[i].name, orderDishes[i].amount);
		if ((wbytes = write(fd_to, buffer, strlen(buffer))) == -1) {
			perror("write order"); return -1;
		}
	}
	// writing total price to file
	sprintf(buffer, "Total Price: %d NIS\n\n", totalPrice);
	if ((wbytes = write(fd_to, buffer, strlen(buffer))) == -1) {
		perror("write order"); return -1;
	}
	printDate(fd_to);

	// changing file to be read-only
	chmod(orderFilePath, 0444);
	fprintf(stdout, "Order created!");
	close(fd_to);
	return 0;
}

void parseDish(char dishStr[BUFFER_SIZE], Dish* dish, char restName[BUFFER_SIZE]) {
	char* commend = "getPrice";
	char* commendPath = "./getPrice";
	int returnStatus = -1;
	int i = strlen(dishStr) - 1;
	// running backwards until meeting space.
	while (i >= 0 && dishStr[i] != ' ') i--;

	dish->amount= atoi(dishStr + i);
	// terminating string at the end of name
	dishStr[i] = '\0';
	strcpy(dish->name, dishStr);

	// calling getPrice process to recive price of dish
	switch (fork()) {
	// if fork failed
	case -1:
		perror("fork"); exit(-1);
	// Only child execute this
	case 0:
		// executing getPrice commend
		execl(commendPath, commend, restName, dish->name, 0);
		// if child has not done any commend 
		fprintf(stderr, "getPrice on %s in %s has failed\n", dish->name, restName);
		exit(-1);
	// Only parent execute this
	default:
		// getPrice process returns price of dish as returnStatus
		waitpid(-1, &returnStatus, 0);
		returnStatus = WEXITSTATUS(returnStatus);
		// if returnStatus is 255 (equal to -1 error), changing it to -1.
		if (returnStatus == 255) returnStatus = -1;
		dish->price = returnStatus;
	}
}

char* trim(char* str) {
	int i;
	removeEnding(str);
	// remove spaces in the beginning
	while (*str == ' ' || *str == '\t') str++;
	// remove spaces in the end
	i = strlen(str) - 1;
	while (*(str + i) == ' ' || *(str + i) == '\t') i--;
	*(str + (i + 1)) = '\0';

	return str;
}

void removeEnding(char* str) {
	str[strlen(str) - 1] = '\0';
}

int getTotalPrice(Dish orderDishes[BUFFER_SIZE], int orderSize) {
	int i, sum = 0;
	for (i = 0; i < orderSize; i++) 
		sum += orderDishes[i].price * orderDishes[i].amount;

	return sum;
}

void printDate(int fd_to) {
	int day, month, year;
	struct tm* local;
	time_t now;
	char buffer[BUFFER_SIZE];

	// Obtain current time
	// `time()` returns the current time of the system as a `time_t` value
	time(&now);

	// localtime converts a `time_t` value to calendar time and
	// returns a pointer to a `tm` structure with its members
	// filled with the corresponding values
	local = localtime(&now);

	day = local->tm_mday;           // get day of month (1 to 31)
	month = local->tm_mon + 1;      // get month of year (0 to 11)
	year = local->tm_year + 1900;   // get year since 1900

	// writing the current date to file
	sprintf(buffer, "%02d/%02d/%d\n", day, month, year);
	if (write(fd_to, buffer, strlen(buffer)) == -1) {
		perror("write date"); return(-1);
	}
}

void Error(char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(-1);
}