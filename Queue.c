#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node structure
struct Customer {
    int token;
    char name[50];
    struct Customer *next;
};

struct Customer *front = NULL, *rear = NULL;

// ENQUEUE
void enqueue(int token, char name[]) {
    struct Customer *newNode = (struct Customer*)malloc(sizeof(struct Customer));
    newNode->token = token;
    strcpy(newNode->name, name);
    newNode->next = NULL;

    if (rear == NULL) {
        front = rear = newNode;
        return;
    }

    rear->next = newNode;
    rear = newNode;
}

// DEQUEUE
void dequeue() {
    if (front == NULL) {
        printf("Queue is empty\n");
        return;
    }

    struct Customer *temp = front;
    printf("Serving: Token %d, Name %s\n", temp->token, temp->name);

    front = front->next;

    if (front == NULL)
        rear = NULL;

    free(temp);
}

// DISPLAY
void display() {
    if (front == NULL) {
        printf("Queue is empty\n");
        return;
    }

    struct Customer *temp = front;
    printf("\nQueue:\n");
    while (temp != NULL) {
        printf("Token: %d, Name: %s\n", temp->token, temp->name);
        temp = temp->next;
    }
}

// SAVE
void save() {
    FILE *fp = fopen("queue.txt", "w");
    if (!fp) return;

    struct Customer *temp = front;
    while (temp != NULL) {
        fprintf(fp, "%d|%s\n", temp->token, temp->name);
        temp = temp->next;
    }

    fclose(fp);
}

// LOAD
void load() {
    FILE *fp = fopen("queue.txt", "r");
    if (!fp) return;

    int token;
    char name[50];

    while (fscanf(fp, "%d|%49[^\n]\n", &token, name) == 2) {
        enqueue(token, name);
    }

    fclose(fp);
}

// MAIN
int main() {
    int ch, token;
    char name[50];

    load();

    do {
        printf("\n1.Add Customer\n2.Serve Customer\n3.Display\n4.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);
        getchar();

        switch(ch) {
            case 1:
                printf("Enter Token: ");
                scanf("%d", &token);
                getchar();

                printf("Enter Name: ");
                fgets(name, 50, stdin);
                name[strcspn(name, "\n")] = '\0';

                enqueue(token, name);
                break;

            case 2:
                dequeue();
                break;

            case 3:
                display();
                break;

            case 4:
                save();
                printf("Saved & Exiting\n");
                break;

            default:
                printf("Invalid choice\n");
        }

    } while(ch != 4);

    return 0;
}
