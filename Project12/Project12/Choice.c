#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "records.h"

int main() {
    int choice;

    loadRecords();

    while (1) {
        printf("1 Create\n");
        printf("2 Search ID(NOT_WORKING)\n");
        printf("3 Search Nam(NOT_WORKING)\n");
        printf("4 Short List\n");
        printf("5 Full List\n");                       //EDIT RECORD WILL BE ADDED//
        printf("6 Delete\n");
        printf("7 Save\n");
        printf("8 Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: createRecord(); break;
        case 4: displayShort(); break;
        case 5: displayFull(); break;                 //EDIT RECORD WILL BE ADDED//
        case 6: deleteRecord(); break;
        case 7: saveRecords(); printf("Saved!\n"); break;
        case 8:
            saveRecords();
            free(records);
            exit(0);
        }
    }
}