#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Employee {
    int emp_id;
    char name[50];
    char department[50];
    float salary;
    struct Employee *prev;
    struct Employee *next;
} Employee;

Employee *head = NULL;
Employee *tail = NULL;

// Function prototypes
void addEmployee(int id, char *name, char *department, float salary);
void removeEmployee(int id);
void searchEmployee(int id);
void displayForward();
void displayBackward();
void saveToFile();
void loadFromFile();

// ADD EMPLOYEE (at the end)
void addEmployee(int id, char *name, char *department, float salary) {
    // Check if employee with same ID already exists
    Employee *temp = head;
    while (temp != NULL) {
        if (temp->emp_id == id) {
            printf("Error: Employee with ID %d already exists!\n", id);
            return;
        }
        temp = temp->next;
    }

    Employee *newEmp = (Employee*)malloc(sizeof(Employee));
    if (newEmp == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    newEmp->emp_id = id;
    strcpy(newEmp->name, name);
    strcpy(newEmp->department, department);
    newEmp->salary = salary;
    newEmp->prev = NULL;
    newEmp->next = NULL;

    // Case 1: Empty list
    if (head == NULL) {
        head = tail = newEmp;
        printf("Employee added successfully! (First employee)\n");
        return;
    }

    // Case 2: Add at the end
    tail->next = newEmp;
    newEmp->prev = tail;
    tail = newEmp;

    printf("Employee added successfully!\n");
    printf("ID: %d, Name: %s, Dept: %s, Salary: ₹%.2f\n",
           id, name, department, salary);
}

// REMOVE EMPLOYEE
void removeEmployee(int id) {
    if (head == NULL) {
        printf("List is empty! No employees to delete.\n");
        return;
    }

    Employee *temp = head;

    // Search for employee with given ID
    while (temp != NULL && temp->emp_id != id) {
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Employee with ID %d not found.\n", id);
        return;
    }

    // Case 1: Only one node
    if (head == tail && head == temp) {
        head = tail = NULL;
        free(temp);
        printf("Employee with ID %d deleted successfully.\n", id);
        return;
    }

    // Case 2: Delete head node
    if (temp == head) {
        head = head->next;
        head->prev = NULL;
        free(temp);
        printf("Employee with ID %d deleted successfully.\n", id);
        return;
    }

    // Case 3: Delete tail node
    if (temp == tail) {
        tail = tail->prev;
        tail->next = NULL;
        free(temp);
        printf("Employee with ID %d deleted successfully.\n", id);
        return;
    }

    // Case 4: Delete middle node
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    free(temp);
    printf("Employee with ID %d deleted successfully.\n", id);
}

// SEARCH EMPLOYEE
void searchEmployee(int id) {
    if (head == NULL) {
        printf("No employees in the system.\n");
        return;
    }

    Employee *temp = head;
    int position = 1;

    while (temp != NULL) {
        if (temp->emp_id == id) {
            printf("\n========== EMPLOYEE FOUND ==========\n");
            printf("Employee ID     : %d\n", temp->emp_id);
            printf("Name            : %s\n", temp->name);
            printf("Department      : %s\n", temp->department);
            printf("Salary          : ₹%.2f\n", temp->salary);
            printf("Position in list: %d (from front)\n", position);

            // Count from back
            Employee *back = tail;
            int backPos = 1;
            while (back != NULL && back->emp_id != id) {
                backPos++;
                back = back->prev;
            }
            printf("Position from end: %d\n", backPos);
            printf("====================================\n");
            return;
        }
        temp = temp->next;
        position++;
    }

    printf("Employee with ID %d not found.\n", id);
}

// DISPLAY FORWARD (Head to Tail)
void displayForward() {
    if (head == NULL) {
        printf("No employees in the system.\n");
        return;
    }

    Employee *temp = head;
    int count = 1;

    printf("\n===============================================================\n");
    printf("              EMPLOYEE MANAGEMENT SYSTEM                       \n");
    printf("===============================================================\n");
    printf("S.No\tEmp ID\tName\t\tDepartment\tSalary (₹)\n");
    printf("===============================================================\n");

    while (temp != NULL) {
        printf("%d\t%d\t%-15s\t%-12s\t%.2f\n",
               count++,
               temp->emp_id,
               temp->name,
               temp->department,
               temp->salary);
        temp = temp->next;
    }

    printf("===============================================================\n");
    printf("Total Employees: %d\n", count - 1);
    printf("===============================================================\n");
}

// DISPLAY BACKWARD (Tail to Head)
void displayBackward() {
    if (tail == NULL) {
        printf("No employees in the system.\n");
        return;
    }

    Employee *temp = tail;
    int count = 1;

    printf("\n===============================================================\n");
    printf("         EMPLOYEE MANAGEMENT SYSTEM (Reverse Order)            \n");
    printf("===============================================================\n");
    printf("S.No\tEmp ID\tName\t\tDepartment\tSalary (₹)\n");
    printf("===============================================================\n");

    while (temp != NULL) {
        printf("%d\t%d\t%-15s\t%-12s\t%.2f\n",
               count++,
               temp->emp_id,
               temp->name,
               temp->department,
               temp->salary);
        temp = temp->prev;
    }

    printf("===============================================================\n");
    printf("Total Employees: %d\n", count - 1);
    printf("===============================================================\n");
}

// SAVE TO FILE
void saveToFile() {
    FILE *fp = fopen("employees.txt", "w");

    if (fp == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    if (head == NULL) {
        fprintf(fp, "0\n");
        fclose(fp);
        printf("No employees to save. File saved as empty.\n");
        return;
    }

    Employee *temp = head;
    int count = 0;

    // Count total employees
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    // Save count
    fprintf(fp, "%d\n", count);

    // Save all employees
    temp = head;
    while (temp != NULL) {
        fprintf(fp, "%d|%s|%s|%.2f\n",
                temp->emp_id,
                temp->name,
                temp->department,
                temp->salary);
        temp = temp->next;
    }

    fclose(fp);
    printf("Employee data saved successfully! (%d employees saved)\n", count);
}

// LOAD FROM FILE
void loadFromFile() {
    FILE *fp = fopen("employees.txt", "r");

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
    char name[50];
    char department[50];
    float salary;

    for (int i = 0; i < count; i++) {
        if (fscanf(fp, "%d|%49[^|]|%49[^|]|%f\n", &id, name, department, &salary) == 4) {
            // Directly add without duplicate check during load
            Employee *newEmp = (Employee*)malloc(sizeof(Employee));
            if (newEmp == NULL) {
                printf("Memory allocation failed during load!\n");
                continue;
            }

            newEmp->emp_id = id;
            strcpy(newEmp->name, name);
            strcpy(newEmp->department, department);
            newEmp->salary = salary;
            newEmp->prev = NULL;
            newEmp->next = NULL;

            if (head == NULL) {
                head = tail = newEmp;
            } else {
                tail->next = newEmp;
                newEmp->prev = tail;
                tail = newEmp;
            }
        }
    }

    fclose(fp);
    printf("Employee data loaded successfully! (%d employees loaded)\n", count);
}

// MAIN FUNCTION
int main() {
    loadFromFile();

    int choice, id;
    char name[50];
    char department[50];
    float salary;

    do {
        printf("\n===============================================================\n");
        printf("         EMPLOYEE MANAGEMENT SYSTEM (Doubly Linked List)       \n");
        printf("===============================================================\n");
        printf("1. Add Employee\n");
        printf("2. Remove Employee\n");
        printf("3. Search Employee\n");
        printf("4. Display All Employees (Forward)\n");
        printf("5. Display All Employees (Backward)\n");
        printf("6. Save & Exit\n");
        printf("===============================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Consume newline

        switch(choice) {
            case 1:
                printf("\n--- Add New Employee ---\n");
                printf("Enter Employee ID: ");
                scanf("%d", &id);
                getchar();

                printf("Enter Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';

                printf("Enter Department: ");
                fgets(department, sizeof(department), stdin);
                department[strcspn(department, "\n")] = '\0';

                printf("Enter Salary (₹): ");
                scanf("%f", &salary);

                addEmployee(id, name, department, salary);
                break;

            case 2:
                printf("\n--- Remove Employee ---\n");
                printf("Enter Employee ID to remove: ");
                scanf("%d", &id);
                removeEmployee(id);
                break;

            case 3:
                printf("\n--- Search Employee ---\n");
                printf("Enter Employee ID to search: ");
                scanf("%d", &id);
                searchEmployee(id);
                break;

            case 4:
                displayForward();
                break;

            case 5:
                displayBackward();
                break;

            case 6:
                saveToFile();
                printf("Exiting Employee Management System...\n");
                break;

            default:
                printf("Invalid choice! Please enter a number between 1-6.\n");
        }
    } while(choice != 6);

    return 0;
}
