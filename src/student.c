#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "student.h"
#include "utils.h"

Student *students = NULL;
int student_count = 0;

/* ---------------- File handling ---------------- */

int load_students(void)
{
    FILE *fp;
    long size;
    int n;
    free(students);
    students = NULL;
    student_count = 0;
    fp = fopen(DATA_FILE, "rb");
    if (!fp) return 0;                         /* first run: no data yet */
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    n = (int)(size / (long)sizeof(Student));
    if (n > 0) {
        students = malloc((size_t)n * sizeof(Student));
        if (!students) { fclose(fp); return -1; }
        student_count = (int)fread(students, sizeof(Student), (size_t)n, fp);
    }
    fclose(fp);
    return student_count;
}

int save_students(void)
{
    FILE *fp = fopen(DATA_FILE, "wb");
    if (!fp) {
        printf(C_RED "Error: cannot write %s (does the data folder exist?)\n" C_RESET, DATA_FILE);
        return 0;
    }
    if (student_count > 0)
        fwrite(students, sizeof(Student), (size_t)student_count, fp);
    fclose(fp);
    return 1;
}

void free_students(void)
{
    free(students);
    students = NULL;
    student_count = 0;
}

/* ---------------- Helpers ---------------- */

int find_student(int roll)
{
    int i;
    for (i = 0; i < student_count; i++)
        if (students[i].profile.roll == roll)
            return i;
    return -1;
}

static int contains_ci(const char *hay, const char *needle)
{
    size_t i, j, hl = strlen(hay), nl = strlen(needle);
    if (nl == 0 || nl > hl) return nl == 0;
    for (i = 0; i + nl <= hl; i++) {
        for (j = 0; j < nl; j++)
            if (tolower((unsigned char)hay[i + j]) != tolower((unsigned char)needle[j]))
                break;
        if (j == nl) return 1;
    }
    return 0;
}

static void print_row(const Student *s)
{
    printf("%-8d %-22s %-14s %-12s\n",
           s->profile.roll, s->profile.name, s->profile.course, s->profile.phone);
}

static void print_table_head(void)
{
    printf(C_YELLOW "%-8s %-22s %-14s %-12s\n" C_RESET, "Roll", "Name", "Course", "Phone");
    printf("--------------------------------------------------------\n");
}

/* Asks for a roll number and returns the index, or -1 (with a message). */
static int ask_student(void)
{
    int roll = read_int("Enter roll number: ", 1, 999999);
    int idx = find_student(roll);
    if (idx < 0)
        printf(C_RED "No student found with roll number %d.\n" C_RESET, roll);
    return idx;
}

/* ---------------- Features ---------------- */

void add_student(void)
{
    Student s, *tmp;
    int roll;
    print_header("ADD NEW STUDENT");
    roll = read_int("Roll number: ", 1, 999999);
    if (find_student(roll) >= 0) {
        printf(C_RED "Roll number %d already exists.\n" C_RESET, roll);
        return;
    }
    memset(&s, 0, sizeof s);
    s.profile.roll = roll;
    read_string("Name: ", s.profile.name, NAME_LEN);
    read_string("Course (e.g. B.Tech CSE): ", s.profile.course, COURSE_LEN);
    read_string("Phone: ", s.profile.phone, PHONE_LEN);
    read_string("Address: ", s.profile.address, ADDR_LEN);
    strcpy(s.activities, "None");

    tmp = realloc(students, (size_t)(student_count + 1) * sizeof(Student));
    if (!tmp) {
        printf(C_RED "Out of memory!\n" C_RESET);
        return;
    }
    students = tmp;
    students[student_count++] = s;
    if (save_students()) {
        char uname[20];
        snprintf(uname, sizeof uname, "%d", roll);
        add_user(uname, "pass123", ROLE_STUDENT, roll);
        printf(C_GREEN "Student added. Login -> username: %d, password: pass123\n" C_RESET, roll);
    }
}

void view_all(void)
{
    int i;
    print_header("ALL STUDENTS");
    if (student_count == 0) {
        printf("No records yet.\n");
        return;
    }
    print_table_head();
    for (i = 0; i < student_count; i++)
        print_row(&students[i]);
    printf("\nTotal students: %d\n", student_count);
}

void search_student(void)
{
    int choice, i, found = 0;
    print_header("SEARCH STUDENT");
    choice = read_int("1. By roll number\n2. By name\nChoice: ", 1, 2);
    if (choice == 1) {
        int roll = read_int("Roll number: ", 1, 999999);
        int idx = find_student(roll);
        if (idx < 0) {
            printf(C_RED "Not found.\n" C_RESET);
            return;
        }
        print_table_head();
        print_row(&students[idx]);
    } else {
        char key[NAME_LEN];
        read_string("Name (or part of it): ", key, sizeof key);
        print_table_head();
        for (i = 0; i < student_count; i++)
            if (contains_ci(students[i].profile.name, key)) {
                print_row(&students[i]);
                found++;
            }
        if (!found)
            printf(C_RED "No matching students.\n" C_RESET);
    }
}

