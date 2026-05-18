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
        printf("3 Search Name(NOT_WORKING)\n");
        printf("4 Short List\n");
        printf("5 Full List\n");            
        printf("6 Update Record\n");
        printf("7 Delete\n");
        printf("8 Save\n");
        printf("9 Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: createRecord(); break;
        case 4: displayShort(); break;
        case 5: displayFull(); break;
        case 6: updateRecord(); break;
        case 7: deleteRecord(); break;
        case 8: saveRecords(); printf("Saved!\n"); break;
        case 9:
            saveRecords();
            free(records);
            exit(0);
        }
    }
}