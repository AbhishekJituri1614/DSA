#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song {
    int song_id;
    char title[100];
    char artist[50];
    int duration;  // in seconds
    struct Song *prev;
    struct Song *next;
} Song;

Song *head = NULL;
Song *tail = NULL;

// Function prototypes
void addSong(int id, char *title, char *artist, int duration);
void removeSong(int id);
void searchSong(int id);
void displayForward();
void displayBackward();
void saveToFile();
void loadFromFile();

// ADD SONG
void addSong(int id, char *title, char *artist, int duration) {
    // Check if song with same ID already exists
    if (head != NULL) {
        Song *temp = head;
        do {
            if (temp->song_id == id) {
                printf("Error: Song with ID %d already exists!\n", id);
                return;
            }
            temp = temp->next;
        } while (temp != head);
    }

    Song *newSong = (Song*)malloc(sizeof(Song));
    if (newSong == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    newSong->song_id = id;
    strcpy(newSong->title, title);
    strcpy(newSong->artist, artist);
    newSong->duration = duration;

    // Case 1: Empty list
    if (head == NULL) {
        newSong->prev = newSong;
        newSong->next = newSong;
        head = tail = newSong;
        printf("Song added successfully! (First song in playlist)\n");
        return;
    }

    // Case 2: Add at the end (maintaining circular nature)
    newSong->prev = tail;
    newSong->next = head;
    tail->next = newSong;
    head->prev = newSong;
    tail = newSong;

    printf("Song added successfully!\n");
    printf("ID: %d, Title: %s, Artist: %s, Duration: %d:%02d\n",
           id, title, artist, duration / 60, duration % 60);
}

// REMOVE SONG
void removeSong(int id) {
    if (head == NULL) {
        printf("Playlist is empty! No songs to delete.\n");
        return;
    }

    Song *temp = head;
    int found = 0;

    // Search for the song
    do {
        if (temp->song_id == id) {
            found = 1;
            break;
        }
        temp = temp->next;
    } while (temp != head);

    if (!found) {
        printf("Song with ID %d not found in playlist.\n", id);
        return;
    }

    // Case 1: Only one node
    if (head == tail && head == temp) {
        free(temp);
        head = tail = NULL;
        printf("Song with ID %d deleted successfully. Playlist is now empty.\n", id);
        return;
    }

    // Case 2: Delete head node
    if (temp == head) {
        head = head->next;
        tail->next = head;
        head->prev = tail;
        free(temp);
        printf("Song with ID %d deleted successfully.\n", id);
        return;
    }

    // Case 3: Delete tail node
    if (temp == tail) {
        tail = tail->prev;
        tail->next = head;
        head->prev = tail;
        free(temp);
        printf("Song with ID %d deleted successfully.\n", id);
        return;
    }

    // Case 4: Delete middle node
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    free(temp);
    printf("Song with ID %d deleted successfully.\n", id);
}

// SEARCH SONG
void searchSong(int id) {
    if (head == NULL) {
        printf("Playlist is empty! No songs to search.\n");
        return;
    }

    Song *temp = head;
    int position = 1;
    int found = 0;

    do {
        if (temp->song_id == id) {
            printf("\n========== SONG FOUND ==========\n");
            printf("Song ID         : %d\n", temp->song_id);
            printf("Title           : %s\n", temp->title);
            printf("Artist          : %s\n", temp->artist);
            printf("Duration        : %d:%02d\n", temp->duration / 60, temp->duration % 60);
            printf("Position        : %d (from start)\n", position);

            // Count from end
            Song *back = tail;
            int backPos = 1;
            while (back != temp) {
                backPos++;
                back = back->prev;
            }
            printf("Position from end: %d\n", backPos);
            printf("================================\n");
            found = 1;
            break;
        }
        temp = temp->next;
        position++;
    } while (temp != head);

    if (!found) {
        printf("Song with ID %d not found in playlist.\n", id);
    }
}

// DISPLAY FORWARD (Circular - shows all songs once)
void displayForward() {
    if (head == NULL) {
        printf("Playlist is empty! No songs to display.\n");
        return;
    }

    Song *temp = head;
    int count = 1;
    int totalDuration = 0;

    printf("\n================================================================================\n");
    printf("                    MUSIC PLAYLIST (Forward - Circular)                          \n");
    printf("================================================================================\n");
    printf("S.No\tSong ID\tTitle\t\t\tArtist\t\tDuration\n");
    printf("================================================================================\n");

    do {
        printf("%d\t%d\t%-25s\t%-15s\t%d:%02d\n",
               count++,
               temp->song_id,
               temp->title,
               temp->artist,
               temp->duration / 60,
               temp->duration % 60);
        totalDuration += temp->duration;
        temp = temp->next;
    } while (temp != head);

    printf("================================================================================\n");
    printf("Total Songs: %d\n", count - 1);
    printf("Total Duration: %d:%02d\n", totalDuration / 60, totalDuration % 60);
    printf("================================================================================\n");
}

// DISPLAY BACKWARD (Circular - shows all songs once in reverse)
void displayBackward() {
    if (tail == NULL) {
        printf("Playlist is empty! No songs to display.\n");
        return;
    }

    Song *temp = tail;
    int count = 1;
    int totalDuration = 0;

    printf("\n================================================================================\n");
    printf("               MUSIC PLAYLIST (Backward - Circular Reverse)                     \n");
    printf("================================================================================\n");
    printf("S.No\tSong ID\tTitle\t\t\tArtist\t\tDuration\n");
    printf("================================================================================\n");

    do {
        printf("%d\t%d\t%-25s\t%-15s\t%d:%02d\n",
               count++,
               temp->song_id,
               temp->title,
               temp->artist,
               temp->duration / 60,
               temp->duration % 60);
        totalDuration += temp->duration;
        temp = temp->prev;
    } while (temp != tail);

    printf("================================================================================\n");
    printf("Total Songs: %d\n", count - 1);
    printf("Total Duration: %d:%02d\n", totalDuration / 60, totalDuration % 60);
    printf("================================================================================\n");
}

// SAVE TO FILE
void saveToFile() {
    FILE *fp = fopen("playlist.txt", "w");

    if (fp == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    if (head == NULL) {
        fprintf(fp, "0\n");
        fclose(fp);
        printf("No songs to save. File saved as empty.\n");
        return;
    }

    Song *temp = head;
    int count = 0;

    // Count total songs
    do {
        count++;
        temp = temp->next;
    } while (temp != head);

    // Save count
    fprintf(fp, "%d\n", count);

    // Save all songs
    temp = head;
    do {
        fprintf(fp, "%d|%s|%s|%d\n",
                temp->song_id,
                temp->title,
                temp->artist,
                temp->duration);
        temp = temp->next;
    } while (temp != head);

    fclose(fp);
    printf("Playlist saved successfully! (%d songs saved)\n", count);
}

// LOAD FROM FILE
void loadFromFile() {
    FILE *fp = fopen("playlist.txt", "r");

    if (fp == NULL) {
        printf("No existing playlist found. Starting fresh.\n");
        return;
    }

    int count;
    if (fscanf(fp, "%d\n", &count) != 1) {
        fclose(fp);
        return;
    }

    if (count == 0) {
        printf("Playlist file is empty. Starting fresh.\n");
        fclose(fp);
        return;
    }

    int id, duration;
    char title[100];
    char artist[50];

    for (int i = 0; i < count; i++) {
        if (fscanf(fp, "%d|%99[^|]|%49[^|]|%d\n", &id, title, artist, &duration) == 4) {
            // Add song directly during load
            Song *newSong = (Song*)malloc(sizeof(Song));
            if (newSong == NULL) {
                printf("Memory allocation failed during load!\n");
                continue;
            }

            newSong->song_id = id;
            strcpy(newSong->title, title);
            strcpy(newSong->artist, artist);
            newSong->duration = duration;

            if (head == NULL) {
                newSong->prev = newSong;
                newSong->next = newSong;
                head = tail = newSong;
            } else {
                newSong->prev = tail;
                newSong->next = head;
                tail->next = newSong;
                head->prev = newSong;
                tail = newSong;
            }
        }
    }

    fclose(fp);
    printf("Playlist loaded successfully! (%d songs loaded)\n", count);
}

// MAIN FUNCTION
int main() {
    loadFromFile();

    int choice, id, duration;
    char title[100];
    char artist[50];

    do {
        printf("\n==========================================================================\n");
        printf("           MUSIC PLAYLIST MANAGEMENT SYSTEM (Circular DLL)                 \n");
        printf("==========================================================================\n");
        printf("1. Add Song\n");
        printf("2. Remove Song\n");
        printf("3. Search Song\n");
        printf("4. Display Playlist (Forward)\n");
        printf("5. Display Playlist (Backward)\n");
        printf("6. Save & Exit\n");
        printf("==========================================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Consume newline

        switch(choice) {
            case 1:
                printf("\n--- Add New Song ---\n");
                printf("Enter Song ID: ");
                scanf("%d", &id);
                getchar();

                printf("Enter Song Title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';

                printf("Enter Artist Name: ");
                fgets(artist, sizeof(artist), stdin);
                artist[strcspn(artist, "\n")] = '\0';

                printf("Enter Duration (in seconds): ");
                scanf("%d", &duration);

                addSong(id, title, artist, duration);
                break;

            case 2:
                printf("\n--- Remove Song ---\n");
                printf("Enter Song ID to remove: ");
                scanf("%d", &id);
                removeSong(id);
                break;

            case 3:
                printf("\n--- Search Song ---\n");
                printf("Enter Song ID to search: ");
                scanf("%d", &id);
                searchSong(id);
                break;

            case 4:
                displayForward();
                break;

            case 5:
                displayBackward();
                break;

            case 6:
                saveToFile();
                printf("Exiting Music Playlist System...\n");
                break;

            default:
                printf("Invalid choice! Please enter a number between 1-6.\n");
        }
    } while(choice != 6);

    // Free memory before exit
    if (head != NULL) {
        Song *temp = head;
        Song *next;
        do {
            next = temp->next;
            free(temp);
            temp = next;
        } while (temp != head);
    }

    return 0;
}
