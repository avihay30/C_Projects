/*
* Filename: myasm_extended.c
* Gets some <name>.asm file and creates a converted <name>.hack file (binary representation).
* The Program is extended, can accept symbols (words) in A instructions.
* According to requirements of Nand2Tetris course.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ALLOC_ERR "AlloctionError: The program couldn't allocate memory!\n"
#define FILE_ERR "FileError: The program couldn't create/read file!\n"
#define FILE_NAME_SIZE 20
#define ASM_EXTENTION_LENGTH 4
#define ASM_FILE_SIZE 300
#define INS_FIELD_MAX_SIZE 3
#define BIT_SIZE 16

typedef enum { FALSE, TRUE } Bool;
typedef enum { A_INSTRUCTION, C_INSTRUCTION } InstractionType;

// struct for holding asmLine data
// dest, comp, jump have max of 3 chars.
typedef struct asmLine {
	InstractionType insType;
	char symbol[ASM_FILE_SIZE];
	char dest[INS_FIELD_MAX_SIZE + 1];
	char comp[INS_FIELD_MAX_SIZE + 1];
	char jump[INS_FIELD_MAX_SIZE + 1];

} asmLine;

// struct for holding CompTable row data (with a value in bin)
typedef struct compAndBin {
	char comp[INS_FIELD_MAX_SIZE + 1];
	char bin[8];
} compAndBin, *pCompAndBin;

// struct for holding destTable row data
typedef struct destAndBin {
	char dest[INS_FIELD_MAX_SIZE + 1];
	char bin[4];
} destAndBin, * pDestAndBin;

// struct for holding jumpTable row data
typedef struct jumpAndBin {
	char jump[INS_FIELD_MAX_SIZE + 1];
	char bin[4];
} jumpAndBin, * pJumpAndBin;

// struct for holding SymbolTable row data
typedef struct symbolAddr {
	char symbol[ASM_FILE_SIZE];
	int address;
} symbolAddr, *pSymbolAddr;

/* Util functions */
char* trim(char*);
char* trimInline(char*);
char* trimTrailingComments(char*);
void resetBinaryStr(char*);
Bool isSymbolIsNumber(char*);
void convertDecimalToBin(int, char*);
void checkAllocation(void*, char*);
void getOutputFileName(char*, char*);

/* Assembler API */
// Parser "module"
InstractionType getInstractionType(char);
void getSymbol(char* source, char* dest);
void getDest(char* source, char* dest);
void getComp(char* source, char* dest);
void getJump(char* source, char* dest);

// Code "module" - returns binary representation
void initCompTable(pCompAndBin*);
void initDestTable(pDestAndBin*);
void initJumpTable(pJumpAndBin*);
void convertCIntsToBin(pDestAndBin, pCompAndBin, pJumpAndBin, asmLine*, char*);

// SymbolTable "module"
void initTable(pSymbolAddr*);
void addEntry(pSymbolAddr, char*, int*);
Bool contains(pSymbolAddr, int, char*);
int getAddress(pSymbolAddr, int, char*);

// Function returns which instraction ins is
InstractionType getInstractionType(char ins) {
	if (ins == '@')
		return A_INSTRUCTION;
	return C_INSTRUCTION;
}

// Function assigns to dest the parameter name
void getSymbol(char* source, char* dest) {
	strcpy(dest, source + 1);
}

// Function assigns to dest the Dest field
void getDest(char* source, char* dest) {
	int i;
	for (i = 0; i < INS_FIELD_MAX_SIZE && source[i] != '='; i++) {
		dest[i] = source[i];
	}
	dest[i] = '\0';
	
	// if the value is not stored (no equal sign)
	if (i == INS_FIELD_MAX_SIZE && source[i] != '=') {
		strcpy(dest, "0");
	}
}

// Function assigns to dest the Comp field
void getComp(char* source, char* dest) {
	int i = 0, j = 0;
	while (source[i] != '=' && i < INS_FIELD_MAX_SIZE + 1)
		i++;
	// there is no equal sign -> reseting i
	if (i == INS_FIELD_MAX_SIZE + 1)
		i = -1;
	for (i++; j < INS_FIELD_MAX_SIZE && source[i] != ';'; i++, j++) {
		dest[j] = source[i];
	}
	dest[j] = '\0';
}

