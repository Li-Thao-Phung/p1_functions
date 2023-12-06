#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./Internal_Libraries/cJSON.h"
#include <ctype.h>
#include "./JSON_Files/cJSON_Test_Functions.h"

#define SIZE_OF_MEDICINE 16
#define DOSAGE_SIZE 10
#define NAME_SIZE 30

typedef struct{
    char medicationName[NAME_SIZE];
    char dosage[DOSAGE_SIZE];
    int biweeklyEstimation;
}estimation_t;

typedef struct{
    char medicationName[NAME_SIZE];
    char dosage[DOSAGE_SIZE];
    char quantity[DOSAGE_SIZE];
}medicine_t;

typedef struct{
    char medicationName[NAME_SIZE];
    char dosage[DOSAGE_SIZE];
    char needed[DOSAGE_SIZE];
}order_t;


order_t* UpdateDbStorage(void);
medicine_t* getMedcineInfo();
int addToList(char* medicationValue,char* dosageValue, estimation_t residentEstimation[], int elementsInArray);
estimation_t* getResidentsEstimations(int *elementsInArray);
void compareQuantityEstimation(estimation_t residentEstimation[], medicine_t quantityList[], order_t *orderList[], int* k, int i);
void convertMedicationNameToLowercase(medicine_t *med);
char* GetDBEItemModified(char* array, char* itemName);
void assignValueToList(estimation_t residentEstimation[], int index, int *elementsInArray, char *medicationValue, char *dosageValue);


int main() {
    // Function return either a list with medicine which is needed or a list containing {0,0,0} as its first element
    order_t* orderList = UpdateDbStorage();

    printf("-------------------\n");

    for (int i = 0; i < 1; ++i){
        printf("%s %s %s \n", orderList[i].medicationName, orderList[i].dosage, orderList[i].needed);
    }

    free(orderList);
    return 0;
}

