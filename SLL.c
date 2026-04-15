#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Using typedef to avoid writing 'struct' everywhere
typedef struct Student {
    int roll;
    char name[50];
    float marks;
    struct Student *next;
} Student;

Student *head = NULL;

// Function prototypes
void addStudent(int roll, char *name, float marks);
void saveToFile();
void loadFromFile();
void display();
void search(int roll);
void deleteStudent(int roll);

// ADD STUDENT
void addStudent(int roll, char *name, float marks) {
    Student *new1 = (Student*)malloc(sizeof(Student));

    if (new1 == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    new1->roll = roll;
    strcpy(new1->name, name);
    new1->marks = marks;
    new1->next = NULL;

    if (head == NULL) {
        head = new1;
    } else {
        Student *temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new1;
    }
    printf("Student Saved Successfully\n");
}

// SAVE TO FILE
void saveToFile() {
    FILE *fp = fopen("Student.txt", "w");
    if (fp == NULL) {
        printf("File opening error\n");
        return;
    }

    Student *temp = head;
    while (temp != NULL) {
        fprintf(fp, "%d,%s,%.2f\n", temp->roll, temp->name, temp->marks);
        temp = temp->next;
    }
    fclose(fp);
    printf("Data Saved\n");
}

// LOAD FROM FILE
void loadFromFile() {
    FILE *fp = fopen("Student.txt", "r");
    if (fp == NULL) {
        printf("No existing file found. Starting fresh.\n");
        return;
    }

    int roll;
    char name[50];
    float marks;

    while (fscanf(fp, "%d,%[^,],%f\n", &roll, name, &marks) == 3) {
        addStudent(roll, name, marks);
    }
    fclose(fp);
    printf("Data loaded from file\n");
}

// DISPLAY - Fixed to show all students
void display() {
    if (head == NULL) {
        printf("No students in the list.\n");
        return;
    }

    Student *temp = head;
    printf("\n====================================\n");
    printf("Roll No\tName\t\tMarks\n");
    printf("====================================\n");

    while (temp != NULL) {  // Fixed: was temp->next != NULL
        printf("%d\t%s\t\t%.2f\n", temp->roll, temp->name, temp->marks);
        temp = temp->next;
    }
    printf("====================================\n");
}

// SEARCH - Fixed
void search(int roll) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    Student *temp = head;
    while (temp != NULL) {
        if (temp->roll == roll) {
            printf("\n=== Student Found ===\n");
            printf("Roll No: %d\n", temp->roll);
            printf("Name: %s\n", temp->name);
            printf("Marks: %.2f\n", temp->marks);
            return;
        }
        temp = temp->next;
    }
    printf("Student with roll number %d not found\n", roll);
}

// DELETE STUDENT - Fixed
void deleteStudent(int roll) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }

    // Delete at head
    if (head->roll == roll) {
        Student *temp = head;
        head = head->next;
        free(temp);
        printf("Student with roll number %d deleted successfully\n", roll);
        return;
    }

    // Delete at middle or end
    Student *temp = head;
    while (temp->next != NULL && temp->next->roll != roll) {
        temp = temp->next;
    }

    if (temp->next == NULL) {
        printf("Student with roll number %d not found\n", roll);
    } else {
        Student *toDel = temp->next;
        temp->next = toDel->next;
        free(toDel);
        printf("Student with roll number %d deleted successfully\n", roll);
    }
}

// MAIN FUNCTION - Fixed
int main() {
    loadFromFile();
    int choice, roll;
    float marks;
    char name[50];

    do {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Delete Student\n");
        printf("3. Search Student\n");
        printf("4. Display All Students\n");
        printf("5. Save & Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();  // Consume newline

        switch(choice) {
            case 1:
                printf("Enter Roll Number: ");
                scanf("%d", &roll);
                getchar();

                printf("Enter Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';

                printf("Enter Marks: ");
                scanf("%f", &marks);

                addStudent(roll, name, marks);
                break;

            case 2:
                printf("Enter Roll Number to delete: ");
                scanf("%d", &roll);
                deleteStudent(roll);
                break;

            case 3:
                printf("Enter Roll Number to search: ");
                scanf("%d", &roll);
                search(roll);
                break;

            case 4:
                display();
                break;

            case 5:
                saveToFile();
                printf("Exiting...\n");
                break;

            default:
                printf("Enter a valid choice!!\n");
        }
    } while(choice != 5);

    return 0;
}
