#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Help me to convert this program into a system to manage consignee vehicles, instead of employees

/**
 * @brief Definition of a struct that represents an Vehicle
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

// There is a main file called vehicles.dat, that contains all the vehicles

// Function to instert a vehicle in the file
// Return a vehicle by his number plate, if it exists, otherwise return NULL
// Return a vehicle by a range of values, minValue and maxValue
// Return a vehicle by his brand and model
// Return a vehicle by this type, P for owned, C for consigned
// Return a vehicle by his state, A for active, E for inactive
// All is in the main file called vehicles.dat

// Function declarations
// void insertVehicle(FILE* mainFile, Vehicle vehicle);
Vehicle* searchVehicleByNumberPlate(FILE* mainFile, char numberPlate[7]);
Vehicle* searchVehicleByValueRange(FILE* mainFile, double minValue, double maxValue);
Vehicle* searchVehicleByBrandAndModel(FILE* mainFile, char brand[20], char model[20]);
Vehicle* searchVehicleByType(FILE* mainFile, char type);
Vehicle* searchVehicleByState(FILE* mainFile, char state);
int updateVehicle(FILE* mainFile, char numberPlate[7], double value, char state);
int removeVehicle(FILE* mainFile, char numberPlate[7]);

int insertVehicle(FILE* mainFile, Vehicle *vehicle) {
    fseek(mainFile, 0, SEEK_END);
    fwrite(vehicle, sizeof(Vehicle), 1, mainFile);
    return 1;
}

Vehicle* searchVehicleByNumberPlate(FILE* mainFile, char numberPlate[6]) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fseek(mainFile, 0, SEEK_SET);
    while(fread(vehicle, sizeof(Vehicle), 1, mainFile)) {
        if(strncmp(vehicle->numberPlate, numberPlate, 6) == 0) {
            return vehicle->state == 'A' ? vehicle : NULL;
        }
    }
    return NULL;
}



// While the return be !== NULL, print the vehicle
Vehicle* searchVehicleByValueRange(FILE* mainFile, double minValue, double maxValue) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    while(fread(vehicle, sizeof(Vehicle), 1, mainFile)) {
        if(vehicle->value >= minValue && vehicle->value <= maxValue) {
            return vehicle;
        }
    }
    return NULL;
}

Vehicle* searchVehicleByBrandAndModel(FILE* mainFile, char brand[20], char model[20]) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fseek(mainFile, 0, SEEK_SET);
    while(fread(vehicle, sizeof(Vehicle), 1, mainFile)) {
        if(strncmp(vehicle->brand, brand, 20) == 0 && strncmp(vehicle->model, model, 20) == 0) {
            return vehicle;
        }
    }
    return NULL;
}

Vehicle* searchVehicleByType(FILE* mainFile, char type) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fseek(mainFile, 0, SEEK_SET);
    while(fread(vehicle, sizeof(Vehicle), 1, mainFile)) {
        if(vehicle->type == type) {
            return vehicle;
        }
    }
    return NULL;
}

Vehicle* searchVehicleByState(FILE* mainFile, char state) {
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    fseek(mainFile, 0, SEEK_SET);
    while(fread(vehicle, sizeof(Vehicle), 1, mainFile)) {
        if(vehicle->state == state) {
            return vehicle;
        }
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

// Function to read stream with blank spaces and save in a struct

void readUserInputWithSpaces(Vehicle *vehicle) {
    getc(stdin);
    printf("Enter the number plate: ");
    fgets(vehicle->numberPlate, 7, stdin);
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
        printf("9 - Exit\n");
        printf("Enter your option: ");
        scanf("%d", &option);

        switch(option) {
            case 1: {
                // Insert a vehicle
                Vehicle *vehicle = (Vehicle*) malloc(sizeof(Vehicle));
                readUserInputWithSpaces(vehicle);
                insertVehicle(mainFile, vehicle);
                printf("Vehicle inserted successfully.\n");
                free(vehicle);
                break;
            }
            case 2: {
                // Search active vehicles by number plate
                char numberPlate[7];
                printf("Enter the number plate: ");
                scanf("%s", numberPlate);

                Vehicle* vehicle = searchVehicleByNumberPlate(mainFile, numberPlate);
                if(vehicle != NULL) {
                    system("clear");
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
                    printf("Press enter to continue...");
                    getc(stdin);
                    getc(stdin);
                } else {
                    printf("Vehicle not found\n");
                }
                break;
            }
            case 3: {
                // Search active vehicles by value range
                double minValue, maxValue;
                printf("Enter the minimum value: ");
                scanf("%lf", &minValue);
                printf("Enter the maximum value: ");
                scanf("%lf", &maxValue);

                fseek(mainFile, 0, SEEK_SET);
                Vehicle* vehicle = searchVehicleByValueRange(mainFile, minValue, maxValue);
                system("clear");
                if (vehicle != NULL) {
                    while (vehicle != NULL) {
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
                        vehicle = searchVehicleByValueRange(mainFile, minValue, maxValue);
                    }
                } else {
                    printf("No vehicles found in the specified value range.\n");
                }
                printf("Press enter to continue...");
                getc(stdin);
                getc(stdin);
                system("clear");
                break;
            }
            case 4: {
                // Search active vehicles by brand and model
                char brand[20], model[20];
                printf("Enter the brand: ");
                scanf("%s", brand);
                printf("Enter the model: ");
                scanf("%s", model);

                Vehicle* vehicle = searchVehicleByBrandAndModel(mainFile, brand, model);
                if(vehicle != NULL) {
                    printf("Vehicle found: %s %s %s\n", vehicle->brand, vehicle->model, vehicle->numberPlate);
                } else {
                    printf("Vehicle not found\n");
                }
                break;
            }
            case 5: {
                // Search active vehicles by type
                char type;
                printf("Enter the type: ");
                scanf(" %c", &type);

                Vehicle* vehicle = searchVehicleByType(mainFile, type);
                if(vehicle != NULL) {
                    printf("Vehicle found: %s %s %s\n", vehicle->brand, vehicle->model, vehicle->numberPlate);
                } else {
                    printf("Vehicle not found\n");
                }
                break;
            }
            case 6: {
                // Search active vehicles by state
                char state;
                printf("Enter the state: ");
                scanf(" %c", &state);

                Vehicle* vehicle = searchVehicleByState(mainFile, state);
                if(vehicle != NULL) {
                    printf("Vehicle found: %s %s %s\n", vehicle->brand, vehicle->model, vehicle->numberPlate);
                } else {
                    printf("Vehicle not found\n");
                }
                break;
            }
            case 7: {
                // Update a vehicle (value, state)
                char numberPlate[7];
                double value;
                char state;
                printf("Enter the number plate: ");
                scanf("%s", numberPlate);
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
                break;
            }
            case 8: {
                // Remove a vehicle logically
                char numberPlate[7];
                printf("Enter the number plate: ");
                scanf("%s", numberPlate);

                int result = removeVehicle(mainFile, numberPlate);
                if(result) {
                    printf("Vehicle removed successfully.\n");
                } else {
                    printf("Vehicle not found\n");
                }
                break;
            }
            case 9: {
                // Exit
                printf("Exiting...\n");
                break;
            }
            default:
                printf("Invalid option. Please try again.\n");
        }
        fclose(mainFile);
    } while(option != 9);

    return 0;
}

// Main menu with the following options:
// 1 - Insert a vehicle
// 2 - Search actives vehicles by number plate
// 3 - Search actives vehicles by value range
// 4 - Search actives vehicles by brand and model
// 5 - Search actives vehicles by type
// 6 - Search actives vehicles by state
// 7 - Update a vehicle (value, state)
// 8 - Remove a vehicle logically
// 9 - Exit

