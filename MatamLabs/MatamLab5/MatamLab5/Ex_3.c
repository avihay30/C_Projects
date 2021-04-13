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
		removeEnter(str1);
		if (*str1 != '\0') {
			printf("Enter substring: ");
			fgets(str2, STR_MAX_LEN, stdin);
			removeEnter(str2);
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

void replaceSubstring(char *str, char *substr)
{
	int *arrOfIndexes = getIndexesOfSubs(str, substr);
	int i, j, curIndex;
	for (i = 0; arrOfIndexes[i] != -1; i++) {
		for (j = 0; j < strlen(substr); j++) {
			curIndex = arrOfIndexes[i] + j;
			// is small letter
			if (str[curIndex] >= 'a' && str[curIndex] <= 'z')
				str[curIndex] -= 32;
		}
	}
}

int* getIndexesOfSubs(char *str, char *substr)
{
	// the worst case possible of size (e.x "aaa" with sub "a")
	int arr[STR_MAX_LEN] = {0};
	char *pOfFirstShow;
	int indexOfArr = 0, indexOfStr = 0;
	while (str[indexOfStr] != '\0') {
		pOfFirstShow = strstr(str + indexOfStr, substr);
		if (!pOfFirstShow)
			break;
		indexOfStr = strlen(str) - strlen(pOfFirstShow);
		// inserting the index of first show of sub in str
		arr[indexOfArr] = indexOfStr;
		indexOfArr++;
		indexOfStr++;
	}
	arr[indexOfArr] = -1;
	return arr;
}

void removeEnter(char *str)
{
	int len = strlen(str);
	if (str[len - 1] == '\n')
		str[len - 1] = '\0';
}