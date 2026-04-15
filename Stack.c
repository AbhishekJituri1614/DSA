#include <stdio.h>
#include <string.h>

#define MAX 100

// Stack structure
struct Stack {
    char url[MAX][100];
    int top;
};

struct Stack s;

// Initialize
void init() {
    s.top = -1;
}

// Push
void push(char url[]) {
    if (s.top == MAX - 1) {
        printf("Overflow\n");
        return;
    }
    strcpy(s.url[++s.top], url);
}

// Pop
void pop() {
    if (s.top == -1) {
        printf("No history\n");
        return;
    }
    printf("Back from: %s\n", s.url[s.top--]);
}

// Display
void display() {
    if (s.top == -1) {
        printf("Empty\n");
        return;
    }
    for (int i = s.top; i >= 0; i--) {
        printf("%s\n", s.url[i]);
    }
}

// Save to file
void save() {
    FILE *fp = fopen("history.txt", "w");
    if (!fp) return;

    fprintf(fp, "%d\n", s.top + 1);
    for (int i = 0; i <= s.top; i++) {
        fprintf(fp, "%s\n", s.url[i]);
    }
    fclose(fp);
}

// Load from file
void load() {
    FILE *fp = fopen("history.txt", "r");
    if (!fp) return;

    int n;
    fscanf(fp, "%d\n", &n);

    for (int i = 0; i < n; i++) {
        fgets(s.url[i], 100, fp);
        s.url[i][strcspn(s.url[i], "\n")] = '\0';
    }
    s.top = n - 1;

    fclose(fp);
}

// Main
int main() {
    int ch;
    char url[100];

    init();
    load();

    do {
        printf("\n1.Visit 2.Back 3.Display 4.Exit\n");
        scanf("%d", &ch);
        getchar();

        switch(ch) {
            case 1:
                printf("Enter URL: ");
                fgets(url, 100, stdin);
                url[strcspn(url, "\n")] = '\0';
                push(url);
                break;

            case 2:
                pop();
                break;

            case 3:
                display();
                break;

            case 4:
                save();
                break;
        }
    } while(ch != 4);

    return 0;
}
