#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef enum { FALSE, TRUE } Bool;

#define M 6 // work hours

void printSchedule(int[][M], int);
Bool isScheduleOk(int[][M]);
Bool isDriverOk(int[], char*);
Bool isStationPickupOk(int[]);
void setDriversWithOverlapStations(int[][M], int[]);
void setDriversDriveOverLimit(int[][M], int[]);
void setLongestSeqToArr(int[], int);

#define N 5 // num of stations
#define K 10 // num of drivers
#define START_HOUR 10

int main()
{
	int bus[N][M] = {{6, 3, 8, 0, 0, 5}, 
					 {6, 6, 5, 7, 8, 4},
					 {4, 5, 6, 6, 1, 7}, 
					 {0, 4, 3, 2, 5, 0}, 
					 {0, 8, 7, 3, 2, 8}};

	printSchedule(bus, START_HOUR); // prints the schedule table.
	if (isScheduleOk(bus)) { // running 3 tests on schedule and returns if successful
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
			printf("%9s", ""); // gap of 9 spaces.
		printf("%4d:00", startHour++); // increment after print
	}
	// printing stations and drivers.
	for (i = 0; i < N; i++) { // iterating over stations.
		printf("\nstation %d:", i + 1);
		for (j = 0; j < M; j++) // iterating over hours.
			printf("%4d%3s", scheduleArr[i][j], "");
	}
	printf("\n");
}

Bool isScheduleOk(int scheduleArr[][M])
{
	int i, isSchedOk = TRUE;
	int errorDrivers[K] = { 0 }; // initialize arr with K drivers.
	// output messages if test 2 or 3 failed.
	char* overlapErrMessage = "\nThe drivers that have two or more stations in one hour are:\n";
	char* overDriveErrMessage = "\nThe drivers that drive three hours in a row are:\n";

	// checking if stations pickup ok.
	for (i = 0; i < N; i++) {
		if (!isStationPickupOk(scheduleArr[i])) {
			if (isSchedOk) { // checking if it's failed in the first time 
				printf("\nThe stations with no collection for more than two hours are:\n");
				isSchedOk = FALSE;
			}
			printf("station number %d\n", i + 1);
		}
	}
	// filling the "errorDrivers" array with -1 in the problematic driver slot.
	setDriversWithOverlapStations(scheduleArr, errorDrivers);
	if (!isDriverOk(errorDrivers, overlapErrMessage)) // checking if drivers overlap stations.
		isSchedOk = FALSE;

	/* rewriting(no need to reset array to 0's) the "errorDrivers" array 
	   with -1 in the problematic driver slot. */
	setDriversDriveOverLimit(scheduleArr, errorDrivers);
	if (!isDriverOk(errorDrivers, overDriveErrMessage)) // checking if drivers over limit time.
		isSchedOk = FALSE;

	return isSchedOk;
}


/* Function checks whether there is a driver with -1 in there slot
   if so, printing the pre message(if first show) and his number and returns FALSE */
Bool isDriverOk(int errorDrivers[], char* preMess)
{
	int i, isCheckOk = TRUE;
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

/* Function gets a station array and checks whether there is sequence of 2 zero's */
Bool isStationPickupOk(int station[])
{
	int i;
	for (i = 0; i < M - 1; i++) {
		if (station[i] == 0 && station[i + 1] == 0) // checking sequence of 2 zero's.
			return FALSE; // station not ok
	}
	return TRUE; // logical else.
}

/* Function gets the schedule matrix and dummy array with size of drivers
   and fill the dummy array with -1 if the driver has two or more stations in one hour. */
void setDriversWithOverlapStations(int scheduleArr[][M], int overlapDrivers[])
{
	int i, j, driverIdx;
	for (i = 0; i < M; i++) { // iterating over cols.
		for (j = 0; j < N; j++) { // iterating over rows.
			driverIdx = scheduleArr[j][i]; // for readabilty.
			if (driverIdx != 0 && overlapDrivers[driverIdx - 1] != -1)
				if (++overlapDrivers[driverIdx - 1] == 2)  // increment before checking
					overlapDrivers[driverIdx - 1] = -1; // the driver has it's first overlap
		}

		for (j = 0; j < K; j++) { // iterating over overlapDrivers.
			if (overlapDrivers[j] != -1) // checking if there isn't overlap of a driver.
				overlapDrivers[j] = 0; // reseting the element in array for next col iteration;
		}
	}
}

/* Function gets the schedule matrix and dummy array with size of drivers
   and rewrite the dummy array with -1 if the driver works 3 or more hours non-stop. */
void setDriversDriveOverLimit(int scheduleArr[][M], int overDriveDrivers[])
{
	/* "driversSeqArr" is matrix with rows for each driver.
	   col[0] holds the longestSeqCounted(not including seq that didn't break).
	   col[1] holds the current counted seq before assinging to col[0].
	   col[2] holds the last col that driver was seen in the matrix. 
	*/
	int driversSeqArr[K][3] = { 0 };
	int i, j, driverIndex;
	for (i = 0; i < M; i++) { // iterating over schedule cols.
		for (j = 0; j < N; j++) { // iterating over schedule rows.
			driverIndex = scheduleArr[j][i]; // for readabilty.
			if (driverIndex != 0)
				setLongestSeqToArr(driversSeqArr[driverIndex - 1], i);
		}
	}

	for (i = 0; i < K; i++) { // iterating over first col of driverSeqArr
		/* checking if longestseq is larger than 2,
		   or currSeqCnt(col 1) is larger than 2, 
	       if the seq didn't break(the driver didn't appear again after counting). */
		if (driversSeqArr[i][0] >= 3 || driversSeqArr[i][1] >= 3)
			overDriveDrivers[i] = -1;
		else
			overDriveDrivers[i] = 0;
	}
}


/* Function gets an array with size of 3 and the col in the scheduleArr he was seen
   and update the "driverSeqArr" with new values where needed */
void setLongestSeqToArr(int driverSeqArr[], int col)
{
	int *lastSeen, *curSeqCnt, *longestSeq; // only for readability.
	longestSeq = &driverSeqArr[0];
	curSeqCnt = &driverSeqArr[1];
	lastSeen = &driverSeqArr[2];
	if (*lastSeen == col - 1) { // if seq continues.
		(*curSeqCnt)++;
	}
	// exulding in calc the overlap drivers. 
	else if (*lastSeen != col || *curSeqCnt == 0) { // if seq breaks or starts.
		*longestSeq = (*longestSeq < *curSeqCnt) ? *curSeqCnt : *longestSeq;
		*curSeqCnt = 1;
	}
	*lastSeen = col;
}