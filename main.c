/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CREDENTIAL_FILE "credentials.txt"
#define STUDENT_FILE "students.txt"

struct student {
    int roll;
    char name[50];
    float marks;
};

char currentUser[20];
char currentRole[20];

/* FUNCTION DECLARATIONS */
void createCredentials();
int loginSystem();
void mainMenu();
void adminMenu();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

/* ---------------------------------------------------------
   MAIN
--------------------------------------------------------- */
int main() {
    createCredentials();

    if (loginSystem())
        mainMenu();
    else
        printf("\nAuthentication Failed! Exiting...\n");

    return 0;
}

/* ---------------------------------------------------------
   CREATE CREDENTIALS
--------------------------------------------------------- */
void createCredentials() {
    char choice, user[20], pass[20], role[20];

    printf("Do you want to create login credentials? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {

        printf("Enter Username: ");
        scanf("%s", user);

        printf("Enter Password: ");
        scanf("%s", pass);

        printf("Assign Role (admin): ");
        scanf("%s", role);

        FILE *fp = fopen(CREDENTIAL_FILE, "w");
        fprintf(fp, "%s %s %s\n", user, pass, role);
        fclose(fp);

        printf("\nCredentials Created Successfully!\n");
    }

    FILE *f = fopen(STUDENT_FILE, "a");
    fclose(f);
}

/* ---------------------------------------------------------
   LOGIN
--------------------------------------------------------- */
int loginSystem() {
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[20];

    printf("\n============== LOGIN PANEL ============\n");
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) return 0;

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) != EOF) {
        if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filePass) == 0) {

            strcpy(currentUser, fileUser);
            strcpy(currentRole, fileRole);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/* ---------------------------------------------------------
   MAIN MENU
--------------------------------------------------------- */
void mainMenu() {
    if (strcmp(currentRole, "admin") == 0)
        adminMenu();
}

/* ---------------------------------------------------------
   ADMIN MENU
--------------------------------------------------------- */
void adminMenu() {
    int ch;

    while (1) {
        printf("\n===== ADMIN PANEL =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    }
}

/* ---------------------------------------------------------
   ADD STUDENT (perfect)
--------------------------------------------------------- */
void addStudent() {
    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "a");

    printf("Enter Roll Number: ");
    scanf("%d", &st.roll);

    printf("Enter Name: ");
    scanf("%s", st.name);

    printf("Enter Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("Student Added!\n");
}

/* ---------------------------------------------------------
   DISPLAY STUDENTS (NEW SAFE METHOD)
--------------------------------------------------------- */
void displayStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    char line[200];

    printf("\nRoll\tName\tMarks\n");
    printf("-----------------------------\n");

    while (fgets(line, sizeof(line), fp)) {

        if (strlen(line) < 5) continue; // skip blank lines

        struct student st;
        sscanf(line, "%d %s %f", &st.roll, st.name, &st.marks);

        printf("%d\t%s\t%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
}

/* ---------------------------------------------------------
   SEARCH STUDENT
--------------------------------------------------------- */
void searchStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    char line[200];
    int roll, found = 0;

    printf("Enter Roll to Search: ");
    scanf("%d", &roll);

    while (fgets(line, sizeof(line), fp)) {

        if (strlen(line) < 5) continue;

        struct student st;
        sscanf(line, "%d %s %f", &st.roll, st.name, &st.marks);

        if (st.roll == roll) {
            printf("\nFound!\nRoll: %d\nName: %s\nMarks: %.2f\n",
                    st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found) printf("Not Found!\n");

    fclose(fp);
}

/* ---------------------------------------------------------
   UPDATE STUDENT
--------------------------------------------------------- */
void updateStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");

    char line[200];
    int roll, found = 0;

    printf("Enter Roll to Update: ");
    scanf("%d", &roll);

    while (fgets(line, sizeof(line), fp)) {

        if (strlen(line) < 5) continue;

        struct student st;
        sscanf(line, "%d %s %f", &st.roll, st.name, &st.marks);

        if (st.roll == roll) {
            found = 1;

            printf("Enter New Name: ");
            scanf("%s", st.name);
            printf("Enter New Marks: ");
            scanf("%f", &st.marks);
        }

        fprintf(tmp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(tmp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Updated Successfully!\n");
    else printf("Roll Not Found!\n");
}

/* ---------------------------------------------------------
   DELETE STUDENT
--------------------------------------------------------- */
void deleteStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");

    char line[200];
    int roll, found = 0;

    printf("Enter Roll to Delete: ");
    scanf("%d", &roll);

    while (fgets(line, sizeof(line), fp)) {

        if (strlen(line) < 5) continue;

        struct student st;
        sscanf(line, "%d %s %f", &st.roll, st.name, &st.marks);

        if (st.roll == roll) {
            found = 1;
            continue; // skip writing record
        }

        fprintf(tmp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(tmp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Record Deleted!\n");
    else printf("Roll Not Found!\n");
}