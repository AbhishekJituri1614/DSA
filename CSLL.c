#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Weather {
    int station_id;
    char location[50];
    float temperature;
    float pressure;
    struct Weather *next;
} Weather;

Weather *head = NULL;

// Function prototypes
void addStation(int id, char *location, float temperature, float pressure);
void deleteStation(int id);
void searchStation(int id);
void displayStations();
void saveToFile();
void loadFromFile();

// ADD STATION
void addStation(int id, char *location, float temperature, float pressure) {
    Weather *new = (Weather*)malloc(sizeof(Weather));
    if (new == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    new->station_id = id;
    strcpy(new->location, location);
    new->temperature = temperature;
    new->pressure = pressure;

    // Case 1: Empty list
    if (head == NULL) {
        new->next = new;  // Points to itself (circular)
        head = new;
        printf("Weather Station Added (First station)\n");
        return;
    }

    // Case 2: Insert at the end (maintaining circular nature)
    Weather *temp = head;
    while (temp->next != head) {
        temp = temp->next;
    }
    temp->next = new;
    new->next = head;

    printf("Weather Station Added Successfully!\n");
    printf("Station ID: %d, Location: %s, Temp: %.2f°C, Pressure: %.2f hPa\n",
           id, location, temperature, pressure);
}

// DELETE STATION
void deleteStation(int id) {
    if (head == NULL) {
        printf("List is empty! No stations to delete.\n");
        return;
    }

    Weather *temp = head;
    Weather *prev = NULL;

    // Case 1: Only one station in the list
    if (head->next == head && head->station_id == id) {
        free(head);
        head = NULL;
        printf("Station with ID %d deleted successfully.\n", id);
        return;
    }

    // Case 2: Delete head node (but more than one node exists)
    if (head->station_id == id) {
        // Find the last node
        Weather *last = head;
        while (last->next != head) {
            last = last->next;
        }

        // Update head and last node's next
        head = head->next;
        last->next = head;
        free(temp);
        printf("Station with ID %d deleted successfully.\n", id);
        return;
    }

    // Case 3: Delete middle or last node
    while (temp->next != head && temp->next->station_id != id) {
        temp = temp->next;
    }

    if (temp->next->station_id == id) {
        Weather *toDel = temp->next;
        temp->next = toDel->next;
        free(toDel);
        printf("Station with ID %d deleted successfully.\n", id);
    } else {
        printf("Station with ID %d not found.\n", id);
    }
}

// SEARCH STATION
void searchStation(int id) {
    if (head == NULL) {
        printf("List is empty! No stations to search.\n");
        return;
    }

    Weather *temp = head;
    int found = 0;
    int position = 1;

    do {
        if (temp->station_id == id) {
            printf("\n========== WEATHER STATION FOUND ==========\n");
            printf("Station ID      : %d\n", temp->station_id);
            printf("Location        : %s\n", temp->location);
            printf("Temperature     : %.2f°C\n", temp->temperature);
            printf("Pressure        : %.2f hPa\n", temp->pressure);
            printf("Position in list: %d\n", position);
            printf("===========================================\n");
            found = 1;
            break;
        }
        temp = temp->next;
        position++;
    } while (temp != head);

    if (!found) {
        printf("Station with ID %d not found in the system.\n", id);
    }
}

// DISPLAY ALL STATIONS
void displayStations() {
    if (head == NULL) {
        printf("No weather stations in the system.\n");
        return;
    }

    Weather *temp = head;
    int count = 1;

    printf("\n========================================================\n");
    printf("              WEATHER MONITORING SYSTEM                  \n");
    printf("========================================================\n");
    printf("S.No\tStation ID\tLocation\tTemp(°C)\tPressure(hPa)\n");
    printf("========================================================\n");

    do {
        printf("%d\t%d\t\t%s\t\t%.2f\t\t%.2f\n",
               count++,
               temp->station_id,
               temp->location,
               temp->temperature,
               temp->pressure);
        temp = temp->next;
    } while (temp != head);

    printf("========================================================\n");
    printf("Total Stations: %d\n", count - 1);
    printf("========================================================\n");
}

// SAVE TO FILE
void saveToFile() {
    FILE *fp = fopen("weather_data.txt", "w");

    if (fp == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    if (head == NULL) {
        fprintf(fp, "0\n");  // Save empty list indicator
        fclose(fp);
        printf("No stations to save. File saved as empty.\n");
        return;
    }

    Weather *temp = head;
    int count = 0;

    // First, count the number of stations
    do {
        count++;
        temp = temp->next;
    } while (temp != head);

    // Save the count
    fprintf(fp, "%d\n", count);

    // Save all stations
    temp = head;
    do {
        fprintf(fp, "%d|%s|%.2f|%.2f\n",
                temp->station_id,
                temp->location,
                temp->temperature,
                temp->pressure);
        temp = temp->next;
    } while (temp != head);

    fclose(fp);
    printf("Weather data saved successfully! (%d stations saved)\n", count);
}

// LOAD FROM FILE
void loadFromFile() {
    FILE *fp = fopen("weather_data.txt", "r");

    if (fp == NULL) {
        printf("No existing data file found. Starting fresh.\n");
        return;
    }

    int count;
    if (fscanf(fp, "%d\n", &count) != 1) {
        fclose(fp);
        return;
    }

    if (count == 0) {
        printf("File is empty. Starting fresh.\n");
        fclose(fp);
        return;
    }

    int id;
    char location[50];
    float temperature, pressure;

    for (int i = 0; i < count; i++) {
        if (fscanf(fp, "%d|%49[^|]|%f|%f\n", &id, location, &temperature, &pressure) == 4) {
            addStation(id, location, temperature, pressure);
        }
    }

    fclose(fp);
    printf("Weather data loaded successfully! (%d stations loaded)\n", count);
}

// MAIN FUNCTION
int main() {
    loadFromFile();

    int choice, id;
    char location[50];
    float temperature, pressure;

    do {
        printf("\n========================================================\n");
        printf("         WEATHER MONITORING SYSTEM (CSLL)               \n");
        printf("========================================================\n");
        printf("1. Add Weather Station\n");
        printf("2. Delete Weather Station\n");
        printf("3. Search Weather Station\n");
        printf("4. Display All Stations\n");
        printf("5. Save & Exit\n");
        printf("========================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Consume newline

        switch(choice) {
            case 1:
                printf("\n--- Add New Weather Station ---\n");
                printf("Enter Station ID: ");
                scanf("%d", &id);
                getchar();

                printf("Enter Location: ");
                fgets(location, sizeof(location), stdin);
                location[strcspn(location, "\n")] = '\0';

                printf("Enter Temperature (°C): ");
                scanf("%f", &temperature);

                printf("Enter Pressure (hPa): ");
                scanf("%f", &pressure);

                addStation(id, location, temperature, pressure);
                break;

            case 2:
                printf("\n--- Delete Weather Station ---\n");
                printf("Enter Station ID to delete: ");
                scanf("%d", &id);
                deleteStation(id);
                break;

            case 3:
                printf("\n--- Search Weather Station ---\n");
                printf("Enter Station ID to search: ");
                scanf("%d", &id);
                searchStation(id);
                break;

            case 4:
                displayStations();
                break;

            case 5:
                saveToFile();
                printf("Exiting Weather Monitoring System...\n");
                break;

            default:
                printf("Invalid choice! Please enter a number between 1-5.\n");
        }
    } while(choice != 5);

    return 0;
}