void edit_profile(void)
{
    int idx, ch;
    Profile *p;
    print_header("EDIT PROFILE");
    idx = ask_student();
    if (idx < 0) return;
    p = &students[idx].profile;
    ch = read_int("1. Name\n2. Course\n3. Phone\n4. Address\nField to edit: ", 1, 4);
    switch (ch) {
    case 1: read_string("New name: ", p->name, NAME_LEN); break;
    case 2: read_string("New course: ", p->course, COURSE_LEN); break;
    case 3: read_string("New phone: ", p->phone, PHONE_LEN); break;
    case 4: read_string("New address: ", p->address, ADDR_LEN); break;
    }
    if (save_students())
        printf(C_GREEN "Profile updated.\n" C_RESET);
}

void delete_student(void)
{
    int idx, i, roll;
    char ans[8];
    print_header("DELETE STUDENT");
    idx = ask_student();
    if (idx < 0) return;
    printf("Delete %s? (y/n): ", students[idx].profile.name);
    if (!fgets(ans, sizeof ans, stdin) || (ans[0] != 'y' && ans[0] != 'Y')) {
        printf("Cancelled.\n");
        return;
    }
    roll = students[idx].profile.roll;
    for (i = idx; i < student_count - 1; i++)
        students[i] = students[i + 1];
    student_count--;
    if (save_students()) {
        remove_user_by_roll(roll);
        printf(C_GREEN "Student deleted.\n" C_RESET);
    }
}

void enter_marks(void)
{
    int idx, i;
    char prompt[64];
    print_header("ENTER MARKS (out of 100)");
    idx = ask_student();
    if (idx < 0) return;
    printf("Student: %s\n", students[idx].profile.name);
    for (i = 0; i < NUM_SUBJECTS; i++) {
        snprintf(prompt, sizeof prompt, "%s: ", SUBJECTS[i]);
        students[idx].acad.marks[i] = read_float(prompt, 0, 100);
    }
    students[idx].acad.entered = 1;
    if (save_students())
        printf(C_GREEN "Marks saved. Percentage: %.2f%%  Grade: %s\n" C_RESET,
               percentage(&students[idx]), grade_of(percentage(&students[idx])));
}

void update_attendance(void)
{
    int idx, held, present;
    Attendance *a;
    print_header("UPDATE ATTENDANCE");
    idx = ask_student();
    if (idx < 0) return;
    a = &students[idx].att;
    printf("Current: %d present out of %d days\n", a->present_days, a->total_days);
    held = read_int("Classes held in this period: ", 1, 400);
    present = read_int("Classes attended: ", 0, held);
    a->total_days += held;
    a->present_days += present;
    if (save_students()) {
        printf(C_GREEN "Attendance updated: %.1f%%\n" C_RESET, attendance_pct(&students[idx]));
        if (attendance_pct(&students[idx]) < 75.0f)
            printf(C_RED "WARNING: attendance is below 75%%!\n" C_RESET);
    }
}

void update_fees(void)
{
    int idx, ch;
    Fees *f;
    print_header("FEES MANAGEMENT");
    idx = ask_student();
    if (idx < 0) return;
    f = &students[idx].fees;
    printf("Total: %.2f  Paid: %.2f  Due: %.2f\n", f->total, f->paid, f->total - f->paid);
    ch = read_int("1. Set total fee\n2. Record a payment\nChoice: ", 1, 2);
    if (ch == 1) {
        f->total = read_float("Total fee: ", 0, 10000000);
    } else {
        float due = f->total - f->paid;
        float pay = read_float("Amount paid now: ", 0, 10000000);
        if (pay > due) {
            printf(C_RED "Payment exceeds the due amount (%.2f).\n" C_RESET, due);
            return;
        }
        f->paid += pay;
    }
    if (save_students())
        printf(C_GREEN "Fees updated. Due now: %.2f\n" C_RESET, f->total - f->paid);
}

void add_activity(void)
{
    int idx;
    print_header("EXTRA-CURRICULAR ACTIVITIES");
    idx = ask_student();
    if (idx < 0) return;
    printf("Current: %s\n", students[idx].activities);
    read_string("New activities / achievements: ", students[idx].activities, ACT_LEN);
    if (save_students())
        printf(C_GREEN "Saved.\n" C_RESET);
}