// Function assigns to dest the Jump field
void getJump(char* source, char* dest) {
	char* temp;
	temp = strstr(source, ";");
	// if there was not jump field (';' doesn't exists or it's in the end)
	if (temp == NULL || strlen(temp+1) == 0) {
		dest[0] = '0';
		dest[1] = '\0';
	}
	// there is a jump field, temp points on ';'
	else {
		strcpy(dest, temp + 1);
	}
}

// Creates and initializes a predifiend CompTable (there are 28 values)
void initCompTable(pCompAndBin *compTable) {
	int i;
	// comp and bin are ordered, comp[i] -> bin[i]
	char comp[28][4] = { 
		"0", "1", "-1", "D", "A", "!D", "!A", "-D", "-A",
		"D+1", "A+1", "D-1", "A-1", "D+A", "D-A", "A-D", "D&A", "D|A",
		"M", "!M", "-M", "M+1", "M-1", "D+M", "D-M", "M-D", "D&M", "D|M"
	};
	char bin[28][8] = {
		"0101010", "0111111", "0111010", "0001100", "0110000", "0001101", "0110001", "0001111", "0110011",
		"0011111", "0110111", "0001110", "0110010", "0000010", "0010011", "0000111", "0000000", "0010101",
		"1110000", "1110001", "1110011", "1110111", "1110010", "1000010", "1010011", "1000111", "1000000", "1010101"
	};

	// predifiend, will hold 28 different values (including a=0 or a=1)
	pCompAndBin newTable = (pCompAndBin)malloc(28 * sizeof(compAndBin));
	checkAllocation(newTable, ALLOC_ERR);
	// filling table
	for (i = 0; i < 28; i++) {
		strcpy(newTable[i].comp, comp[i]); 
		strcpy(newTable[i].bin, bin[i]);
	}

	*compTable = newTable;
}

// Creates and initializes a predifiend DestTable (there are 8 values)
void initDestTable(pDestAndBin *destTable) {
	int i, j;
	// "0" in dest is equal to null
	char dest[8][4] = { "0", "M", "D", "DM", "A", "AM", "AD", "ADM" };

	pDestAndBin newTable = (pDestAndBin)malloc(8 * sizeof(destAndBin));
	checkAllocation(newTable, ALLOC_ERR);

	// filling table
	for (i = 0; i < 8; i++) {
		strcpy(newTable[i].dest, dest[i]);
		strcpy(newTable[i].bin, "000");
		// inserting ones if specific letter is exists in dest
		for (j = 0; j < 3; j++) {
			if (dest[i][j] == 'A')
				newTable[i].bin[0] = '1';
			else if (dest[i][j] == 'D')
				newTable[i].bin[1] = '1';
			else if (dest[i][j] == 'M')
				newTable[i].bin[2] = '1';
		}
	}

	*destTable = newTable;
}

// Creates and initializes a predifiend JumpTable
void initJumpTable(pJumpAndBin *jumpTable) {
	int i;
	// jump and bin are ordered, jump[i] -> bin[i]
	// "0" in jump is equal to null
	char jump[8][4] = { "0", "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP" };
	char bin[8][4] = {"000", "001", "010", "011", "100", "101", "110", "111"};

	pJumpAndBin newTable = (pJumpAndBin)malloc(8 * sizeof(jumpAndBin));
	checkAllocation(newTable, ALLOC_ERR);

	// filling table
	for (i = 0; i < 8; i++) {
		strcpy(newTable[i].jump, jump[i]);
		strcpy(newTable[i].bin, bin[i]);
	}

	*jumpTable = newTable;
}

// Function gets asmLine and translate it to binary accordding to predefiend tables
void convertCIntsToBin(pDestAndBin destTable, pCompAndBin compTable, pJumpAndBin jumpTable,
	asmLine* asmLine, char* binaryStr) {
	// prefix "111"
	strcpy(binaryStr, "111");

	// while table->dest != dest
	while (strcmp(destTable->dest, asmLine->dest) != 0) destTable++;
	// while table->comp != comp
	while (strcmp(compTable->comp, asmLine->comp) != 0) compTable++;
	// while table->jump != jump
	while (strcmp(jumpTable->jump, asmLine->jump) != 0) jumpTable++;
	
	// when all table pointers points on the searched value
	strcat(binaryStr, compTable->bin);
	strcat(binaryStr, destTable->bin);
	strcat(binaryStr, jumpTable->bin);
}

