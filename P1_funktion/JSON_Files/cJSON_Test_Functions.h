//
// Created by danie on 13-11-2023.
//

#ifndef P1_PROGRAM_CJSON_TEST_FUNCTIONS_H
#define P1_PROGRAM_CJSON_TEST_FUNCTIONS_H

char *ReadDB(char* chooseDatabase);
char* GetDBEntry(char* dataBase, int index);
char* GetDBItem(char* array, char* itemName);
int GetDBArrayLength(char* array);

#endif //P1_PROGRAM_CJSON_TEST_FUNCTIONS_H
