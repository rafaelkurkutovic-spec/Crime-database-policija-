#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "records.h"

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

Record* records = NULL;
int count = 0;
int nextID = 1;
int unsavedChanges = 0;

static const char* FILE_NAME = "records.dat";
static const char* TEMP_FILE = "temp.dat";

static inline void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void readLine(char* text, int size) {
    fgets(text, size, stdin);
    text[strcspn(text, "\n")] = 0;
}

static int getSmallestFreeID() {
    int id = 1;

    while (1) {
        int found = 0;

        for (int i = 0; i < count; i++) {
            if (records[i].id == id) {
                found = 1;
                break;
            }
        }

        if (!found)
            return id;

        id++;
    }
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
        printf(RED "Invalid status. Try again: " RESET);
    }

    clearInputBuffer();
    return (Status)s;
}

static void updateField(char* field, int size) {
    char temp[250];

    readLine(temp, sizeof(temp));

    if (strcmp(temp, "#keep#") != 0) {
        strncpy(field, temp, size - 1);
        field[size - 1] = '\0';
    }
}

void createRecord() {

    clearInputBuffer();

    Record* temp = realloc(records, (count + 1) * sizeof(Record));

    if (temp == NULL) {
        perror(RED"realloc failed"RESET);
        return;
    }

    records = temp;

    records[count].id = getSmallestFreeID();

    printf("\nFirst name: ");
    readLine(records[count].firstName, MAX_NAME);

    printf("Last name: ");
    readLine(records[count].lastName, MAX_NAME);

    printf("Age: ");
    while (scanf("%d", &records[count].age) != 1) {
        clearInputBuffer();
        printf(RED "Invalid age (MUST USE NUMBERS): " RESET);
    }

    clearInputBuffer();

    printf("Crime: ");
    readLine(records[count].crime, MAX_CRIME);

    printf("Description: ");
    readLine(records[count].description, MAX_DESCRIPTION);

    records[count].status = inputStatus();

    count++;

    printf(GREEN "Created successfully.\n" RESET);

    printf(GREEN"\n===== NEW RECORD =====\n");

    printf("ID: %d\n", records[count - 1].id);
    printf("Name: %s %s\n",
        records[count - 1].firstName,
        records[count - 1].lastName);

    printf("Age: %d\n", records[count - 1].age);

    printf("Crime: %s\n",
        records[count - 1].crime);

    printf("Description: %s\n",
        records[count - 1].description);

    printf("Status: %s\n",
        statusToString(records[count - 1].status));

    unsavedChanges = 1;

    printf("======================\n"RESET);

}

void displayShort() {

    if (count == 0) {
        printf(RED "\nNo records found(create one first).\n" RESET);
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("%d - %s %s\n",
            records[i].id,
            records[i].firstName,
            records[i].lastName);
    }
}

void displayFull() {

    if (count == 0) {
        printf(RED "\nNo records found(create one first).\n" RESET);
        return;
    }

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
        printf(RED "Open failed.\n" RESET);
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(records, sizeof(Record), count, file);

    fclose(file);

    remove(FILE_NAME);

    if (rename(TEMP_FILE, FILE_NAME) != 0) {
        printf(RED "Rename failed.\n" RESET);
        return;
    }

    unsavedChanges = 0;

    printf(GREEN "Saved successfully.\n" RESET);
}

void loadRecords() {

    FILE* file = fopen(FILE_NAME, "rb");

    if (file == NULL)
        return;

    fread(&count, sizeof(int), 1, file);

    records = calloc(count, sizeof(Record));

    if (records == NULL) {
        fclose(file);
        return;
    }

    fread(records, sizeof(Record), count, file);

    fclose(file);
}

void deleteRecord()
{
    int id;

    printf("\nID: ");

    if (scanf("%d", &id) != 1)
    {
        clearInputBuffer();
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (records[i].id == id)
        {
            printf("\n===== RECORD TO DELETE =====\n");

            printf("ID: %d\n",
                records[i].id);

            printf("Name: %s %s\n",
                records[i].firstName,
                records[i].lastName);

            printf("Age: %d\n",
                records[i].age);

            printf("Crime: %s\n",
                records[i].crime);

            printf("Description: %s\n",
                records[i].description);

            printf("Status: %s\n",
                statusToString(records[i].status));

            printf("============================\n");

            int confirmation;

            printf("\nAre you sure?\n");
            printf(GREEN"1 = YES\n"RESET);
            printf(RED"0 = NO\n"RESET);
            printf("Choice: ");

            if (scanf("%d", &confirmation) != 1)
            {
                clearInputBuffer();
                printf(RED "\nInvalid input.\n" RESET);
                return;
            }

            if (confirmation == 0)
            {
                printf(RED "\nDelete cancelled.\n" RESET);
                return;
            }

            if (confirmation != 1)
            {
                printf(RED "\nInvalid choice.\n" RESET);
                return;
            }

            for (int j = i; j < count - 1; j++)
            {
                records[j] = records[j + 1];
            }

            count--;

            Record* temp = realloc(records,
                count * sizeof(Record));

            if (temp != NULL || count == 0)
            {
                records = temp;
            }

            unsavedChanges = 1;

            printf(GREEN "\nRecord deleted successfully.\n" RESET);

            return;
        }
    }

    printf(RED "\nRecord not found.\n" RESET);
}

