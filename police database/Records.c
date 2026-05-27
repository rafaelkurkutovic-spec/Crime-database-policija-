#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "records.h"

Record* records = NULL;
int count = 0;


static const char* FILE_NAME = "records.dat";
static const char* TEMP_FILE = "temp.dat";

static inline void clearInputBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

static const char* statusToString(Status s) {
	switch (s) {
	case OPEN: return "OPEN";
	case CLOSED: return "CLOSED";
	case COLD: return "COLD";
	default: return "UNKNOWN";
	}
}

static Status inputStatus() {
	int s;
	printf("Status (0=OPEN,1=CLOSED,2=COLD): ");
	while (scanf("%d", &s) != 1 || s < 0 || s > 2) {
		clearInputBuffer();
		printf("Invalid status. Try again: ");
	}
	return (Status)s;
}

void createRecord() {

	Record* temp = realloc(records, (count + 1) * sizeof(Record));

	if (temp == NULL) {
		perror("realloc failed");
		return;
	}

	records = temp;
	records[count].id = getNextAvailableID();

	clearInputBuffer();

	printf("\nFirst name: ");
	fgets(records[count].firstName, MAX_NAME, stdin);
	records[count].firstName[strcspn(records[count].firstName, "\n")] = 0;

	printf("Last name: ");
	fgets(records[count].lastName, MAX_NAME, stdin);
	records[count].lastName[strcspn(records[count].lastName, "\n")] = 0;

	printf("Age: ");
	while (scanf("%d", &records[count].age) != 1) {
		clearInputBuffer();
		printf("Invalid age: ");
	}

	clearInputBuffer();

	printf("Crime: ");
	fgets(records[count].crime, MAX_CRIME, stdin);
	records[count].crime[strcspn(records[count].crime, "\n")] = 0;

	printf("Description: ");
	fgets(records[count].description, MAX_DESCRIPTION, stdin);
	records[count].description[strcspn(records[count].description, "\n")] = 0;

	records[count].status = inputStatus();

	count++;

	printf("Created.\n");
}

void displayShort() {
	for (int i = 0; i < count; i++) {
		printf("%d - %s %s\n",
			records[i].id,
			records[i].firstName,
			records[i].lastName);
	}
}

void displayFull() {
	for (int i = 0; i < count; i++) {
		printf("\nID:%d\n", records[i].id);
		printf("Name:%s %s\n", records[i].firstName, records[i].lastName);
		printf("Age:%d\n", records[i].age);
		printf("Crime:%s\n", records[i].crime);
		printf("Description:%s\n", records[i].description);
		printf("Status:%s\n", statusToString(records[i].status));
	}
}

void saveRecords() {
	FILE* file = fopen(TEMP_FILE, "wb");
	if (file == NULL) {
		perror("Open failed");
		return;
	}

	fwrite(&count, sizeof(int), 1, file);
	fwrite(records, sizeof(Record), count, file);

	if (ferror(file)) {
		perror("Write error");
	}

	fclose(file);

	remove(FILE_NAME);
	if (rename(TEMP_FILE, FILE_NAME) != 0) {
		perror("Rename failed");
	}
}

