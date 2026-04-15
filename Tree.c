#include <stdio.h>
#include <stdlib.h>

struct Node {
    int bookID;
    struct Node *left;
    struct Node *right;
};

struct Node* createNode(int id) {
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->bookID = id;
    newNode->left   = NULL;
    newNode->right  = NULL;
    return newNode;
}

struct Node* insert(struct Node *root, int id) {
    if (root == NULL) return createNode(id);
    if (id < root->bookID)
        root->left = insert(root->left, id);
    else if (id > root->bookID)
        root->right = insert(root->right, id);
    else
        printf("Book ID %d already exists.\n", id);
    return root;
}

void search(struct Node *root, int id) {
    if (root == NULL) { printf("Book ID %d NOT FOUND.\n", id); return; }
    if (id == root->bookID)      printf("Book ID %d FOUND.\n", id);
    else if (id < root->bookID)  search(root->left, id);
    else                         search(root->right, id);
}

struct Node* findMin(struct Node *root) {
    while (root->left != NULL) root = root->left;
    return root;
}

struct Node* deleteNode(struct Node *root, int id) {
    if (root == NULL) { printf("Book ID %d NOT FOUND.\n", id); return NULL; }

    if (id < root->bookID)
        root->left = deleteNode(root->left, id);
    else if (id > root->bookID)
        root->right = deleteNode(root->right, id);
    else {
        if (root->left == NULL && root->right == NULL) {
            free(root); return NULL;
        } else if (root->left == NULL) {
            struct Node *temp = root->right; free(root); return temp;
        } else if (root->right == NULL) {
            struct Node *temp = root->left; free(root); return temp;
        } else {
            struct Node *successor = findMin(root->right);
            root->bookID = successor->bookID;
            root->right = deleteNode(root->right, successor->bookID);
        }
    }
    return root;
}

void inorder(struct Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d  ", root->bookID);
    inorder(root->right);
}

void freeTree(struct Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    struct Node *root = NULL;
    int choice, id;

    do {
        printf("\n------- MENU -------\n");
        printf("1. Insert Book ID\n");
        printf("2. Search Book ID\n");
        printf("3. Delete Book ID\n");
        printf("4. Display All (Sorted)\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Book ID to insert: ");
                scanf("%d", &id);
                root = insert(root, id);
                printf("Inserted %d successfully.\n", id);
                break;
            case 2:
                printf("Enter Book ID to search: ");
                scanf("%d", &id);
                search(root, id);
                break;
            case 3:
                printf("Enter Book ID to delete: ");
                scanf("%d", &id);
                root = deleteNode(root, id);
                printf("Deleted %d successfully.\n", id);
                break;
            case 4:
                if (root == NULL) printf("No records found.\n");
                else { printf("Book IDs (sorted): "); inorder(root); printf("\n"); }
                break;
            case 5:
                freeTree(root);
                printf("Exiting. Goodbye!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 5);

    return 0;
}
