#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file consigneeVehicles.c
 * @brief Implementation of a program that manages consignee vehicles.
 * 
 * This file contains the definition of a struct that represents a vehicle,
 * along with its attributes such as number plate, brand, model, year, color,
 * value, state, and type. The struct is named Vehicle.
 */
typedef struct {
    char numberPlate[6];
    char brand[20];
    char model[20];
    int year;
    char color[20];
    double value;
    char state; // A for active, E for inactive
    char type; // P for ownend, C for consigned
} Vehicle;

/**
 * Searches for a vehicle in the main file by its number plate.
 *
 * @param mainFile The main file containing the vehicle records.
 * @param numberPlate The number plate of the vehicle to search for.
 * @param returnAll Flag indicating whether to return all matching vehicles or just the first one found.
 *                  Set to 1 to return all matching vehicles, or 0 to return only the first one found.
 * @return A pointer to the first matching vehicle found, or NULL if no matching vehicle is found.
 */
Vehicle* searchVehicleByNumberPlate(FILE* mainFile, char numberPlate[7], int returnAll);
/**
 * Searches for a vehicle within a given value range.
 *
 * This function searches for a vehicle in the specified main file
 * that has a value within the given minimum and maximum values.
 *
 * @param mainFile The main file containing the vehicle data.
 * @param minValue The minimum value for the vehicle.
 * @param maxValue The maximum value for the vehicle.
 * @return A pointer to the found vehicle, or NULL if no vehicle is found.
 */
Vehicle* searchVehicleByValueRange(FILE* mainFile, double minValue, double maxValue);
/**
 * Searches for a vehicle in the main file by brand and model.
 *
 * @param mainFile The main file containing the vehicle records.
 * @param brand The brand of the vehicle to search for.
 * @param model The model of the vehicle to search for.
 * @return A pointer to the found vehicle, or NULL if not found.
 */
Vehicle* searchVehicleByBrandAndModel(FILE* mainFile, char brand[20], char model[20]);

/**
 * Searches for a vehicle of a specific type in the given file.
 *
 * @param mainFile The file to search in.
 * @param type The type of vehicle to search for(P own, C consigned).
 * @return A pointer to the found vehicle, or NULL if not found.
 */
Vehicle* searchVehicleByType(FILE* mainFile, char type);

/**
 * Searches for a vehicle by state in the given file.
 *
 * @param mainFile The file to search in.
 * @param state The state to search for.
 * @return A pointer to the found vehicle, or NULL if not found.
 */
Vehicle* searchVehicleByState(FILE* mainFile, char state);


/**
 * @brief Updates the information of a vehicle in the main file.
 *
 * This function updates the value and state of a vehicle with the specified number plate
 * in the main file.
 *
 * @param mainFile The pointer to the main file.
 * @param numberPlate The number plate of the vehicle to update.
 * @param value The new value of the vehicle.
 * @param state The new state of the vehicle.
 * @return int Returns 0 if the vehicle was successfully updated, otherwise returns -1.
 */
int updateVehicle(FILE* mainFile, char numberPlate[7], double value, char state);


/**
 * @brief Removes a vehicle from the main file based on its number plate.
 * 
 * @param mainFile The file pointer to the main file.
 * @param numberPlate The number plate of the vehicle to be removed.
 * @return int Returns 0 if the vehicle was successfully removed, -1 otherwise.
 */
int removeVehicle(FILE* mainFile, char numberPlate[7]);

/**
 * Inserts a vehicle record into the main file.
 *
 * This function appends the given vehicle record to the end of the main file.
 *
 * @param mainFile The file pointer to the main file.
 * @param vehicle The vehicle record to be inserted.
 * @return Returns 1 if the insertion is successful, otherwise returns 0.
 */
int insertVehicle(FILE* mainFile, Vehicle *vehicle);


/**
 * Reads user input for a Vehicle object, including spaces.
 *
 * @param vehicle The pointer to the Vehicle object to store the user input.
 */
void readUserInputWithSpaces(Vehicle *vehicle, FILE* mainFile);

/**
 * Clears the screen.
 * Valid for both Windows and Unix systems.
 */
void clearScreen();


Vehicle* searchVehicleByNumberPlate(FILE* mainFile, char numberPlate[6], int returnAll) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fseek(mainFile, 0, SEEK_SET);
    while(fread(vehicle, sizeof(Vehicle), 1, mainFile)) {
        if (returnAll) {
            if(strncmp(vehicle->numberPlate, numberPlate, 6) == 0) {
                return vehicle;
            }
        } else {
            if(strncmp(vehicle->numberPlate, numberPlate, 6) == 0 && vehicle->state == 'A') {
                return vehicle;
            }
        }
    }
    return NULL;
}