void loadRecords() {
	FILE* file = fopen(FILE_NAME, "rb");
	if (file == NULL) {
		printf("No existing records found. Creating a new DATABASE.");
		return;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	if (size == 0) {
		fclose(file);
		return;
	}

	if (fread(&count, sizeof(int), 1, file) != 1) {
		perror("Read count failed");
		fclose(file);
		return;
	}

	records = calloc(count, sizeof(Record));
	if (records == NULL) {
		perror("calloc failed");
		fclose(file);
		return;
	}

	if (fread(records, sizeof(Record), count, file) != (size_t)count) {
		perror("Read records failed");
	}

	fclose(file);

	for (int i = 0; i < count; i++) {
		if (records[i].id <= 0) {
			records[i].id =
				getNextAvailableID();
		}
	}
}

void deleteRecord() {
	int id;
	printf("ID: ");
	if (scanf("%d", &id) != 1) {
		clearInputBuffer();
		return;
	}

	for (int i = 0; i < count; i++) {
		if (records[i].id == id) {

			for (int j = i; j < count - 1; j++)
				records[j] = records[j + 1];

			count--;

			Record* temp = realloc(records, count * sizeof(Record));
			if (temp != NULL || count == 0)
				records = temp;

			printf("Deleted.\n");
			return;
		}
	}

	printf("Not found.\n");
}

void updateRecord() {

	int id;

	printf("ID: ");

	if (scanf("%d", &id) != 1) {
		clearInputBuffer();
		return;
	}

	for (int i = 0; i < count; i++) {

		if (records[i].id == id) {

			clearInputBuffer();

			printf("New first name: ");
			fgets(records[i].firstName, MAX_NAME, stdin);
			records[i].firstName[strcspn(records[i].firstName, "\n")] = 0;

			printf("New last name: ");
			fgets(records[i].lastName, MAX_NAME, stdin);
			records[i].lastName[strcspn(records[i].lastName, "\n")] = 0;

			printf("New age: ");

			while (scanf("%d", &records[i].age) != 1) {
				clearInputBuffer();
				printf("Invalid age: ");
			}

			clearInputBuffer();

			printf("New crime: ");
			fgets(records[i].crime, MAX_CRIME, stdin);
			records[i].crime[strcspn(records[i].crime, "\n")] = 0;

			printf("New description: ");
			fgets(records[i].description, MAX_DESCRIPTION, stdin);
			records[i].description[strcspn(records[i].description, "\n")] = 0;

			records[i].status = inputStatus();

			printf("Updated.\n");
			return;
		}
	}

	printf("Not found.\n");
}

static int recursiveBinarySearch(Record arr[], int left, int right, int id) {
	if (left > right)
		return -1;

	int mid = (left + right) / 2;

	if (arr[mid].id == id)
		return mid;

	if (id < arr[mid].id)
		return recursiveBinarySearch(arr, left, mid - 1, id);

	return recursiveBinarySearch(arr, mid + 1, right, id);
}

int compareNames(const void* a, const void* b) {
	const Record* r1 = a;
	const Record* r2 = b;
	return strcmp(r1->firstName, r2->firstName);
}

int compareID(const void* a, const void* b) {
	const Record* r1 = a;
	const Record* r2 = b;
	return r1->id - r2->id;
}

void sortRecords() {
	qsort(records, count, sizeof(Record), compareNames);
	printf("Sorted.\n");
}

void searchByID() {
	if (count == 0) {
		printf("No records.\n");
		return;
	}

	qsort(records, count, sizeof(Record), compareID);

	int id;
	printf("ID: ");
	if (scanf("%d", &id) != 1) {
		clearInputBuffer();
		return;
	}

	int index = recursiveBinarySearch(records, 0, count - 1, id);

	if (index == -1) {
		printf("Not found.\n");
		return;
	}

	printf("%d %s %s\n",
		records[index].id,
		records[index].firstName,
		records[index].lastName);
}

void searchByName() {

	char name[MAX_NAME];
	int found = 0;

	clearInputBuffer();

	printf("First name: ");
	fgets(name, MAX_NAME, stdin);
	name[strcspn(name, "\n")] = 0;

	for (int i = 0; i < count; i++) {

		if (strcmp(records[i].firstName, name) == 0) {

			printf("%d %s %s\n",
				records[i].id,
				records[i].firstName,
				records[i].lastName);

			found = 1;
		}
	}

	if (!found)
		printf("Not found.\n");
}

int getNextAvailableID() {

	for (int id = 1; ; id++) {

		int found = 0;

		for (int i = 0; i < count; i++) {
			if (records[i].id == id) {
				found = 1;
				break;
			}
		}

		if (!found)
			return id;
	}
}