void updateRecord() {

    int id;

    printf("ID: ");

    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        return;
    }

    printf("To keep something unchanged type #keep# OR 0=keep.\n");

    clearInputBuffer();

    for (int i = 0; i < count; i++) {

        if (records[i].id == id) {

            printf("Update first name from \"%s\" to (#keep#): ", records[i].firstName);
            updateField(records[i].firstName, MAX_NAME);

            printf("Update last name from \"%s\" to (#keep#): ", records[i].lastName);
            updateField(records[i].lastName, MAX_NAME);

            printf("Current age: %d (0=keep): ", records[i].age);

            int age;
            scanf("%d", &age);
            clearInputBuffer();

            if (age != 0)
                records[i].age = age;

            printf("Update crime from \"%s\" to (#keep#): ", records[i].crime);
            updateField(records[i].crime, MAX_CRIME);

            printf("Update description from \"%s\" to (#keep#): ", records[i].description);
            updateField(records[i].description, MAX_DESCRIPTION);

            records[i].status = inputStatus();

            unsavedChanges = 1;

            printf(GREEN "Record updated successfully.\n" RESET);
            return;


            printf(RED "Record not found.\n" RESET);


            printf(GREEN"\n===== UPDATED RECORD =====\n");

            printf("ID: %d\n", records[i].id);

            printf("Name: %s %s\n",
                records[i].firstName,
                records[i].lastName);

            printf("Age: %d\n",
                records[i].age);

            printf("Crime: %s\n",
                records[i].crime);

            printf("Description: %s\n",
                records[i].description);

            printf("Status: %s\n",
                statusToString(records[i].status));


            printf("==========================\n"RESET);

        }

    }



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

    int result = strcmp(r1->firstName, r2->firstName);

    if (result == 0)
        result = strcmp(r1->lastName, r2->lastName);

    return result;
}

int compareID(const void* a, const void* b) {

    const Record* r1 = a;
    const Record* r2 = b;

    if (r1->id < r2->id)
        return -1;

    if (r1->id > r2->id)
        return 1;

    return 0;
}

void sortRecords() {

    int choice;

    printf("\n1. Sort by ID");
    printf("\n2. Sort by Name");
    printf("\nChoice: ");

    scanf("%d", &choice);

    switch (choice) {

    case 1:
        qsort(records, count, sizeof(Record), compareID);
        printf(GREEN "\nSorted by ID.\n" RESET);

        unsavedChanges = 1;

        displayShort();

        break;





    case 2:
        qsort(records, count, sizeof(Record), compareNames);
        printf(GREEN "\nSorted by Name.\n" RESET);

        unsavedChanges = 1;

        displayShort();

        break;

    default:
        printf(RED "Invalid choice(chose between 1 and 2).\n" RESET);
    }
}

void searchByID() {

    if (count == 0) {
        printf(RED "No records found.\n" RESET);
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
        printf(RED "ID not found.\n" RESET);
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
    readLine(name, MAX_NAME);

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
        printf(RED "Name not found.\n" RESET);
}


void exitProgram()
{
    if (unsavedChanges)
    {
        int saveChoice;

        printf("\n====================================\n");
        printf("You have unsaved changes.\n");
        printf("Do you want to save before exiting?\n");
        printf(GREEN"1 = YES\n"RESET);
        printf(RED"0 = NO\n"RESET);
        printf("Choice: ");

        if (scanf("%d", &saveChoice) != 1)
        {
            clearInputBuffer();
            printf(RED "\nInvalid input.\n" RESET);
            return;
        }

        if (saveChoice == 1)
        {
            saveRecords();

            printf(GREEN
                "\nChanges saved successfully.\n"
                RESET);
        }
        else if (saveChoice == 0)
        {
            printf(RED
                "\nChanges will not be saved.\n"
                RESET);
        }
        else
        {
            printf(RED
                "\nInvalid choice.\n"
                RESET);
            return;
        }
    }

    if (records != NULL)
    {
        free(records);
        records = NULL;
    }

    printf(GREEN "\n=====Exited program=====\n" RESET);

    exit(EXIT_SUCCESS);
}