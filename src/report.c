#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "utils.h"

const char *SUBJECTS[NUM_SUBJECTS] = {
    "Mathematics", "Physics", "Chemistry", "English", "Programming in C"
};

float total_marks(const Student *s)
{
    float t = 0;
    int i;
    for (i = 0; i < NUM_SUBJECTS; i++)
        t += s->acad.marks[i];
    return t;
}

float percentage(const Student *s)
{
    return total_marks(s) / (NUM_SUBJECTS * 100.0f) * 100.0f;
}

float cgpa_of(const Student *s)
{
    return percentage(s) / 9.5f;        /* common university conversion */
}

float attendance_pct(const Student *s)
{
    if (s->att.total_days == 0) return 0.0f;
    return s->att.present_days * 100.0f / s->att.total_days;
}

const char *grade_of(float p)
{
    if (p >= 90) return "O";
    if (p >= 80) return "A+";
    if (p >= 70) return "A";
    if (p >= 60) return "B+";
    if (p >= 50) return "B";
    if (p >= 40) return "C";
    return "F";
}

const char *remark_of(const Student *s)
{
    float p = percentage(s);
    if (!s->acad.entered) return "Marks pending";
    if (p < 40 || (s->att.total_days > 0 && attendance_pct(s) < 75))
        return "At Risk";
    if (p >= 75) return "Excellent";
    if (p >= 60) return "Good";
    return "Needs Improvement";
}

void print_report_card(const Student *s)
{
    int i;
    float p = percentage(s);
    print_header("STUDENT 360 - REPORT CARD");
    printf(" Roll No : %d\n Name    : %s\n Course  : %s\n Phone   : %s\n Address : %s\n",
           s->profile.roll, s->profile.name, s->profile.course,
           s->profile.phone, s->profile.address);

    printf("\n" C_YELLOW " ACADEMICS" C_RESET "\n");
    if (!s->acad.entered) {
        printf(" Marks not entered yet.\n");
    } else {
        for (i = 0; i < NUM_SUBJECTS; i++)
            printf(" %-20s %6.1f / 100\n", SUBJECTS[i], s->acad.marks[i]);
        printf(" ----------------------------------\n");
        printf(" Total: %.1f / %d   Percentage: %.2f%%\n",
               total_marks(s), NUM_SUBJECTS * 100, p);
        printf(" Grade: %s   CGPA: %.2f\n", grade_of(p), cgpa_of(s));
    }

    printf("\n" C_YELLOW " ATTENDANCE" C_RESET "\n");
    if (s->att.total_days == 0) {
        printf(" No attendance recorded yet.\n");
    } else {
        printf(" %d / %d classes (%.1f%%)\n",
               s->att.present_days, s->att.total_days, attendance_pct(s));
        if (attendance_pct(s) < 75)
            printf(C_RED " WARNING: below 75%% - risk of detention!\n" C_RESET);
    }

    printf("\n" C_YELLOW " FEES" C_RESET "\n");
    printf(" Total: %.2f   Paid: %.2f   Due: %.2f\n",
           s->fees.total, s->fees.paid, s->fees.total - s->fees.paid);

    printf("\n" C_YELLOW " EXTRA-CURRICULAR" C_RESET "\n %s\n", s->activities);

    printf("\n" C_YELLOW " OVERALL REMARK: " C_RESET "%s\n", remark_of(s));
}

void report_for_roll(void)
{
    int roll = read_int("Enter roll number: ", 1, 999999);
    int idx = find_student(roll);
    if (idx < 0)
        printf(C_RED "No student found with roll number %d.\n" C_RESET, roll);
    else
        print_report_card(&students[idx]);
}

static int cmp_total_desc(const void *a, const void *b)
{
    float ta = total_marks(*(const Student *const *)a);
    float tb = total_marks(*(const Student *const *)b);
    return (ta < tb) - (ta > tb);
}

void show_rank_list(void)
{
    const Student **list;
    int i, n = 0;
    print_header("RANK LIST");
    if (student_count == 0) {
        printf("No records yet.\n");
        return;
    }
    list = malloc((size_t)student_count * sizeof(*list));
    if (!list) return;
    for (i = 0; i < student_count; i++)
        if (students[i].acad.entered)
            list[n++] = &students[i];
    if (n == 0) {
        printf("No marks entered yet.\n");
        free(list);
        return;
    }
    qsort(list, (size_t)n, sizeof(*list), cmp_total_desc);
    printf(C_YELLOW "%-5s %-8s %-22s %-8s %-6s\n" C_RESET, "Rank", "Roll", "Name", "Percent", "Grade");
    printf("-----------------------------------------------------\n");
    for (i = 0; i < n; i++)
        printf("%-5d %-8d %-22s %-8.2f %-6s\n", i + 1, list[i]->profile.roll,
               list[i]->profile.name, percentage(list[i]), grade_of(percentage(list[i])));
    printf("\nClass topper: %s\n", list[0]->profile.name);
    free(list);
}

void export_csv(void)
{
    FILE *fp;
    int i;
    print_header("EXPORT TO CSV");
    fp = fopen(CSV_FILE, "w");
    if (!fp) {
        printf(C_RED "Cannot create %s\n" C_RESET, CSV_FILE);
        return;
    }
    fprintf(fp, "Roll,Name,Course,Total,Percentage,Grade,CGPA,Attendance%%,FeeDue,Remark\n");
    for (i = 0; i < student_count; i++) {
        const Student *s = &students[i];
        fprintf(fp, "%d,%s,%s,%.1f,%.2f,%s,%.2f,%.1f,%.2f,%s\n",
                s->profile.roll, s->profile.name, s->profile.course,
                total_marks(s), percentage(s), grade_of(percentage(s)),
                cgpa_of(s), attendance_pct(s), s->fees.total - s->fees.paid,
                remark_of(s));
    }
    fclose(fp);
    printf(C_GREEN "Exported %d records to %s\n" C_RESET, student_count, CSV_FILE);
}
