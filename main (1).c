#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define CREDENTIAL_FILE "credentials.txt"

/* -------------------------
   STUDENT STRUCTURE
-------------------------- */
struct student {
    int roll;
    char name[50];
    float marks;
};

/* STACK */
struct student stack[MAX];
int top = -1;

/* TEMP STACK */
struct student temp[MAX];
int ttop = -1;

/* LOGIN */
char currentUser[20];
char currentRole[20];

/* Function Declarations */
void createCredentials();
int loginSystem();
void mainMenu();
void adminMenu();
void userMenu();

/* Stack operations */
int isFull();
int isEmpty();
void push(struct student st);
struct student pop();
struct student peek();

/* Student operations */
void pushStudent();
void popStudent();
void peekStudent();
void displayStack();
void searchStudent();
void updateStudent();
void deleteByRoll();

/* TEMP stack ops */
void tpush(struct student st);
struct student tpop();


/* ---------------------------------------------------------
                          MAIN
--------------------------------------------------------- */
int main() {

    createCredentials();

    if (loginSystem()) {
        mainMenu();
    } else {
        printf("Login failed! Exiting...\n");
    }

    return 0;
}


/* ---------------------------------------------------------
                   CREATE CREDENTIALS
--------------------------------------------------------- */
void createCredentials() {
    char choice;
    char user[20], pass[20], role[10];

    printf("Do you want to create login credentials? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {

        printf("Enter new username: ");
        scanf("%s", user);

        printf("Enter new password: ");
        scanf("%s", pass);

        printf("Enter role (admin/user): ");
        scanf("%s", role);

        FILE *fp = fopen(CREDENTIAL_FILE, "w");
        if (!fp) {
            printf("Error creating credential file!\n");
            return;
        }

        fprintf(fp, "%s %s %s\n", user, pass, role);
        fclose(fp);

        printf("\nCredentials created successfully!\n");
    }
}


/* ---------------------------------------------------------
                          LOGIN
--------------------------------------------------------- */
int loginSystem() {
    char user[20], pass[20];
    char fuser[20], fpass[20], frole[20];

    printf("\n========== LOGIN ==========\n");
    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Credential file not found!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fuser, fpass, frole) != EOF) {
        if (strcmp(user, fuser) == 0 && strcmp(pass, fpass) == 0) {
            strcpy(currentUser, fuser);
            strcpy(currentRole, frole);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


/* ---------------------------------------------------------
                     MENUS
--------------------------------------------------------- */
void mainMenu() {
    if (strcmp(currentRole, "admin") == 0)
        adminMenu();
    else
        userMenu();
}


/* ---------------------------------------------------------
                 ADMIN MENU
--------------------------------------------------------- */
void adminMenu() {
    int ch;

    do {
        printf("\n===== ADMIN STACK MENU =====\n");
        printf("1. Push Student\n");
        printf("2. Pop Student\n");
        printf("3. Peek (Top Student)\n");
        printf("4. Display Stack\n");
        printf("5. Search Student\n");
        printf("6. Update Student\n");
        printf("7. Delete by Roll (Stack Simulation)\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: pushStudent(); break;
            case 2: popStudent(); break;
            case 3: peekStudent(); break;
            case 4: displayStack(); break;
            case 5: searchStudent(); break;
            case 6: updateStudent(); break;
            case 7: deleteByRoll(); break;
            case 8: return;
            default: printf("Invalid choice!\n");
        }

    } while (1);
}


/* ---------------------------------------------------------
                 USER MENU
--------------------------------------------------------- */
void userMenu() {
    int ch;

    do {
        printf("\n===== USER STACK MENU =====\n");
        printf("1. Display Stack\n");
        printf("2. Search Student\n");
        printf("3. Peek\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: displayStack(); break;
            case 2: searchStudent(); break;
            case 3: peekStudent(); break;
            case 4: return;
            default: printf("Invalid choice!\n");
        }

    } while (1);
}


/* ---------------------------------------------------------
                 STACK FUNCTIONS
--------------------------------------------------------- */
int isFull()  { return top == MAX - 1; }
int isEmpty() { return top == -1; }

void push(struct student st) {
    if (isFull()) {
        printf("Stack overflow! Cannot push.\n");
        return;
    }
    stack[++top] = st;
}

struct student pop() {
    struct student dummy = {-1, "", 0};
    if (isEmpty()) {
        printf("Stack underflow!\n");
        return dummy;
    }
    return stack[top--];
}

struct student peek() {
    struct student dummy = {-1, "", 0};
    if (isEmpty()) {
        printf("Stack empty!\n");
        return dummy;
    }
    return stack[top];
}

/* TEMP STACK OPS */
void tpush(struct student st) { temp[++ttop] = st; }
struct student tpop() { return temp[ttop--]; }


/* ---------------------------------------------------------
                STACK OPERATIONS (STUDENT)
--------------------------------------------------------- */

void pushStudent() {
    struct student st;

    printf("Enter roll number: ");
    scanf("%d", &st.roll);

    printf("Enter name: ");
    scanf("%s", st.name);

    printf("Enter marks: ");
    scanf("%f", &st.marks);

    push(st);
    printf("Student pushed!\n");
}

void popStudent() {
    struct student st = pop();
    if (st.roll != -1)
        printf("Popped: %d %s %.2f\n", st.roll, st.name, st.marks);
}

void peekStudent() {
    struct student st = peek();
    if (st.roll != -1)
        printf("Top Student: %d %s %.2f\n", st.roll, st.name, st.marks);
}


void displayStack() {
    if (isEmpty()) {
        printf("Stack is empty!\n");
        return;
    }

    printf("\nTOP → BOTTOM\n");
    for (int i = top; i >= 0; i--) {
        printf("%d\t%s\t%.2f\n", stack[i].roll, stack[i].name, stack[i].marks);
    }
}


void searchStudent() {
    int roll, found = 0;
    printf("Enter roll to search: ");
    scanf("%d", &roll);

    for (int i = top; i >= 0; i--) {
        if (stack[i].roll == roll) {
            printf("Found: %d %s %.2f\n", stack[i].roll, stack[i].name, stack[i].marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student not found!\n");
}


void updateStudent() {
    int roll, found = 0;
    printf("Enter roll to update: ");
    scanf("%d", &roll);

    while (!isEmpty()) {
        struct student st = pop();

        if (st.roll == roll) {
            found = 1;
            printf("New name: ");
            scanf("%s", st.name);
            printf("New marks: ");
            scanf("%f", &st.marks);
        }
        tpush(st);
    }

    while (ttop != -1)
        push(tpop());

    if (found) printf("Updated successfully!\n");
    else printf("Roll not found!\n");
}


void deleteByRoll() {
    int roll, found = 0;
    printf("Enter roll to delete: ");
    scanf("%d", &roll);

    while (!isEmpty()) {
        struct student st = pop();

        if (st.roll == roll) {
            found = 1;
            continue;
        }
        tpush(st);
    }

    while (ttop != -1)
        push(tpop());

    if (found) printf("Deleted successfully!\n");
    else printf("Roll not found!\n");
}
