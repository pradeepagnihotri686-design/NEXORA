#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "utils.h"

/* djb2 hash: passwords are never stored as plain text. */
unsigned long hash_password(const char *s)
{
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*s++))
        h = ((h << 5) + h) + c;
    return h;
}

static int user_exists(const char *username)
{
    FILE *fp = fopen(USER_FILE, "rb");
    User u;
    if (!fp) return 0;
    while (fread(&u, sizeof u, 1, fp) == 1) {
        if (strcmp(u.username, username) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int add_user(const char *username, const char *password, int role, int roll)
{
    User u;
    FILE *fp;
    if (user_exists(username)) return 0;
    fp = fopen(USER_FILE, "ab");
    if (!fp) return 0;
    memset(&u, 0, sizeof u);
    strncpy(u.username, username, sizeof u.username - 1);
    u.pass_hash = hash_password(password);
    u.role = role;
    u.roll = roll;
    fwrite(&u, sizeof u, 1, fp);
    fclose(fp);
    return 1;
}

/* Creates default admin and teacher accounts on first run. */
void ensure_default_users(void)
{
    FILE *fp = fopen(USER_FILE, "rb");
    if (fp) {
        fclose(fp);
        return;
    }
    add_user("admin",   "admin123", ROLE_ADMIN,   0);
    add_user("teacher", "teach123", ROLE_TEACHER, 0);
}

void remove_user_by_roll(int roll)
{
    FILE *fp = fopen(USER_FILE, "rb");
    User *all = NULL, u;
    int n = 0, i;
    if (!fp) return;
    while (fread(&u, sizeof u, 1, fp) == 1) {
        User *tmp = realloc(all, (n + 1) * sizeof(User));
        if (!tmp) { free(all); fclose(fp); return; }
        all = tmp;
        all[n++] = u;
    }
    fclose(fp);
    fp = fopen(USER_FILE, "wb");
    if (!fp) { free(all); return; }
    for (i = 0; i < n; i++)
        if (!(all[i].role == ROLE_STUDENT && all[i].roll == roll))
            fwrite(&all[i], sizeof(User), 1, fp);
    fclose(fp);
    free(all);
}

/* Up to 3 attempts. Returns 1 on success and fills *out. */
int login(User *out)
{
    char username[20], password[40];
    int attempt;
    for (attempt = 1; attempt <= 3; attempt++) {
        FILE *fp;
        User u;
        int ok = 0;
        read_string("Username (student: roll number): ", username, sizeof username);
        read_string("Password: ", password, sizeof password);
        fp = fopen(USER_FILE, "rb");
        if (fp) {
            while (fread(&u, sizeof u, 1, fp) == 1) {
                if (strcmp(u.username, username) == 0 &&
                    u.pass_hash == hash_password(password)) {
                    *out = u;
                    ok = 1;
                    break;
                }
            }
            fclose(fp);
        }
        if (ok) return 1;
        printf(C_RED "Wrong username or password (%d/3).\n" C_RESET, attempt);
    }
    return 0;
}