order_t* UpdateDbStorage(void){

    order_t* orderList = malloc(SIZE_OF_MEDICINE * sizeof(order_t));

    if(orderList == NULL){
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int elementsInArray = 0;

    // Read medication name, dosage from residents DB onto residentEstimation
    estimation_t* residentEstimation = getResidentsEstimations(&elementsInArray);
    // Read medication name, dosage and quantity from medicine DB onto quantityList
    medicine_t* quantityList = getMedcineInfo();

    /*
    for (int i = 0; i < elementsInArray; ++i){
        printf("%s %s %d\n", residentEstimation[i].medicationName, residentEstimation[i].dosage, residentEstimation[i].biweeklyEstimation);
    }
    printf("-------------------\n");
    for (int i = 0; i < SIZE_OF_MEDICINE; ++i){
        printf("%s %s %s\n", quantityList[i].medicationName, quantityList[i].dosage, quantityList[i].quantity);
    }
     */


    int k = 0;
    // Loop through each element in residentEstimation and compare to every single element in quantityList
    for (int i = 0; i < elementsInArray; ++i){
        compareQuantityEstimation(residentEstimation, quantityList, &orderList, &k, i);
    }

    // free memory for dynamically allocated arrays
    free(quantityList);
    free(residentEstimation);

    // Return the list if it has been assigned
    if (k != 0)
        return orderList;
    else{
        // Assign 0 to all fields when the list has not been assigned
        orderList[0] = (order_t) {.medicationName = "0", .dosage = "0", .needed = "0"};
        return orderList;
    }


}

char* GetDBEItemModified(char* array, char* itemName){
    cJSON *json = cJSON_Parse(array);
    cJSON *itemValue = cJSON_GetObjectItemCaseSensitive(json, itemName);
    return cJSON_Print(itemValue);
}

void compareQuantityEstimation(estimation_t residentEstimation[], medicine_t quantityList[], order_t *orderList[], int *k, int i){

    int needed;

    // Loop through every element in quantityList
    for(int j = 0; j < SIZE_OF_MEDICINE; ++j){

        // Compare the medication name and dosage
        if(strcmp(quantityList[j].medicationName,residentEstimation[i].medicationName) == 0 &&
           strcmp(quantityList[j].dosage, residentEstimation[i].dosage) == 0) {

            // Determine whether biweeklyEstimation is greater than the quantity
            if(residentEstimation[i].biweeklyEstimation > atoi(quantityList[j].quantity)){

                // Assign the medication name and the dosage onto the orderList
                strcpy(orderList[*k]->medicationName, residentEstimation[i].medicationName);
                strcpy(orderList[*k]->dosage, residentEstimation[i].dosage);

                // Calculate how much medication is needed and asign onto orderList
                needed = residentEstimation[i].biweeklyEstimation - atoi(quantityList[j].quantity);
                sprintf(orderList[*k]->needed, "%d", needed); // stores needed as a string

                ++(*k); // increment the index-counter k for orderList
            }
        }
    }

}


/*********************************FUNCTION TO READ FROM RESIDENTS DB**************************************************************/

estimation_t* getResidentsEstimations(int *elementsInArray){

    estimation_t *residentEstimation = malloc(SIZE_OF_MEDICINE * sizeof(estimation_t));

    // Open resident file and get json-formatted string for whole DB
    char *residentsArray = ReadDB("residents.json");
    int lengthResidents = GetDBArrayLength(residentsArray);

    // Loop through each resident
    for (int i = 0; i < lengthResidents; ++i){
        // Get entry to the json objects of residents
        char *resident = GetDBEntry(residentsArray, i);

        // Get item medicine containing arrays
        char *medicineArray = GetDBEItemModified(resident, "medicine");
        int lengthResidentsItems = GetDBArrayLength(medicineArray);


        // Process each medication in medicine array
        for(int j = 0; j < lengthResidentsItems; ++j){
            char *medicineArrayItems = GetDBEntry(medicineArray, j);

            char *medicationName = GetDBEntry(medicineArrayItems, 0);
            char *dosage = GetDBEntry(medicineArrayItems, 1);

            // Get the values
            char* medicationValue = cJSON_Parse(medicationName)->valuestring;
            char* dosageValue = cJSON_Parse(dosage)->valuestring;

            // Check if medicine name and dosage already exist in list
            int index = addToList(medicationValue,dosageValue, residentEstimation, *elementsInArray);

            assignValueToList(residentEstimation, index, elementsInArray, medicationValue, dosageValue);

        }
    }
    return residentEstimation;
}

void assignValueToList(estimation_t residentEstimation[], int index, int *elementsInArray, char *medicationValue, char *dosageValue){
    // If it already exits increment by 14 for returned index from function
    if( index >= 0){
        residentEstimation[index].biweeklyEstimation += 14;
    } else{ // if new to list add medicine name, dosage and add 14 to biweeklyEstimation

        strcpy(residentEstimation[*elementsInArray].medicationName,medicationValue);
        strcpy(residentEstimation[*elementsInArray].dosage,dosageValue);
        residentEstimation[*elementsInArray].biweeklyEstimation = 14;
        ++(*elementsInArray);
    }
}

int addToList(char* medicationValue,char* dosageValue, estimation_t residentEstimation[], int elementsInArray){

    for(int i = 0; i < elementsInArray; ++i){
        // check if medication name and dosage from json file exists in residentEstimation
        if(strcmp(medicationValue, residentEstimation[i].medicationName) == 0 && strcmp(dosageValue, residentEstimation[i].dosage) ==  0)
            return i;
    }

    return -1;
}


medicine_t* getMedcineInfo(){

    medicine_t* quantityList = malloc(SIZE_OF_MEDICINE * sizeof(estimation_t));

    char* medicineArray = ReadDB("medicine.json");
    int lengthMedicine = GetDBArrayLength(medicineArray);


    // iterate through each medicine
    for(int i = 0; i < lengthMedicine; ++i){
        // get json object with medicine
        char *medicine = GetDBEntry(medicineArray, i);


        // acces specific keys from the object
        char *medicationName = GetDBItem(medicine, "name");
        char *dosage = GetDBItem(medicine, "dosage");
        char *quantity = GetDBItem(medicine, "quantity");


        // Assign to array
        strcpy(quantityList[i].medicationName, medicationName);
        // Convert to medicine name to lowercase
        convertMedicationNameToLowercase(&quantityList[i]);
        strcpy(quantityList[i].dosage, dosage);
        strcpy(quantityList[i].quantity, quantity);

    }

    return quantityList;
}

void convertMedicationNameToLowercase(medicine_t *med) {
    char *str = med->medicationName;
    *str = tolower(*str);
}