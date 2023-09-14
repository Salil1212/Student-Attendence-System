// This is student attendence system using sqlite3 to fetch details from database 
#include <iostream>
#include <sqlite3.h>
#include <string>
using namespace std;

// Function to add a new student record to the database
void addStudent(sqlite3* db, const string& name, int roll, int attendance) {
    string sql = "INSERT INTO students (name, rollNumber, attendance) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, roll);
    sqlite3_bind_int(stmt, 3, attendance); // Change marks to attendance

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    cout << "Student record added successfully!" << endl;
    sqlite3_finalize(stmt);
}

// Function to display all student records from the database
void displayAllStudents(sqlite3* db) {
    string sql = "SELECT * FROM students;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        int roll = sqlite3_column_int(stmt, 2);
        int attendance = sqlite3_column_int(stmt, 3); // Change marks to attendance

        cout << "ID: " << id << ", Name: " << name << ", Roll Number: " << roll << ", Attendance: " << attendance << endl;
    }

    sqlite3_finalize(stmt);
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("students.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return 0;
    }

    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS students ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "name TEXT NOT NULL,"
                                 "rollNumber INT NOT NULL,"
                                 "attendance INT NOT NULL);"; // Change marks to attendance

    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);

    if (rc) {
        cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 0;
    }

    // Interact with the database
    addStudent(db, "John Doe", 101, 90);     // Add attendance values
    addStudent(db, "Alice Smith", 102, 95);  // Add attendance values
    addStudent(db, "Bob Johnson", 103, 88);  // Add attendance values

    cout << "Student Records:" << endl;
    displayAllStudents(db);

    // Close the database connection
    sqlite3_close(db);

    system("pause");
    return 0;
}