Vehicle* searchVehicleByValueRange(FILE* mainFile, double minValue, double maxValue) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fread(vehicle, sizeof(Vehicle), 1, mainFile);
    if(vehicle->value >= minValue && vehicle->value <= maxValue) {
            return vehicle;
    }
    return NULL;
}

Vehicle* searchVehicleByBrandAndModel(FILE* mainFile, char brand[20], char model[20]) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fread(vehicle, sizeof(Vehicle), 1, mainFile);
    if(vehicle->state == 'A' && strncmp(vehicle->brand, brand, 20) == 0 && strncmp(vehicle->model, model, 20) == 0 ){
        return vehicle;
    }
    return NULL;
}

//Get total of consignee vehicles and own vehicles and his total value
void getTotal(FILE* mainFile) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fseek(mainFile, 0, SEEK_END);
    int numberOfVehicles = ftell(mainFile) / sizeof(Vehicle);
    int consigned = 0;
    int owned = 0;

    double consignedValue = 0;
    double ownedValue = 0;
    fseek(mainFile, 0, SEEK_SET);
    for (int i = 0; i < numberOfVehicles; i++) {
        fread(vehicle, sizeof(Vehicle), 1, mainFile);
        if(vehicle->state == 'A') {
            if(vehicle->type == 'C') {
                consigned++;
                consignedValue += vehicle->value;
            } else {
                owned++;
                ownedValue += vehicle->value;
            }
        }
    }
    printf("Consigned vehicles: %d\n", consigned);
    printf("Owned vehicles: %d\n", owned);
    printf("Consigned vehicles total value: %.2lf\n", consignedValue);
    printf("Owned vehicles total value: %.2lf\n", ownedValue);
    free(vehicle);


}

Vehicle* searchVehicleByType(FILE* mainFile, char type) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fseek(mainFile, 0, SEEK_SET);
    fread(vehicle, sizeof(Vehicle), 1, mainFile);
    if(vehicle->type == type) {
            return vehicle;
    }
    return NULL;
}

Vehicle* searchVehicleByState(FILE* mainFile, char state) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fread(vehicle, sizeof(Vehicle), 1, mainFile);
    if(vehicle->state == state) {
            return vehicle;
    }
    return NULL;
}

int updateVehicle(FILE* mainFile, char numberPlate[6], double value, char state) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fseek(mainFile, 0, SEEK_SET);
    while(fread(vehicle, sizeof(Vehicle), 1, mainFile)) {
        if(strncmp(vehicle->numberPlate, numberPlate, 6) == 0) {
            vehicle->value = value;
            vehicle->state = state;
            fseek(mainFile, -sizeof(Vehicle), SEEK_CUR);
            fwrite(vehicle, sizeof(Vehicle), 1, mainFile);
            return 1;
        }
    }
    return 0;
}

int removeVehicle(FILE* mainFile, char numberPlate[6]) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fseek(mainFile, 0, SEEK_SET);
    while(fread(vehicle, sizeof(Vehicle), 1, mainFile)) {
        if(strncmp(vehicle->numberPlate, numberPlate, 6) == 0) {
            vehicle->state = 'E';
            fseek(mainFile, -sizeof(Vehicle), SEEK_CUR);
            fwrite(vehicle, sizeof(Vehicle), 1, mainFile);
            return 1;
        }
    }
    return 0;
}

int insertVehicle(FILE* mainFile, Vehicle *vehicle) {
    fseek(mainFile, 0, SEEK_END);
    fwrite(vehicle, sizeof(Vehicle), 1, mainFile);
    return 1;
}

