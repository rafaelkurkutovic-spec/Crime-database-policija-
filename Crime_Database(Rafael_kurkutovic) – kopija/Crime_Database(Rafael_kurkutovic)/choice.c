#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "records.h"

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

typedef enum {
    MENU_CREATE = 1,
    MENU_SEARCH_ID,
    MENU_SEARCH_NAME,
    MENU_SHORT,
    MENU_FULL,
    MENU_UPDATE,
    MENU_DELETE,
    MENU_SAVE,
    MENU_SORT,
    MENU_EXIT
} Menu;

int main() {

    int choice = 0;
    loadRecords();

    while (1) {

        printf("\n===== POLICE RECORD SYSTEM =====\n");
        printf("1.Create record\n2.Search ID\n3.Search Name\n4.Short list\n5.Full list\n");
        printf("6.Update record\n7.Delete record\n8.Save records\n9.Sort records\n10.Exit program\nChoice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\\n");
            while (getchar() != '\\n');
            continue;
        }

        switch (choice) {

        case MENU_CREATE: createRecord(); break;
        case MENU_SEARCH_ID: searchByID(); break;
        case MENU_SEARCH_NAME: searchByName(); break;
        case MENU_SHORT: displayShort(); break;
        case MENU_FULL: displayFull(); break;
        case MENU_UPDATE: updateRecord(); break;
        case MENU_DELETE: deleteRecord(); break;
        case MENU_SAVE: saveRecords(); break;
        case MENU_SORT: sortRecords(); break;
        case MENU_EXIT: exitProgram(); break;
         
        default:
            printf("Invalid choice.\\n");
        }
    }
}