// Creates and initializes a SymbolTable with predifiend values
// no need to fill predefined labels
void initTable(pSymbolAddr *symbolTable) {
	int i;
	// str that represents the postfix of R
	// (e.g strIndex = "12", symbol = "R12")
	char strIndex[3];
	char symbol[4];
	pSymbolAddr newTable = (pSymbolAddr)malloc(ASM_FILE_SIZE * sizeof(symbolAddr));
	checkAllocation(newTable, ALLOC_ERR);

	// filling [R0, R15] rows
	for (i = 0; i < 16; i++) {
		symbol[0] = 'R';
		symbol[1] = '\0';
		_itoa(i, strIndex, 10);
		strcat(symbol, strIndex);
		strcpy(newTable[i].symbol, symbol);
		newTable[i].address = i;
	}
	// adding screen and kbd symbols (i = 16)
	strcpy(newTable[i].symbol, "SCREEN");
	newTable[i].address = 16384;
	strcpy(newTable[i + 1].symbol, "KBD");
	newTable[i + 1].address = 24576;

	*symbolTable = newTable;
}

void addEntry(pSymbolAddr symbolTable, char* symbol, int* nextAddress) {
	// moving pointer to look on the first empty slot in table
	// (+2 is for skipping screen and kbd symbols)
	pSymbolAddr pToInsert = symbolTable + (*nextAddress) + 2;
	// Fill row in table
	strcpy(pToInsert->symbol, symbol);
	pToInsert->address = *nextAddress;
	// Increment table counter
	(*nextAddress)++;
}

// Checks whether symbol exists in the table
Bool contains(pSymbolAddr symbolTable, int addrCtr, char* symbol) {
	int i;
	// iterating on table (+2 is for screen and kbd symbols)
	for (i = 0; i < addrCtr + 2; i++) {
		// if the symbol exists
		if (strcmp(symbolTable[i].symbol, symbol) == 0) {
			return TRUE;
		}
	}
	return FALSE;
}

// Returns the address associated with symbol if exists, 
// else returns -1 (not valid address)
int getAddress(pSymbolAddr symbolTable, int addrCtr, char* symbol) {
	int i;
	// iterating on table (+2 is for screen and kbd symbols)
	for (i = 0; i < addrCtr + 2; i++) {
		// if the symbol exists
		if (strcmp(symbolTable[i].symbol, symbol) == 0) {
			return symbolTable[i].address;
		}
	}
	return -1;
}

// Removes all white spaces (start, end) from string and between chars
// returns clean string
char* trim(char* string) {
	char* pToEnd;
	
	while (*string == ' ' || *string == '\t') string++;

	pToEnd = string + strlen(string) - 1;
	while (pToEnd > string && 
		  (*pToEnd == ' ' || *pToEnd == '\t' || *pToEnd == '\n')) {
		pToEnd--;
	}
	pToEnd[1] = '\0';

	return string;
}

// Removes all white spaces from string between chars
// returns clean string
char* trimInline(char* string) {
	char cleanedStr[ASM_FILE_SIZE];
	int i, j = 0;

	// removes spaces within the string (e.g: A =   1 --> A=1)
	for (i = 0; i < strlen(string); i++) {
		if (string[i] != ' ' && string[i] != '\t') {
			cleanedStr[j] = string[i];
			j++;
		}
	}
	cleanedStr[j] = '\0';
	strcpy(string, cleanedStr);

	return string;
}

// Removes all inline comments if there are
// e.g ( @32 //<comment to be removed> )
char* trimTrailingComments(char* string) {
	int i;
	
	for (i = 0; i < strlen(string); i++) {
		if (string[i] == '/') {
			string[i] = '\0';
			// calling to trim again to remove all white spaces
			// if any was between asm line and comment.
			return trim(string);
		}
	}
	return string;
}

// Reseting string to hold only zeros
void resetBinaryStr(char* binStr) {
	int i;
	for (i = 0; i < BIT_SIZE; i++) {
		binStr[i] = '0';
	}
	binStr[BIT_SIZE] = '\0';
}

// Function checks whether symbol is only number (i.g @65) 
Bool isSymbolIsNumber(char* symbol) {
	// while *symbol is digit
	while (*symbol >= '0' && *symbol <= '9') {
		symbol++;
		// if *symbol was digit and now line ended.
		if (*symbol == '\n' || *symbol == '\0')
			return TRUE;
	}
	return FALSE;
}