//If return 1 the vehicle is already in the system and if return 0 the vehicle is not in the system
int readUserInputWithSpaces(Vehicle *vehicle, FILE* mainFile) {
    getc(stdin);
    printf("Enter the number plate: ");
    fgets(vehicle->numberPlate, 7, stdin);
    //Validate if the vehicle is already in the system
    Vehicle* foundVehicle = searchVehicleByNumberPlate(mainFile, vehicle->numberPlate, 1);
    if(foundVehicle != NULL){
        printf("Vehicle already in the system\n");
        if (foundVehicle->state == 'A') {
            printf("Vehicle is active\n");
            return 1;
        } else {
          printf("Do you want to active the vehicle? (y/n): ");
          char choice;
          getc(stdin);
            getc(stdin);
          scanf("%c", &choice);
          if (choice == 'y') {
            updateVehicle(mainFile, foundVehicle->numberPlate, foundVehicle->value, foundVehicle->state);
          } else {
            return 1;
          }
        }
    }
    getc(stdin);
    printf("Enter the brand: ");
    fgets(vehicle->brand, sizeof(vehicle->brand), stdin);
    vehicle->brand[strcspn(vehicle->brand, "\n")] = '\0';
    printf("Enter the model: ");
    fgets(vehicle->model, sizeof(vehicle->model), stdin);
    vehicle->model[strcspn(vehicle->model, "\n")] = '\0';
    printf("Enter the year: ");
    scanf("%d", &(vehicle->year));
    getc(stdin); 
    printf("Enter the color: ");
    fgets(vehicle->color, sizeof(vehicle->color), stdin);
    vehicle->color[strcspn(vehicle->color, "\n")] = '\0';
    printf("Enter the value: ");
    scanf("%lf", &(vehicle->value));
    printf("Enter the state: ");
    scanf(" %c", &(vehicle->state));
    getchar(); 
    printf("Enter the type: ");
    scanf(" %c", &(vehicle->type));
    getchar();
    return 0;
}

void clearScreen() {
    #ifdef WINDOWS
        system("cls");
    #else
        system("clear");
    #endif
}


