#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "utils.h"

static void student_menu(const User *u)
{
    while (1) {
        int ch;
        print_header("STUDENT PORTAL");
        ch = read_int("1. View my report card\n0. Logout\nChoice: ", 0, 1);
        if (ch == 0) return;
        {
            int idx = find_student(u->roll);
            if (idx < 0)
                printf(C_RED "Your record was not found.\n" C_RESET);
            else
                print_report_card(&students[idx]);
        }
        pause_screen();
    }
}

/* Returns 1 if this role may use this menu option. */
static int allowed(int role, int choice)
{
    if (role == ROLE_ADMIN) return 1;
    /* Teacher: view, search, marks, attendance, activities, rank, report */
    switch (choice) {
    case 2: case 3: case 6: case 7: case 9: case 10: case 11: return 1;
    default: return 0;
    }
}

static void staff_menu(const User *u)
{
    while (1) {
        int ch;
        print_header(u->role == ROLE_ADMIN ? "ADMIN DASHBOARD" : "TEACHER DASHBOARD");
        printf(" 1. Add student %s\n", u->role == ROLE_ADMIN ? "" : "(admin only)");
        printf(" 2. View all students\n");
        printf(" 3. Search student\n");
        printf(" 4. Edit profile %s\n", u->role == ROLE_ADMIN ? "" : "(admin only)");
        printf(" 5. Delete student %s\n", u->role == ROLE_ADMIN ? "" : "(admin only)");
        printf(" 6. Enter marks\n");
        printf(" 7. Update attendance\n");
        printf(" 8. Fees management %s\n", u->role == ROLE_ADMIN ? "" : "(admin only)");
        printf(" 9. Extra-curricular activities\n");
        printf("10. Rank list\n");
        printf("11. 360 Report card\n");
        printf("12. Export CSV %s\n", u->role == ROLE_ADMIN ? "" : "(admin only)");
        printf(" 0. Logout\n");
        ch = read_int("Choice: ", 0, 12);
        if (ch == 0) return;
        if (!allowed(u->role, ch)) {
            printf(C_RED "Access denied: admin only.\n" C_RESET);
            pause_screen();
            continue;
        }
        switch (ch) {
        case 1:  add_student();       break;
        case 2:  view_all();          break;
        case 3:  search_student();    break;
        case 4:  edit_profile();      break;
        case 5:  delete_student();    break;
        case 6:  enter_marks();       break;
        case 7:  update_attendance(); break;
        case 8:  update_fees();       break;
        case 9:  add_activity();      break;
        case 10: show_rank_list();    break;
        case 11: report_for_roll();   break;
        case 12: export_csv();        break;
        }
        pause_screen();
    }
}

int main(void)
{
    ensure_default_users();
    if (load_students() < 0) {
        printf(C_RED "Could not load data.\n" C_RESET);
        return 1;
    }
    while (1) {
        User u;
        int ch;
        print_header("STUDENT 360 - Complete Student Management System");
        ch = read_int("1. Login\n0. Exit\nChoice: ", 0, 1);
        if (ch == 0) break;
        if (!login(&u)) {
            printf(C_RED "Too many failed attempts.\n" C_RESET);
            continue;
        }
        printf(C_GREEN "Welcome, %s!\n" C_RESET, u.username);
        if (u.role == ROLE_STUDENT)
            student_menu(&u);
        else
            staff_menu(&u);
    }
    free_students();
    printf("Goodbye!\n");
    return 0;
}
