#ifndef RECORDS_H
#define RECORDS_H

typedef struct {
    int id;
    char firstName[50];
    char lastName[50];
    int age;
    char crime[100];
    char description[250];
    char status[20]; // OPEN / CLOSED

} Record;

extern Record* records;
extern int count;
extern int nextID;

void loadRecords();
void saveRecords();
void createRecord();
void editRecord();          
void deleteRecord();
void displayShort();
void displayFull();
void updateRecord();
void searchByID();
void searchByName();

#endif