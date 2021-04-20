#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

void replaceSubstring(char*, char*);
int* getIndexesOfSubs(char*, char*);
void removeEnter(char*);

#define STR_MAX_LEN 100

int main()
{
	char str1[STR_MAX_LEN], str2[STR_MAX_LEN];
	do {
		printf("Enter text: ");
		fgets(str1, STR_MAX_LEN, stdin);
		removeEnter(str1); // replacing '\n' char in end of string, if necessary.
		if (*str1 != '\0') { // continue if str1 is not empty.
			printf("Enter substring: ");
			fgets(str2, STR_MAX_LEN, stdin);
			removeEnter(str2); // replacing '\n' char in end of string, if necessary.
			/* checking if str2 isn't empty and if it's possible that
			   str2 will be substring of str1, by checking strlen(str1) >= strlen(str2) */
			if (*str2 != '\0' && (strlen(str1) >= strlen(str2))) {
				replaceSubstring(str1, str2);
				puts(str1);
			}

		printf("\n");
		}
	} while (*str1 != '\0' && *str2 != '\0');
	printf("Finish\n");

	return 0;
}

/* Function gets two strings and check whether
   "substr" is substring of "str". */
void replaceSubstring(char *str, char *substr)
{
	int i, j, curIndex, *arrOfIndexes;
	// getting all start indexes of occurrences of substr in str.
	arrOfIndexes = getIndexesOfSubs(str, substr);
	for (i = 0; arrOfIndexes[i] != -1; i++) {
		for (j = 0; j < strlen(substr); j++) {
			curIndex = arrOfIndexes[i] + j;
			// is small letter
			if (str[curIndex] >= 'a' && str[curIndex] <= 'z')
				str[curIndex] -= 32;
		}
	}
}

/* Function gets two string and returns 
   all start indexes of occurrences of substr in str. 
   The returned array ends with the element of -1. */
int* getIndexesOfSubs(char *str, char *substr)
{
	/* the worst case possible of size (e.x: "aaaa" with sub "a" -> size=4).
	   the plus one because the last element will hold -1. */ 
	int arr[STR_MAX_LEN + 1] = {0};
	int indexOfArr = 0, indexOfStr = 0;
	// declaring a pointer that will hold the address of one occurrence at a time.
	char* pOfFirstShow;
	while (str[indexOfStr] != '\0') { // while str is not empty.
		// getting address of first occurrence of "substr" in "str".
		pOfFirstShow = strstr(str + indexOfStr, substr);
		if (!pOfFirstShow) // no more occurrence left.
			break;
		indexOfStr = strlen(str) - strlen(pOfFirstShow); // calc the start index of occurrence.
		// inserting the index of first show of sub in str
		arr[indexOfArr] = indexOfStr;
		indexOfArr++;
		indexOfStr++;
	}
	/* adding -1 to the end of array in order to 
	   know when to stop itereting in main function (-1 is not a valid index) */
	arr[indexOfArr] = -1; 
	return arr;
}
/* Function gets string and if last char in string is '\n'
   replacing it with proper ending '\0'. */
void removeEnter(char *str)
{
	int len = strlen(str);
	if (str[len - 1] == '\n')
		str[len - 1] = '\0';
}