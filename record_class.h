/* This is a skeleton code for Optimized Merge Sort, you can make modifications as long as you meet 
   all question requirements*/  
/* This record_class.h contains the class Records, which can be used to store tuples form Emp.csv (stored
in the EmpRecords) and Dept.csv (Stored in DeptRecords) files.
*/
#include <bits/stdc++.h>

using namespace std;

class Records{
    public:
    
    struct EmpRecord {
        int eid = -1;
        string ename;
        int age;
        double salary;
    }emp_record;

    struct DeptRecord {
        int did = -1;
        string dname;
        double budget;
        int managerid = 0;
    }dept_record;

    struct JoinRecord {
        int eid = 0;
        string ename;
        int age;
        double salary;
        int did = 0;
        string dname;
        double budget;
    }join_record;

    int no_values = 0; //You can use this to check if you've don't have any more tuples
    int number_of_emp_records = 0; // Tracks number of emp_records you have on the buffer
    int number_of_dept_records = 0; //Track number of dept_records you have on the buffer
    int fileStreamIndex = 1; // Tracks the index of the file stream you are currently using

    void printEmpRecord() {
        cout << "Eid:" << emp_record.eid << ", Ename: " << emp_record.ename << ", Age: " << emp_record.age << ", Salary " << emp_record.salary << ", no_values: " << no_values << endl;
    }

    void printDeptRecord() {
        cout << "Did:" << dept_record.did << ", Dname: " << dept_record.dname << ", Budget: " << dept_record.budget << ", Managerid " << dept_record.managerid << ", no_values: " << no_values << endl;
    }

    void printJoinRecord() {
        cout << "Eid:" << join_record.eid << ", Ename: " << join_record.ename << ", Age: " << join_record.age << ", Salary " << join_record.salary << ", Did: " << join_record.did << ", Dname: " << join_record.dname << ", Budget: " << join_record.budget << endl;
    }

    Records(){};


    Records(int fileStreamIndex) {
        this->fileStreamIndex = fileStreamIndex;
    }
};

// Grab a single block from the Emp.csv file and put it inside the EmpRecord structure of the Records Class
Records Grab_Emp_Record(fstream &empin) {
    string line, word;
    Records emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.emp_record.eid = stoi(word);
        getline(s, word, ',');
        emp.emp_record.ename = word;
        getline(s, word, ',');
        emp.emp_record.age = stoi(word);
        getline(s, word, ',');
        emp.emp_record.salary = stod(word);

        //Ensuring that you cannot use both structure (EmpEecord, DeptRecord) at the same memory block / time 
        emp.dept_record.did = 0;
        emp.dept_record.dname = "";
        emp.dept_record.budget = 0;
        emp.dept_record.managerid = 0;

        return emp;
    } else {
        emp.no_values = -1;
        emp.emp_record.eid = -1;
        return emp;
    }
}

// Grab a single block from the Dept.csv file and put it inside the DeptRecord structure of the Records Class
Records Grab_Dept_Record(fstream &deptin) {
    string line, word;
    //DeptRecord dept;
    Records dept;
    if (getline(deptin, line, '\n')) {
        stringstream s(line);
        getline(s, word,',');
        cout << "ERR102: " << word << endl;
        dept.dept_record.did = stoi(word);
        getline(s, word, ',');
        dept.dept_record.dname = word;
        getline(s, word, ',');
        dept.dept_record.budget = stod(word);
        getline(s, word, ',');
        dept.dept_record.managerid = stoi(word);

        //Ensuring that you cannot use both structure (EmpEecord, DeptRecord) at the same memory block / time 
        dept.emp_record.eid = 0;
        dept.emp_record.ename = "";
        dept.emp_record.age = 0;
        dept.emp_record.salary = 0;
        return dept;
    } else {
        dept.no_values = -1;
        dept.dept_record.did = -1;
        return dept;
    }
}