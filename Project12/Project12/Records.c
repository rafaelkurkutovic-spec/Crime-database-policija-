#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "records.h"

Record* records = NULL;
int count = 0;
int nextID = 1;

void createRecord() {
    Record* temp = realloc(records, (count + 1) * sizeof(Record));
    if (temp == NULL) {
        printf("Memory error!\n");
        return;
    }

    records = temp;

    records[count].id = nextID++;

    printf("\nAuto ID: %d\n", records[count].id);

    printf("First name: ");
    scanf("%s", records[count].firstName);

    printf("Last name: ");
    scanf("%s", records[count].lastName);

    printf("Age: ");
    scanf("%d", &records[count].age);

    printf("Crime: ");
    scanf("%s", records[count].crime);

    printf("Description (max 250 characters): ");
    scanf(" %[^\n]", records[count].description);

    printf("Status (OPEN/CLOSED/COLD): ");
    scanf("%s", records[count].status);

    count++;

    printf("Record created!\n");
}

void displayShort() {
    for (int i = 0; i < count; i++) {
        printf("================================\n");
        printf("ID:%d | %s %s\n",
            records[i].id,
            records[i].firstName,
            records[i].lastName);
        printf("\n");
    }
}

void displayFull() {
    for (int i = 0; i < count; i++) {
        printf("================================\n");
        printf("\nID: %d\n", records[i].id);
        printf("Name: %s %s\n", records[i].firstName, records[i].lastName);
        printf("Age: %d\n", records[i].age);
        printf("Crime: %s\n", records[i].crime);
        printf("Description: %s\n", records[i].description);
        printf("Status: %s\n", records[i].status);
    }
}

void loadRecords() {
    FILE* file = fopen("records.dat", "rb");
    if (file == NULL) return;

    fread(&count, sizeof(int), 1, file);

    records = malloc(count * sizeof(Record));
    if (records == NULL) return;

    fread(records, sizeof(Record), count, file);
    fclose(file);

    int maxID = 999;
    for (int i = 0; i < count; i++) {
        if (records[i].id > maxID)
            maxID = records[i].id;
    }

    nextID = maxID + 1;
}

void saveRecords() {
    FILE* file = fopen("records.dat", "wb");
    if (file == NULL) return;

    fwrite(&count, sizeof(int), 1, file);
    fwrite(records, sizeof(Record), count, file);

    fclose(file);
}



void deleteRecord() {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (records[i].id == id) {

            for (int j = i; j < count - 1; j++)
                records[j] = records[j + 1];

            count--;
            records = realloc(records, count * sizeof(Record));

            printf("Deleted!\n");
            return;
        }
    }
}


//EDIT RECORD WILL BE ADDED//