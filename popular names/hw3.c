/*
 * hw3.c
 *
 *  Created on: Apr 20, 2015
 *      Author: Shannon Murphy
 */

#include <stdio.h>
#include <string.h>

#define MAXNAMES 1000

int cmp(const void *a, const void *b) {
	return strcmp(a, b);
}

void getNames(char lines[MAXNAMES][100], char myNames[MAXNAMES][100], int ranks[MAXNAMES][10]) {
	char firstName[MAXNAMES][100];
	int rows = 0;
	int found = 0;
	int uniqueNames = 0; // number of unique names
	int yearCol = 0; // columns of rank array for the year
	int offset = 0;
	while(rows < MAXNAMES) {
		yearCol = rows / 100;
		sscanf(lines[rows], " %[^,]", firstName[rows]); // get name and put into firstName
		if(rows > 99) { // after first 100 are duplicates
			found = 0;
			int k = 0;
			while (k < MAXNAMES) {
				if (strcmp(firstName[rows], myNames[k]) == 0) { // if name exists in myNames already
					found = 1;
					offset++;
					ranks[k][yearCol] = (rows % 100) + 1;
				}
				k++;
			}
			if (found == 0) {
				int location = rows - offset;
				strcpy(myNames[location], firstName[rows]); // if it wasn't found, add it to myNames
				ranks[location][yearCol] = (rows % 100) + 1;
				uniqueNames++;
			}
		} else { // first 100
			uniqueNames++;
			strcpy(myNames[rows], firstName[rows]);
			ranks[rows][yearCol] = (rows % 100) + 1;
		}
	rows++;
	}
}

void readFile(char lines[MAXNAMES][100], FILE *infile, int size) {
	char item[100];
	int i = size;
	int j = 0;
	while((fscanf(infile,"%s",item) != EOF)) {
		size_t len = strlen(item);
		if (i < MAXNAMES) {
			while (j < len) {
				lines[i][j] = item[j];
				j++;
			}
			i++;
		}
		j = 0;
	}
}

void readAllFiles(char lines[MAXNAMES][100], FILE *infile) {
	char fileNames[10][21] = {"./names//yob1920.txt", "./names//yob1930.txt",
			"./names//yob1940.txt", "./names//yob1950.txt", "./names//yob1960.txt",
			"./names//yob1970.txt", "./names//yob1980.txt", "./names//yob1990.txt",
			"./names//yob2000.txt", "./names//yob2010.txt"};
	int yearCount;
	int size = 0;
	for (yearCount = 0; yearCount < 10; yearCount++) {
		infile = fopen(fileNames[yearCount], "rb");
		readFile(lines, infile, size);
		size += 100;
	}
}

void printFile(FILE *outfile, char myNames[MAXNAMES][100], int ranks[MAXNAMES][10]) {
	fprintf(outfile, "Names, 1920, 1930, 1940, 1950, 1960, 1970, 1980, 1990, 2000, 2010\r\n");
	int rows, cols;
	for(rows = 0; rows < 366; rows++) {
		fprintf(outfile, "%s,", myNames[rows]);
		for (cols = 0; cols < 10; cols++) {
			if (ranks[rows][cols] == 0) {
				fprintf(outfile, "%c", ',');
			} else {
				fprintf(outfile, "%d,", ranks[rows][cols]);
			}
		}
		fprintf(outfile, "\r\n");
	}
}

int main(void) {
	char myNames[MAXNAMES][100];
	int ranks[MAXNAMES][10];
	char lines[MAXNAMES][100]; // stores the lines of the file
	char fileNames[10][21] = {"./names//yob1920.txt", "./names//yob1930.txt",
			"./names//yob1940.txt", "./names//yob1950.txt", "./names//yob1960.txt",
			"./names//yob1970.txt", "./names//yob1980.txt", "./names//yob1990.txt",
			"./names//yob2000.txt", "./names//yob2010.txt"};
	FILE *infile;
    setvbuf(stdout, NULL, _IONBF, 0);
	FILE *outfile = fopen("./summary.csv", "w");
	int yearCount;
	int size = 0;
	for (yearCount = 0; yearCount < 10; yearCount++) {
		infile = fopen(fileNames[yearCount], "rb");
		readFile(lines, infile, size);
		size += 100;
	}
//	readAllFiles(lines, infile);

	getNames(lines, myNames, ranks);
	char temp[366][100]; // unsorted names
	int m;
	for (m = 0; m < 366; m++) {
		strcpy(temp[m], myNames[m]);
	}

	int i;
	for (i = 0; i < 366; i++) {
		qsort(myNames, 366, sizeof(myNames[0]), cmp);
	}

	int j, k = 0;
	while (k < 366) {
		for (j = 0; j < 366; j++) {
			if(strcmp(myNames[k], temp[j]) == 0) {
				ranks[k][0] = ranks[j][0];
				ranks[k][1] = ranks[j][1];
				ranks[k][2] = ranks[j][2];
				ranks[k][3] = ranks[j][3];
				ranks[k][4] = ranks[j][4];
				ranks[k][5] = ranks[j][5];
				ranks[k][6] = ranks[j][6];
				ranks[k][7] = ranks[j][7];
				ranks[k][8] = ranks[j][8];
				ranks[k][9] = ranks[j][9];
			}
		}
		k++;
	}

//	sortArrays(myNames, temp, ranks);
	printFile(outfile, myNames, ranks);
	fclose(infile);
	return 0;
}