int main() {
    int option;
    do {
        FILE* mainFile = fopen("vehicles.dat", "r+b");
        if(mainFile == NULL) {
            mainFile = fopen("vehicles.dat", "w+b");
        }
        printf("Main menu:\n");
        printf("1 - Insert a vehicle\n");
        printf("2 - Search active vehicles by number plate\n");
        printf("3 - Search active vehicles by value range\n");
        printf("4 - Search active vehicles by brand and model\n");
        printf("5 - Search active vehicles by type\n");
        printf("6 - Search active vehicles by state\n");
        printf("7 - Update a vehicle (value, state)\n");
        printf("8 - Remove a vehicle logically\n");
        printf("9 - Get total\n");
        printf("10 - Exit\n");
        printf("Enter your option: ");
        scanf("%d", &option);

        switch(option) {
            case 1: {
                clearScreen();
                // Insert a vehicle
                Vehicle *vehicle = (Vehicle*) malloc(sizeof(Vehicle));
                int isInSystem = readUserInputWithSpaces(vehicle, mainFile);
                if (isInSystem) {
                    printf("Press enter to continue...");
                    fgetc(stdin);
                    clearScreen();
                    break;
                }
                insertVehicle(mainFile, vehicle);
                printf("Vehicle inserted successfully.\n");
                free(vehicle);
                printf("Press enter to continue...");
                fgetc(stdin);
                clearScreen();
                break;
            }
            case 2: {
                // Search active vehicles by number plate
                clearScreen();
                char numberPlate[7];
                printf("Enter the number plate: ");
                scanf("%s", numberPlate);
                Vehicle* vehicle = searchVehicleByNumberPlate(mainFile, numberPlate, 0);
                if(vehicle != NULL) {
                    clearScreen();
                    char *brand = (char*) malloc(sizeof(char) * 20);
                    char *model = (char*) malloc(sizeof(char) * 20);
                    char *numberPlate = (char*) malloc(sizeof(char) * 6);
                    strcpy(brand, vehicle->brand);
                    strcpy(model, vehicle->model);
                    strcpy(numberPlate, vehicle->numberPlate);
                    numberPlate[6] = '\0';
                    brand[19] = '\0';
                    model[19] = '\0';
                    printf("* Vehicle found *\n");
                    printf("Brand: %s\n", brand);
                    printf("Model: %s\n", model);
                    printf("Number Plate: %s\n", numberPlate);
                    printf("Year: %d\n", vehicle->year);
                    printf("Color: %s\n", vehicle->color);
                    printf("Value: %.2lf\n", vehicle->value);
                    printf("State: %c\n", vehicle->state);
                    printf("Type: %c\n\n", vehicle->type);
                    free(brand);
                    free(model);
                    free(numberPlate);
                } else {
                    printf("Vehicle not found\n");
                }
                printf("Press enter to continue...");
                fgetc(stdin);
                fgetc(stdin);
                clearScreen();
                break;
            }
            case 3: {
                // Search active vehicles by value range
                double minValue, maxValue;
                printf("Enter the minimum value: ");
                scanf("%lf", &minValue);
                printf("Enter the maximum value: ");
                scanf("%lf", &maxValue);

                fseek(mainFile, 0, SEEK_END);
                int numberOfVehicles = ftell(mainFile) / sizeof(Vehicle);
                int notFound = 1;


                fseek(mainFile, 0, SEEK_SET);
                Vehicle* vehicle = searchVehicleByValueRange(mainFile, minValue, maxValue);
                clearScreen();
                for (int i = 0; i < numberOfVehicles; i++) {
                    printf("%d\n", i);
                    if (vehicle != NULL) {
                        notFound = 0;
                        char *brand = (char*) malloc(sizeof(char) * 20);
                        char *model = (char*) malloc(sizeof(char) * 20);
                        char *numberPlate = (char*) malloc(sizeof(char) * 7);
                        strcpy(brand, vehicle->brand);
                        strcpy(model, vehicle->model);
                        strcpy(numberPlate, vehicle->numberPlate);
                        numberPlate[6] = '\0';
                        brand[19] = '\0';
                        model[19] = '\0';
                        printf("* Vehicle found *\n");
                        printf("Brand: %s\n", brand);
                        printf("Model: %s\n", model);
                        printf("Number Plate: %s\n", numberPlate);
                        printf("Year: %d\n", vehicle->year);
                        printf("Color: %s\n", vehicle->color);
                        printf("Value: %.2lf\n", vehicle->value);
                        printf("State: %c\n", vehicle->state);
                        printf("Type: %c\n\n", vehicle->type);
                        free(brand);
                        free(model);
                        free(numberPlate);
                    }
                    vehicle = searchVehicleByValueRange(mainFile, minValue, maxValue);
                }
                if(notFound) {
                    printf("No vehicles found\n");
                }

                printf("Press enter to continue...");
                getc(stdin);
                getc(stdin);
                clearScreen()
;                break;
            }
            case 4: {
                clearScreen();
                // Search active vehicles by brand and model
                char brand[20], model[20];
                printf("Enter the brand: ");
                scanf("%s", brand);
                printf("Enter the model: ");
                scanf("%s", model);

                fseek(mainFile, 0, SEEK_END);
                int numberOfVehicles = ftell(mainFile) / sizeof(Vehicle);
                printf("Number of vehicles: %d\n", numberOfVehicles);
                fseek(mainFile, 0, SEEK_SET);
                int notFound = 1;
                Vehicle* vehicle = searchVehicleByBrandAndModel(mainFile, brand, model);
                for (int i = 0; i < numberOfVehicles; i++) {
                    printf("%d\n", i);
                    if (vehicle != NULL) {
                        notFound = 0;
                        char *brand = (char*) malloc(sizeof(char) * 20);
                        char *model = (char*) malloc(sizeof(char) * 20);
                        char *numberPlate = (char*) malloc(sizeof(char) * 7);
                        strcpy(brand, vehicle->brand);
                        strcpy(model, vehicle->model);
                        strcpy(numberPlate, vehicle->numberPlate);
                        numberPlate[6] = '\0';
                        brand[19] = '\0';
                        model[19] = '\0';
                        printf("* Vehicle found *\n");
                        printf("Brand: %s\n", brand);
                        printf("Model: %s\n", model);
                        printf("Number Plate: %s\n\n", numberPlate);
                        free(brand);
                        free(model);
                        free(numberPlate);
                    }
                    vehicle = searchVehicleByBrandAndModel(mainFile, brand, model);
                }
                if(notFound) {
                    printf("No vehicles found\n");
                }
                printf("Press enter to continue...");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
            }
            case 5: {
                // Search active vehicles by type
                clearScreen();
                char type;
                printf("Enter the type: ");
                scanf(" %c", &type);
                if (type != 'P' && type != 'C') {
                    printf("Invalid type. Please try again.\n");
                    printf("Press enter to continue...");
                    getc(stdin);
                    getc(stdin);
                    clearScreen();
                    break;
                }
                
                fseek(mainFile, 0, SEEK_END);
                int numberOfVehicles = ftell(mainFile) / sizeof(Vehicle);
                fseek(mainFile, 0, SEEK_SET);
                int notFound = 1;
                Vehicle* vehicle = searchVehicleByType(mainFile, type);
                for (int i = 0; i < numberOfVehicles; i++) {
                    if (vehicle != NULL) {
                        notFound = 0;
                        char *brand = (char*) malloc(sizeof(char) * 20);
                        char *model = (char*) malloc(sizeof(char) * 20);
                        char *numberPlate = (char*) malloc(sizeof(char) * 7);
                        strcpy(brand, vehicle->brand);
                        strcpy(model, vehicle->model);
                        strcpy(numberPlate, vehicle->numberPlate);
                        numberPlate[6] = '\0';
                        brand[19] = '\0';
                        model[19] = '\0';
                        printf("* Vehicle found *\n");
                        printf("Brand: %s\n", brand);
                        printf("Model: %s\n", model);
                        printf("Number Plate: %s\n", numberPlate);
                        printf("Year: %d\n", vehicle->year);
                        printf("Color: %s\n", vehicle->color);
                        printf("Value: %.2lf\n", vehicle->value);
                        printf("State: %c\n", vehicle->state);
                        printf("Type: %c\n\n", vehicle->type);
                        free(brand);
                        free(model);
                        free(numberPlate);
                    }
                    vehicle = searchVehicleByType(mainFile, type);
                }
                if(notFound) {
                    printf("No vehicles found\n");
                }
                printf("Press enter to continue...");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
            }
            case 6: {
                // Search active vehicles by state
                clearScreen();
                char state;
                printf("Enter the state: ");
                scanf(" %c", &state);
                if (state != 'A' && state != 'E') {
                    printf("Invalid state. Please try again.\n");
                    printf("Press enter to continue...");
                    getc(stdin);
                    getc(stdin);
                    clearScreen();
                    break;
                }

                fseek(mainFile, 0, SEEK_END);
                int numberOfVehicles = ftell(mainFile) / sizeof(Vehicle);
                fseek(mainFile, 0, SEEK_SET);
                int notFound = 1;
                Vehicle* vehicle = searchVehicleByState(mainFile, state);
                for (int i = 0; i < numberOfVehicles; i++) {
                    if (vehicle != NULL) {
                        notFound = 0;
                        char *brand = (char*) malloc(sizeof(char) * 20);
                        char *model = (char*) malloc(sizeof(char) * 20);
                        char *numberPlate = (char*) malloc(sizeof(char) * 7);
                        strcpy(brand, vehicle->brand);
                        strcpy(model, vehicle->model);
                        strcpy(numberPlate, vehicle->numberPlate);
                        numberPlate[6] = '\0';
                        brand[19] = '\0';
                        model[19] = '\0';
                        printf("* Vehicle found *\n");
                        printf("Brand: %s\n", brand);
                        printf("Model: %s\n", model);
                        printf("Number Plate: %s\n", numberPlate);
                        printf("Year: %d\n", vehicle->year);
                        printf("Color: %s\n", vehicle->color);
                        printf("Value: %.2lf\n", vehicle->value);
                        printf("State: %c\n", vehicle->state);
                        printf("Type: %c\n\n", vehicle->type);
                        free(brand);
                        free(model);
                        free(numberPlate);
                    }
                    vehicle = searchVehicleByState(mainFile, state);
                }
                if(notFound) {
                    printf("No vehicles found\n");
                }
                printf("Press enter to continue...");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
            }
            case 7: {
                clearScreen();
                // Update a vehicle (value, state)
                char numberPlate[7];
                double value;
                char state;
                printf("Enter the number plate: ");
                scanf("%s", numberPlate);
                if (searchVehicleByNumberPlate(mainFile, numberPlate, 1) == NULL) {
                    printf("Vehicle not found\n");
                    printf("Press enter to continue...");
                    getc(stdin);
                    getc(stdin);
                    clearScreen();
                    break;
                }
                fseek(mainFile, 0, SEEK_SET);
                printf("Enter the new value: ");
                scanf("%lf", &value);
                printf("Enter the new state: ");
                scanf(" %c", &state);

                int result = updateVehicle(mainFile, numberPlate, value, state);
                if(result) {
                    printf("Vehicle updated successfully.\n");
                } else {
                    printf("Vehicle not found\n");
                }
                printf("Press enter to continue...");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
            }

            case 8: {
                // Remove a vehicle logically
                clearScreen();
                char numberPlate[7];
                printf("Enter the number plate: ");
                scanf("%s", numberPlate);
                
                Vehicle* vehicle = searchVehicleByNumberPlate(mainFile, numberPlate, 1);
                if(vehicle == NULL) {
                    printf("Vehicle not found\n");
                    printf("Press enter to continue...");
                    getc(stdin);
                    getc(stdin);
                    clearScreen();
                    break;
                }
                fseek(mainFile, 0, SEEK_SET);
                int result = removeVehicle(mainFile, numberPlate);
                if(result) {
                    printf("Vehicle removed successfully.\n");
                } else {
                    printf("Vehicle not found\n");
                }
                printf("Press enter to continue...");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
            }
            //add get total to the main menu
            case 9: {
                clearScreen();
                getTotal(mainFile);
                printf("Press enter to continue...");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
            }
            case 10: {
                clearScreen();
                printf("Exiting...\n");
                return 0;
            }
            default:
                clearScreen();
                printf("Invalid option. Please try again.\n");
                printf("Press enter to continue...");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
        }

        fclose(mainFile);
    } while(option != 9);

    return 0;
}
