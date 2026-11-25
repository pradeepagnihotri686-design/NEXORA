
# Student 360 - Complete Student Management System (C)

A console-based application written in C that gives a **360-degree view of every student**: profile, academics, attendance, fees and extra-curricular activities, all in one report card.

## Features

- **Role-based login**: Admin, Teacher and Student (passwords stored as hashes, 3 login attempts)
- **Student profile**: add, view, search (by roll number or partial name), edit, delete
- **Academics**: marks for 5 subjects, total, percentage, grade and CGPA
- **Attendance**: running record with an automatic warning below 75%
- **Fees**: set total fee, record payments, see the due amount
- **Extra-curricular**: sports, clubs and achievements
- **Rank list**: sorted with `qsort`, shows the class topper
- **360 Report card**: everything about a student on one screen, with an overall remark (Excellent / Good / Needs Improvement / At Risk)
- **CSV export** of all students for Excel
- **Persistent storage** in binary files (`fread` / `fwrite`)
- **Input validation**: wrong marks, wrong roll numbers, empty or too-long input are rejected

## C concepts used

| Concept | Where |
|---|---|
| Nested structs | `Student` contains `Profile`, `Academics`, `Attendance`, `Fees` |
| Dynamic memory | `malloc` / `realloc` / `free` for the student array |
| File handling | Binary files for students and users, text file for CSV |
| Pointers and `const` | Passing records to functions without copying |
| Sorting | `qsort` with a comparator for the rank list |
| Hashing | djb2 password hash |
| Modular code | Separate `.c` and `.h` files, `Makefile` |

## Project structure

```
student360/
├── src/        main.c, auth.c, student.c, report.c, utils.c
├── include/    student.h, utils.h
├── data/       created data files (.dat, .csv)
├── docs/       report, flowchart, screenshots
├── Makefile
└── README.md
```

## Build and run

```
make
./student360          # on Windows: student360.exe
```

Without `make`:

```
gcc -Wall -Wextra -std=c99 -Iinclude src/*.c -o student360
```

Run the program from the project root so the `data/` folder is found.
If colours look broken in your terminal, set `USE_COLOR` to `0` in `include/utils.h`.

## Default logins

| Role | Username | Password |
|---|---|---|
| Admin | `admin` | `admin123` |
| Teacher | `teacher` | `teach123` |
| Student | roll number (e.g. `101`) | `pass123` (created automatically when admin adds the student) |

## Permissions

| Action | Admin | Teacher | Student |
|---|:-:|:-:|:-:|
| Add / edit / delete student | Yes | No | No |
| Fees, CSV export | Yes | No | No |
| Marks, attendance, activities | Yes | Yes | No |
| View all, search, rank list, report card | Yes | Yes | Own report only |

## Future scope

- Password change option and stronger hashing (SHA-256)
- Editable subject list per course
- Semester-wise records
- GUI or web front end

## 👥 Project Team (Collaborators)

| S.No. | Name | Roll Number | Role |
|---|---|---|---|
| 1 | Pradeep Agnihotri | B2655R101A1123 | Developer |
| 2 | Hardik Sewanee| B2655R101A1107 | Developer |

---

## 🤝 Team Project

Student360 was developed collaboratively as a group project.
Each team member contributed to the development, implementation,
testing, or documentation of the project.


## 🤖 AI Assistance & Acknowledgement

During the development of Student360, we used AI tools such as
ChatGPT (OpenAI) and Claude (Anthropic) for assistance with:

- Understanding C programming concepts
- Debugging and resolving errors
- Improving code structure and modularity
- Generating ideas and documentation
- Learning and refining implementation approaches

The project was developed, reviewed, tested, and implemented by the team,
with AI tools used as supportive learning and development assistants.

