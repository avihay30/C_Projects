#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFFER_SIZE 256
#define DOTS ".............................."
#define DOTS_LENGTH 30

typedef struct Dish {
    char name[BUFFER_SIZE];
    int price;
} Dish;

void Error(char*);
void removeEnding(char*);
void handleUserDishNames(int);
void parseDish(char[BUFFER_SIZE], Dish*);
int getNamePriceSpace(int);
char* trim(char*);

// argv[1] shuold be resturant name
// argv[2] num of items to be inserted
int main(int argc, char* argv[]) {
    int i;
    int fd_to, wbytes;
    int numOfItems;
    char dishTypeChar[2] = { '\0' };
    char resturantFileName[BUFFER_SIZE] = { '\0' };
    char buffer[BUFFER_SIZE] = { '\0' };
    char bufToWrite[BUFFER_SIZE] = { '\0' };

    if (argc != 3) Error("Invalid given arguments, should exactly two given\n");
    
    numOfItems = atoi(argv[2]);
    strcpy(resturantFileName, argv[1]);
    strcat(resturantFileName, ".txt");
    if ((fd_to = open(resturantFileName, O_WRONLY | O_CREAT, 0664)) == -1) {
        perror("create/open manu"); return(-1);
    }
    
    // writing title on file (BBB Manu)
    strcpy(bufToWrite, argv[1]);
    strcat(bufToWrite, " Menu\n");
    if ((wbytes = write(fd_to, bufToWrite, strlen(bufToWrite))) == -1) {
        perror("write resturant name"); return(-1);
    }

    for (i = 0; i < numOfItems; i++) {
        dishTypeChar[0] = 'a' + i;
        fprintf(stdout, "Insert Type Dish %c:\n", dishTypeChar[0]);
        // get dish type input
        fgets(buffer, BUFFER_SIZE, stdin);

        // adding prefix data to be written (e.g "a. ")
        sprintf(bufToWrite, "\n%s. %s", dishTypeChar, buffer);

        // write dish type
        if ((wbytes = write(fd_to, bufToWrite, strlen(bufToWrite))) == -1) {
            perror("write dish type"); return(-1);
        }

        handleUserDishNames(fd_to);
    }

    // write Bon Appetit
    strcpy(bufToWrite, "\n\t\t\tBon Appetit\n");
    if ((wbytes = write(fd_to, bufToWrite, strlen(bufToWrite))) == -1) {
        perror("write Bon Appetit"); return(-1);
    }

    // create oreders directory
    strcpy(buffer, argv[1]);
    strcat(buffer, "_Order");
    if (mkdir(buffer, 0777) == -1) {
        perror("create orders folder"); return(-1);
    }

    fprintf(stdout, "Successfully created\n");
    close(fd_to);
    return 0;
}

void handleUserDishNames(int fd_to) {
    int numOfDish = 1;
    int wbytes, spacer;
    char dishName[BUFFER_SIZE] = { '\0' };
    char dishNameWithDots[BUFFER_SIZE] = { '\0' };
    char dishBuffer[BUFFER_SIZE] = { '\0' };
    Dish dish;

    while (1) {
        // get dish name and price input
        fprintf(stdout, "Insert dish name %d:\n", numOfDish);
        fgets(dishName, BUFFER_SIZE, stdin);
        // trim all white spaces and '\n' in the end
        strcpy(dishName, trim(dishName));

        // stop getting dishs from user
        if (strcmp(dishName, "Stop") == 0) return;
        
        parseDish(dishName, &dish);
        // setting variable of name combined with dots to print
        sprintf(dishNameWithDots, "%s %s", dish.name, DOTS);

        spacer = getNamePriceSpace(dish.price);
        sprintf(dishBuffer, "  %.*s %dNIS\n", spacer, dishNameWithDots, dish.price);

        // write dish name and price
        if ((wbytes = write(fd_to, dishBuffer, strlen(dishBuffer))) == -1) {
            perror("write dish name and price"); exit(-1);
        }

        numOfDish++;
    }
}

void parseDish(char dishStr[BUFFER_SIZE], Dish* dish) {
    int i = strlen(dishStr) - 1;
    // running backwards until meeting space.
    while (i >= 0 && dishStr[i] != ' ') i--;

    dish->price = atoi(dishStr + i);
    // terminating string at the end of name
    dishStr[i] = '\0';
    strcpy(dish->name, dishStr);
}

// calculate the amount of space required between the name and price
int getNamePriceSpace(int price) {
    int numOfDigits = 1;
    while (price > 9) {
        price /= 10;
        numOfDigits++;
    }

    return DOTS_LENGTH - numOfDigits;
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

void Error(char* msg) {
    fprintf(stderr, "Error:%s\n", msg);
    exit(1);
}
