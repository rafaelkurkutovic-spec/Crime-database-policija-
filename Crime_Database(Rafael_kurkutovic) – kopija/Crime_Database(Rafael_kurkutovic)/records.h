#ifndef RECORDS_H
#define RECORDS_H

#define MAX_NAME 50
#define MAX_CRIME 100
#define MAX_DESCRIPTION 250

typedef enum {
    OPEN,
    CLOSED,
    COLD
} Status;

typedef struct {
    int id;
    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    int age;
    char crime[MAX_CRIME];
    char description[MAX_DESCRIPTION];
    Status status;
} Record;

extern Record* records;
extern int count;
extern int nextID;
extern int unsavedChanges;

void loadRecords();
void saveRecords();

void createRecord();
void deleteRecord();
void updateRecord();

void displayShort();
void displayFull();

void searchByID();
void searchByName();

void sortRecords();

void exitProgram();

#endif