// Function gets a decimal number as a string
// and convert it to binary
void convertDecimalToBin(int decimal, char* binaryStr) {
	int i;

	for (i = 1; decimal > 0; i++) {
		// inserting to [strlen(binaryStr) - i] because we set lsb to msb
		binaryStr[strlen(binaryStr) - i] = (decimal % 2) == 1 ? '1' : '0';
		decimal = decimal / 2;
	}
}

// Function checks if pointer allocation is invalid and free memory needed and exit. 
void checkAllocation(void* pToCheck, char *message)
{
	if (pToCheck == NULL) {
		fprintf(stderr, "%s\n", message);
		exit(1); // closes files automaticaly.
	}
}

// Replace the file extention (.asm) to (.hack)
void getOutputFileName(char* fileName, char* outputFile) {
	char* pToExtention;

	strcpy(outputFile, fileName);
	// e.g: <name>.asm, pToExtention points on 'a' of asm. 
	pToExtention = outputFile + strlen(outputFile) - ASM_EXTENTION_LENGTH + 1;
	// adding "hack" after dot in outputFile
	strcpy(pToExtention, "hack");
}


int main()
{
	FILE* asmFile, *hackFile;
	asmLine parsedline;
	pSymbolAddr symbolTable = NULL;
	pDestAndBin destTable = NULL;
	pCompAndBin compTable = NULL;
	pJumpAndBin jumpTable = NULL;
	// counter for table length
	int nextEmptyAddress = 16;
	// helper variable for A_instraction
	int address;
	// Adding 4 for file extention (.asm)
	char fileName[FILE_NAME_SIZE + ASM_EXTENTION_LENGTH];
	// Adding 5 for file extention (.hack)
	char outputName[FILE_NAME_SIZE + 5];
	char binLine[BIT_SIZE + 1];
	char line[ASM_FILE_SIZE];
	//line = (char*)malloc(ASM_FILE_SIZE * sizeof(char));
	checkAllocation(line, ALLOC_ERR);

	printf("PLEASE NOTE: inputted assembly file and .exe");
	printf("\n\t  should be located on the **same** folder\n\n");
	printf("Please Enter assembly file name (i.g <name>.asw): ");
	scanf("%s", fileName);

	getOutputFileName(fileName, outputName);

	asmFile = fopen(fileName, "rt");
	checkAllocation(asmFile, FILE_ERR);
	hackFile = fopen(outputName, "wt");
	checkAllocation(hackFile, FILE_ERR);
	initTable(&symbolTable);
	initDestTable(&destTable);
	initCompTable(&compTable);
	initJumpTable(&jumpTable);
	// reading asmFile and writing to hack file
	// fgets stops on a new line
	while (fgets(line, ASM_FILE_SIZE, asmFile) != NULL) {
		strcpy(line, trim(line));
		// skipping comments
		if (line[0] == '/' || line[0] == '\n') {
			continue;
		}
		strcpy(line, trimTrailingComments(line));
		strcpy(line, trimInline(line));

		resetBinaryStr(binLine);
		parsedline.insType = getInstractionType(line[0]);
		if (parsedline.insType == A_INSTRUCTION) {
			getSymbol(line, parsedline.symbol);
			if (isSymbolIsNumber(parsedline.symbol)) {
				address = atoi(parsedline.symbol);
			}
			// read and/or add to table 
			else {
				// if symbol doesn't exist in table
				if (!contains(symbolTable, nextEmptyAddress, parsedline.symbol)) {
					address = nextEmptyAddress;
					addEntry(symbolTable, parsedline.symbol, &nextEmptyAddress);
				} // symbol exists in table 
				else {
					address = getAddress(symbolTable, nextEmptyAddress, parsedline.symbol);
				}
			}
			convertDecimalToBin(address, binLine);
		}
		// equal to if (lineType == C_INSTRUCTION)
		else {
			getDest(line, parsedline.dest);
			getComp(line, parsedline.comp);
			getJump(line, parsedline.jump);
			convertCIntsToBin(destTable, compTable, jumpTable, &parsedline, binLine);
		}
		// writing to hack file
		fprintf(hackFile, "%s\n", binLine);
	}

	printf("\n\nOutput file %s is now available", outputName);
	fclose(asmFile);
	fclose(hackFile);
	free(symbolTable);
	free(destTable);
	free(compTable);
	free(jumpTable);

	return 0;
}