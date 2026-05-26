#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "records.h"

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
        printf("1.Create\n2.Search ID\n3.Search Name\n4.Short\n5.Full\n");
        printf("6.Update\n7.Delete\n8.Save\n9.Sort\n10.Exit\nChoice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
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

        case MENU_EXIT:
            saveRecords();
            if (records != NULL) {
                free(records);
                records = NULL;
            }
            return 0;

        default:
            printf("Invalid choice.\n");
        }
    }
}
