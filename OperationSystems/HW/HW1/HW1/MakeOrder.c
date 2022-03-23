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

void getUserOrder(char*, Dish[BUFFER_SIZE], int*);
void parseDish(char[BUFFER_SIZE], Dish*, char[BUFFER_SIZE]);
void removeNewLine(char*);
char* trim(char*);
int getTotalPrice(Dish[BUFFER_SIZE], int);
void writeTitle(int, char*);
void writeDishOrders(int, Dish[BUFFER_SIZE], int);
void writePrice(int, int);
void writeDate(int);
void assertRestaurantExists(char*);
void assertConfirmation(int);
void Error(char*);

/* argv[1] should be restaurant name
   argv[2] should be customer name 
   user enters an order of some given restaurant */
int main(int argc, char* argv[]) {
	int fd_to, dishesAmount = 0, totalPrice = 0;
	char orderFilePath[BUFFER_SIZE];
	Dish orderDishes[BUFFER_SIZE];

	if (argc != 3) {
		fprintf(stderr, "Error: Invalid given arguments, should exactly two given\n");
		return(1);
	}

	assertRestaurantExists(argv[1]);
	// getting order info from user and insert it into `orderDishes` array and amount
	getUserOrder(argv[1], orderDishes, &dishesAmount);
	// calculating total price
	totalPrice = getTotalPrice(orderDishes, dishesAmount);
	assertConfirmation(totalPrice);

	// if order is confirmed creating new order file
	sprintf(orderFilePath, "%s_Order/%s.txt", argv[1], argv[2]);
    if ((fd_to = open(orderFilePath, O_WRONLY | O_CREAT, 0664)) == -1) {
        perror("create/open order"); return -1;
    }

	writeTitle(fd_to, argv[1]);
	writeDishOrders(fd_to, orderDishes, dishesAmount);
	writePrice(fd_to, totalPrice);
	writeDate(fd_to);

	// changing file to be read-only
	chmod(orderFilePath, 0444);
	printf("Order created!\n");
	close(fd_to);
	return 0;
}

// getting order info from user and insert it into `orderDishes` array and amount
void getUserOrder(char* restName, Dish orderDishes[BUFFER_SIZE], int* dishesAmount) {
	char buffer[BUFFER_SIZE] = { '\0' };
	Dish* currDishPtr;

	// get order input
	printf("Insert your order (Finish to finish):\n");
	while (1) {
		fgets(buffer, BUFFER_SIZE, stdin);
		// trim all white spaces and '\n' in the end
		strcpy(buffer, trim(buffer));

		// stop getting order from user
		if (strcmp(buffer, "Finish") == 0) break;

		currDishPtr = &(orderDishes[*dishesAmount]);
		parseDish(buffer, currDishPtr, restName);

		// if item doesn't exists in the given restaurant.
		// the relevant error message will be shown by getPrice procces
		if (currDishPtr->price == -1) Error("Order is canceled");

		(*dishesAmount)++;
	}
}

/* parsingand getting price by calling getPrice program
   to retrive the price by the returned status */
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

	// calling getPrice process to receive price of dish
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
		// if returnStatus is 255 (equal to -1 error), reseting it to -1.
		if (returnStatus == 255) returnStatus = -1;
		dish->price = returnStatus;
	}
}

// trim all white spaces before and after the given string
char* trim(char* str) {
	int i;
	removeNewLine(str);
	// remove spaces in the beginning
	while (*str == ' ' || *str == '\t') str++;
	// remove spaces in the end
	i = strlen(str) - 1;
	while (*(str + i) == ' ' || *(str + i) == '\t') i--;
	*(str + (i + 1)) = '\0';

	return str;
}

// removes \n in the end of the string
void removeNewLine(char* str) {
	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';
}

// calculating total price on all dishes
int getTotalPrice(Dish orderDishes[BUFFER_SIZE], int orderSize) {
	int i, sum = 0;
	for (i = 0; i < orderSize; i++) 
		sum += orderDishes[i].price * orderDishes[i].amount;

	return sum;
}

// writing title to order file
void writeTitle(int fd_to, char* restaurantName) {
	char buffer[BUFFER_SIZE] = { '\0' };

	sprintf(buffer, "%s Order\n\n", restaurantName);
	if (write(fd_to, buffer, strlen(buffer)) == -1) {
		perror("write order title"); exit(-1);
	}
}

// writing dish orders to file
void writeDishOrders(int fd_to, Dish orderDishes[BUFFER_SIZE], int dishesAmount) {
	int i;
	char buffer[BUFFER_SIZE] = { '\0' };

	for (i = 0; i < dishesAmount; i++) {
		sprintf(buffer, "%s %d\n", orderDishes[i].name, orderDishes[i].amount);
		if (write(fd_to, buffer, strlen(buffer)) == -1) {
			perror("write order"); exit(-1);
		}
	}
}

// writing total price to file
void writePrice(int fd_to, int totalPrice) {
	char buffer[BUFFER_SIZE] = { '\0' };

	sprintf(buffer, "Total Price: %d NIS\n\n", totalPrice);
	if (write(fd_to, buffer, strlen(buffer)) == -1) {
		perror("write order"); exit(-1);
	}
}

void writeDate(int fd_to) {
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
		perror("write date"); exit(-1);
	}
}

/* checking if Restaurant is exists by trying to open Restaurant menu file
   if not, printing an error and exit program */
void assertRestaurantExists(char* restaurantName) {
	char resFileName[BUFFER_SIZE];
	// adding file postfix (BBB -> BBB.txt)
	sprintf(resFileName, "%s.txt", restaurantName);
	// checking only if restaurant is exists.
	if (open(resFileName, O_RDONLY) == -1) Error("Restaurant Not Found!");
}

/* asking user for Confirmation,
   if not confirmed prompt cancel message and exit program */
void assertConfirmation(int totalPrice) {
	char buffer[BUFFER_SIZE] = { '\0' };
	// getting confirm/cancel order from user
	printf("Total Price: %d NIS (Confirm to approve/else cancel)\n", totalPrice);
	fgets(buffer, BUFFER_SIZE, stdin);
	// trim all white spaces and '\n' in the end
	strcpy(buffer, trim(buffer));

	// canceling order if not Confirmed 
	if (strcmp(buffer, "Confirm") != 0) Error("Order is canceled!");
}

void Error(char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(-1);
}