#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define M 6 // work hours

void printSchedule(int[][M], int);
int isScheduleOk(int[][M]);
int isStationPickupOk(int[]);
void setDriversWithOverlapStations(int[][M], int[]);
void isDriversDriveOverLimit(int[][M], int[]);
void setLongestSeqToArr(int[], int);
int isDriverOk(int[], char*);

#define N 5 // pickup points
#define K 10 // num of drivers
#define TRUE 1
#define FALSE 0

int main()
{
	int bus[N][M] = {{6, 3, 8, 0, 0, 5}, 
					 {6, 6, 5, 7, 8, 4},
					 {4, 5, 6, 6, 1, 7}, 
					 {0, 4, 3, 2, 5, 0}, 
					 {0, 8, 7, 3, 2, 8}};
	int startHourt = 10;

	printSchedule(bus, startHourt);
	if (isScheduleOk(bus)) {
		printf("\nThe schedule is ok!\n");
		return 0;
	}
	return 1; // logical else
}

void printSchedule(int scheduleArr[][M], int startHour)
{
	int i, j;
	// printing hours row.
	for (i = 0; i < M; i++) {
		if (i == 0)
			printf("%9s", "");
		printf("%4d:00", startHour++); // incriment after print
	}
	// printing stations and drivers.
	for (i = 0; i < N; i++) {
		printf("\nstation %d:", i + 1);
		for (j = 0; j < M; j++)
			printf("%4d%3s", scheduleArr[i][j], "");
	}
	printf("\n");
}

int isScheduleOk(int scheduleArr[][M])
{
	int i, tempFlag = TRUE, isSchedOk = TRUE;
	int errorDrivers[K] = { 0 }; // initialize arr with K drivers.
	char* overlapErrMessage = "\nThe drivers that have two or more stations in one hour are:\n";
	char* overDriveErrMessage = "\nThe drivers that drive three hours in a row are:\n";

	// checking if stations pickup ok.
	for (i = 0; i < N; i++) {
		if (!isStationPickupOk(scheduleArr[i])) {
			if (tempFlag) {
				printf("\nThe stations with no collection for more than two hours are:\n");
				tempFlag = FALSE;
				isSchedOk = FALSE;
			}
			printf("station number %d\n", i + 1);
		}
	}
	// checking if drivers overlap stations.
	setDriversWithOverlapStations(scheduleArr, errorDrivers);
	if (!isDriverOk(errorDrivers, overlapErrMessage))
		isSchedOk = FALSE;

	// checking if drivers over limit time.
	isDriversDriveOverLimit(scheduleArr, errorDrivers);
	if (!isDriverOk(errorDrivers, overDriveErrMessage))
		isSchedOk = FALSE;

	return isSchedOk;
}

int isDriverOk(int errorDrivers[], char* preMess)
{
	int i, isCheckOk;
	isCheckOk = TRUE;
	for (i = 0; i < K; i++) {
		if (errorDrivers[i] == -1) {
			if (isCheckOk) {
				printf("%s", preMess);
				isCheckOk = FALSE;
			}
			printf("driver number %d\n", i + 1);
		}
	}
	return isCheckOk;
}


int isStationPickupOk(int station[])
{
	int i;
	for (i = 0; i < M - 1; i++) {
		if (station[i] == 0 && station[i + 1] == 0)
			return FALSE; // station not ok
	}
	return TRUE; // logical else.
}

void setDriversWithOverlapStations(int scheduleArr[][M], int overlapDrivers[])
{
	int i, j, driverIdx;
	for (i = 0; i < M; i++) { // iterating over cols.
		for (j = 0; j < N; j++) { // iterating over rows.
			driverIdx = scheduleArr[j][i];
			if (driverIdx != 0 && overlapDrivers[driverIdx - 1] != -1)
				if (++overlapDrivers[driverIdx - 1] == 2)  // incriment before checking
					overlapDrivers[driverIdx - 1] = -1; // the driver has overlap
		}

		for (j = 0; j < K; j++) { // iterating over overlapDrivers.
			if (overlapDrivers[j] != -1) // checking if there isn't overlap of a driver.
				overlapDrivers[j] = 0; // reseting the element in array for next col;
		}
	}
}

void isDriversDriveOverLimit(int scheduleArr[][M], int overDriveDrivers[])
{
	/* "driversSeqArr" is matrix with rows for each driver.
	   col[0] holds the longestSeqCounted(not including seq that didn't break).
	   col[1] holds the current counted seq before assinging to col[0].
	   col[2] holds the last col that driver was seen in the matrix. 
	*/
	int driversSeqArr[K][3] = { 0 };
	int i, j, driverIndex;
	for (i = 0; i < N; i++) { // iterating over schedule rows.
		for (j = 0; j < M; j++) { // iterating over schedule cols.
			driverIndex = scheduleArr[i][j];
			if (driverIndex != 0)
				setLongestSeqToArr(driversSeqArr[driverIndex - 1], j);
		}
	}

	for (i = 0; i < K; i++) { // iterating over first col of driverSeqArr
		// checking if seq is larger than 2
		if (driversSeqArr[i][0] >= 3 || driversSeqArr[i][1] >= 3)
			overDriveDrivers[i] = -1;
		else
			overDriveDrivers[i] = 0;
	}
}

void setLongestSeqToArr(int driverSeqArr[], int col)
{
	int *lastSeen, *curSeqCnt, *longestSeq; // only for readablilty.
	longestSeq = &driverSeqArr[0];
	curSeqCnt = &driverSeqArr[1];
	lastSeen = &driverSeqArr[2];
	if (*lastSeen == col - 1) {
		*lastSeen = col;
		(*curSeqCnt)++;
	}
	else {
		*longestSeq = (*longestSeq < *curSeqCnt) ? *curSeqCnt : *longestSeq;
		*curSeqCnt = 1;
	}
}