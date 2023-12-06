#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../Internal_Libraries/cJSON.h"
#include "cJSON_Test_Functions.h"

char *ReadDB(char* chooseDatabase) {
    const char* filename;

    if(strcmp(chooseDatabase, "residents.json") == 0) {
        filename = "../JSON_Files/residents.json";
    } else if(strcmp(chooseDatabase, "medicine.json") == 0) {
        filename = "../JSON_Files/medicine.json";
    } else {
        perror("The chosen database can't be found");
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    fread(content, 1, length, file);
    fclose(file);

    content[length] = '\0';
    cJSON* json = cJSON_Parse(content);
    free(content);
    return cJSON_Print(json);
}

char* GetDBEntry(char* dataBase, int index) {
    cJSON *json = cJSON_Parse(dataBase);
    cJSON *jsonObject = cJSON_GetArrayItem(json, index);
    return cJSON_Print(jsonObject);
}

char* GetDBItem(char* array, char* itemName) {
    cJSON *json = cJSON_Parse(array);
    cJSON *itemValue = cJSON_GetObjectItemCaseSensitive(json, itemName);
    char* string = itemValue->valuestring;
    return string;
}

int GetDBArrayLength(char* array) {
    cJSON *json = cJSON_Parse(array);
    return cJSON_GetArraySize(json);
}

void TestFunction() {

    /* Read JSON content from a file
    const char* filename = "../JSON_Files/residents.json";
    char* file_content = read_file(filename);
    if (file_content == NULL) {
        perror("Error reading file");
        return;
    } else {
        printf("Read file correctly");
    }

    cJSON* json = cJSON_Parse(file_content);
    if (json == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        free(file_content);
        return;
    }

    ReadDB();

    int array_size = cJSON_GetArraySize(json);

    printf("%i", array_size);

    //printf("%s", file_content);

    cJSON* name = cJSON_GetObjectItem(json, "first_name");
    if (cJSON_IsString(name) && (name->valuestring != NULL)) {
        printf("Name: %s\n", name->valuestring);
    }

    cJSON *jsonObject = cJSON_GetArrayItem(json, 0);

    cJSON *itemValue = cJSON_GetObjectItemCaseSensitive(jsonObject, "room_number");

    //cJSON *jsonObjectNrTwo = cJSON_GetArrayItem(jsonObject, 0);
    // Print JSON Data
    char *string = cJSON_Print(jsonObject);

    char *stringTwo = cJSON_Print(itemValue);


    //printf("%s\n", string);
    printf("%s\n", stringTwo);
     Clean up
    cJSON_Delete(json);
    free(file_content);*/

}