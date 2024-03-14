#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    char L_name[50];
    char F_name[50];
    char group[10];
    float GPA;
};

void addnewStudent(FILE *fp) {
    struct Student student;
    char add;
    do {
        printf("Student's First Name : ");
        scanf("%s", student.F_name);
        printf("Student's Last Name : ");
        scanf("%s", student.L_name);
        printf("Student's Group : ");
        scanf("%s", student.group);
        printf("Student's GPA : ");
        scanf("%f", &student.GPA);

        fprintf(fp, "%s %s %s %.2f\n", student.L_name, student.F_name, student.group, student.GPA);
        printf("Do you want to add another student ? (y/n) : ");
        scanf(" %c", &add); 
    } while (add == 'y' || add == 'Y');
}

void afficheAllStudent(FILE *fp){
    struct Student student;
    fp = fopen("students.txt", "r");
    if (fp == NULL) {
        printf("Erreur.\n");
        return;
    }

    printf("\nList of students:\n");
    while (fscanf(fp, "%s %s %s %f", student.L_name, student.F_name, student.group, &student.GPA) != EOF) {
        printf("First Name: %s, Last name: %s, Group: %s, GPA: %.2f\n", student.F_name, student.L_name, student.group, student.GPA);
    }
    fclose(fp);
}

void searchStudent(FILE *fp, const char *searchName) {
    struct Student student;
    int found = 0;
    fp = fopen("students.txt", "r");
    if (fp == NULL) {
        printf("Erreur.\n");
        return;
    }

    while (fscanf(fp, "%s %s %s %f", student.L_name, student.F_name, student.group, &student.GPA) != EOF) {
        if (strcmp(student.F_name, searchName) == 0) {
            printf("First Name: %s, Last name: %s, Group: %s, GPA: %.2f\n", student.F_name, student.L_name, student.group, student.GPA);
            found = 1;
        }
    }

    if (found!=1) {
        printf("No students found with the first name: %s\n", searchName);
    }

    fclose(fp);
}

void sortByGPA(struct Student students[], int n) {
    int i, j;
    struct Student tmp;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (students[j].GPA > students[j + 1].GPA) {
                tmp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = tmp;
            }
        }
    }
}
int removeStudent(const char *fname, const char *lname) {
    FILE *fp, *tmp;
    struct Student student;
    int found = 0;

    fp = fopen("students.txt", "r");
    tmp = fopen("tmp.txt", "w");

    while (fscanf(fp, "%s %s %s %f", student.L_name, student.F_name, student.group, &student.GPA) != EOF) {
        if (strcmp(student.F_name, fname) == 0 && strcmp(student.L_name, lname) == 0) {
            found = 1;
            continue;
        }
        fprintf(tmp, "%s %s %s %.2f\n", student.L_name, student.F_name, student.group, student.GPA);
    }

    fclose(fp);
    fclose(tmp);

    if (found!=1) {
        return 0;
    } else {
        tmp = fopen("tmp.txt", "r");
        fp = fopen("students.txt", "w");
        if (tmp == NULL || fp == NULL) {
            printf("Error.\n");
            return 0;
        }
        
        while (fscanf(tmp, "%s %s %s %f", student.L_name, student.F_name, student.group, &student.GPA) != EOF) {
            fprintf(fp, "%s %s %s %.2f\n", student.L_name, student.F_name, student.group, student.GPA);
        }
        fclose(tmp);
        fclose(fp); 
        return found;
    }
}
int main() {
    FILE *fp;
    int choix;
    struct Student students[100];
    int n = 0;
    char first_name[50];
    char last_name[50];
    do {
        printf("\n1- Add a new student\n");
        printf("2- Show list of all students\n");
        printf("3- Search for a student by last name\n");
        printf("4- Sort students by GPA\n");
        printf("5- Remove student\n");
        printf("6- Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choix);

        switch(choix){
            case 1:
                fp = fopen("students.txt", "a");
                if (fp == NULL) {
                    printf("Erreur.\n");
                    return 1;
                }
                addnewStudent(fp);
                fclose(fp);
                break;
            case 2:
                afficheAllStudent(fp);
                break;
            case 3:
                char searchName[50];
                printf("Enter the last name you want to search: ");
                scanf("%s", searchName);
                searchStudent(fp, searchName);
                break;
            case 4:
            	fp = fopen("students.txt", "r");
                if (fp == NULL) {
                    printf("Error.\n");
                    return 1;
                }
                n = 0;
                while (fscanf(fp, "%s %s %s %f", students[n].L_name, students[n].F_name, students[n].group, &students[n].GPA) != EOF) {
                    n++;
                }
                fclose(fp);
                sortByGPA(students, n);
                printf("\nList of students sorted by GPA:\n");
                for (int i = 0; i < n; i++) {
                    printf("Name: %s %s, Group: %s, GPA: %.2f\n", students[i].F_name, students[i].L_name, students[i].group, students[i].GPA);
                }
                break;
            case 5:
            	printf("Enter the first name of the student you want to remove: ");
                scanf("%s", first_name);
                printf("Enter the last name of the student you want to remove: ");
                scanf("%s", last_name);
                if (removeStudent(first_name, last_name)) {
                    printf("Student %s %s removed successfully.\n", first_name, last_name);
                } else {
                    printf("Student %s %s not found.\n", first_name, last_name);
                }
            	break;
            case 6:
                printf("Exiting program.\n");
                break;
            	
            default:
                printf("Invalid choice.\n");
                break;
        }
    } while (choix != 6);

    return 0;
}

