#ifndef STUDENT_H
#define STUDENT_H

#define NAME_LEN  50
#define COURSE_NAME 30
#define PHONE_LEN 15
#define ADDR_LEN 90
#define ACT_LEN 120
#define NUM_SUBJECTS 10

#define DATA_FILE "data/students.dat"
#define USER_FILE "data/users.dat"
#define CSV_FILE "data/report.csv"

#define ROLE_ADMIN 1
#define ROLE_TEACHER 2
#define ROLE_STUDENT 3

/*-----------Nested structures : one student = one 360-degree record-----------*/
typedef struct {
    int roll;
    char name[NAME_LEN];
    char course[COURSE_NAME];
    char phone[PHONE_LEN];
    char address[ADDR_LEN];
}Profile;

typedef struct{
    float marks[NUM_SUBJECTS];
    int entered;
}Academics;

typedef struct{
    int total_days;
    int present_days;
}Attendence;

typedef struct{
    float total;
    float paid;
}Fees;

typedef struct {
    Profile  profile;
    Academics acad;
    Attendence att;
    Fees       fees;
    char       activities[ACT_LEN];
}Student;

typedef struct {
    char    username[20];
    unsigned long   pass_hash;
    int        role;
    int        roll;   /* only used for ROLE_STUDENT*/
} User;

extern const char *SUBJECTS[NUM_SUBJECTS];
extern Student *students;
extern int student_count;

/* auth.c*/
unsigned long hash_password(const char *s);
void ensure_default_users(void);
int login(User *out);
int add_user(const char *username, const char *password,int role,int roll);
void remove_user_by_roll(int roll);

/* student.c*/
int load_students(void);
int save_students(void);
void free_students(void);
int find_students(int roll);
void add_studnts(void);
void view_all(void);
void search_students(void);
void edit_students(void);
void delete_students(void);
void enter_marks(void);
void update_attendence(void);
void update_fees(void);
void add_activity(void);

/* report.c*/
float       total_marks(const Student *s);
float       percentage(const Student *s);
float       cgpa_of(const Student *s);
float       attendence_pct(const Student *s);
const char *graph_of(float pct);
const char *remark_of(const Student *s);
void print_report_card(const Student *s);
void report_for_roll(void);
void show_rank_list(void);
void export_csv(void);

#endif



