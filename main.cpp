#include <bits/stdc++.h>
#include "record_class.h"
#include <vector>
#include <limits.h>

using namespace std;

//defines how many blocks are available in the Main Memory 
#define BUFFER_SIZE 22

// Vectors we'll be using

// General main memory buffer
vector<Records> buffers(BUFFER_SIZE);
// Temporary file names
vector<string> tempEmpFiles;
vector<string> tempDeptFiles;

// Method to count number of valid records in the buffer
int countElements(vector<Records> &buffers, bool deptFlag) {
    int count = 0;
    if (deptFlag) {
        for (Records& it: buffers) {
            if (it.dept_record.did != INT_MAX) {
                count++;
            }
        }
    }
    else if (!deptFlag) {
        for (Records& it: buffers) {
            if (it.emp_record.eid != INT_MAX) {
                count++;
            }
        }
    }
    return count;
}

// Counts the number of valid records in the csv file
int relationRowCount(string filename) {
    string line;
    fstream inputRelation;
    inputRelation.open(filename, ios::in);
    int count = 0;
    while (getline(inputRelation, line)) {
        count++;
    }
    inputRelation.close();
    return count;
}

// Used in priority queue to sort records
struct CompareRecordsEmp {
    bool operator()(const Records& a, const Records& b) const {
        return a.emp_record.eid > b.emp_record.eid;
    }
};

struct CompareRecordsDept {
    bool operator()(const Records& a, const Records& b) const {
        return a.dept_record.managerid > b.dept_record.managerid;
    }
};

void sortEmpBuffer(vector<Records> &buffers) {
    std::sort(buffers.begin(), buffers.begin() + 11, [](const Records& a, const Records& b) {
        return a.emp_record.eid < b.emp_record.eid;
        }
    );
}

void sortDeptBuffer(vector<Records> &buffers) {
    std::sort(buffers.begin() + 11, buffers.end(), [](const Records& a, const Records& b) {
        return a.dept_record.managerid < b.dept_record.managerid;
        }
    );
}

// Joins tuples from the Emp and Dept relations
Records Join_Tuples(Records empRecord, Records deptRecord) {
    Records joinRecord;
    joinRecord.join_record.eid = empRecord.emp_record.eid;
    joinRecord.join_record.ename = empRecord.emp_record.ename;
    joinRecord.join_record.age = empRecord.emp_record.age;
    joinRecord.join_record.salary = empRecord.emp_record.salary;
    joinRecord.join_record.did = deptRecord.dept_record.did;
    joinRecord.join_record.dname = deptRecord.dept_record.dname;
    joinRecord.join_record.budget = deptRecord.dept_record.budget;
    return joinRecord;
}

// Sorts the buffers in main_memory and stores the sorted records into a temporary file (runs) 
void Sort_Buffer(bool deptFlag, fstream &inputRelation) {
    string runFileName;
    Records tempRecord;
    int runs = 0;
    int counter = 0;
    int numElems = 0;

    // Loop gets records from Emp.csv and loads them into the page buffer: loads a single record
    // Terminates when main memory is full or no more records to load

    // Outer loop: 
    // 1. Load records into buffer
    // 2. Sort buffer
    // 3. Write sorted buffer to temporary file
    // 4. Clear buffer
    // 5. Repeat until no more records to load

    // Terminate loop when no more records to load
    while (tempRecord.no_values != -1) {

        // Load records into buffer
        while (counter < BUFFER_SIZE - 1 && tempRecord.no_values != -1) {
            if (deptFlag) {
                tempRecord = Grab_Dept_Record(inputRelation);
            }
            else {
                tempRecord = Grab_Emp_Record(inputRelation);
            }

            // If there are no more records to load, break
            if (tempRecord.no_values == -1) {
                break;
                }

            // If there's room, add record to buffer
            if (counter < BUFFER_SIZE - 1)
                buffers.at(counter++) = tempRecord;
        }

        // Sort the buffer
        if (deptFlag) {
            std::sort(buffers.begin(), buffers.end(), [](const Records& a, const Records& b) {
                return a.dept_record.managerid < b.dept_record.managerid;
            });
        } else {
            std::sort(buffers.begin(), buffers.end(), [](const Records& a, const Records& b) {
                return a.emp_record.eid < b.emp_record.eid;
            });
        }

        // Write sorted records to temporary files (runs)
        vector<Records>::iterator it = buffers.begin();
        numElems = countElements(buffers, deptFlag);
        if (deptFlag) {
            runFileName = "deptRun" + to_string(tempDeptFiles.size()) + ".csv";
            tempDeptFiles.push_back(runFileName);
        } else {
            runFileName = "empRun" + to_string(tempEmpFiles.size()) + ".csv";
            tempEmpFiles.push_back(runFileName);
        }
        
        std::fstream runFile(runFileName, std::ios::out | std::ios::trunc);

        // Write records to run file
        while (it != buffers.end() && it->no_values != -1) {
            if (deptFlag) {
                if (it->dept_record.managerid == INT_MAX) {
                    ++it;
                    continue;
                }
                runFile << it->dept_record.did << "," << it->dept_record.dname << "," << it->dept_record.budget << "," << it->dept_record.managerid << endl;
                ++it;
            } else {
                if (it->emp_record.eid == INT_MAX) {
                    ++it;
                    continue;
                }
                runFile << it->emp_record.eid << "," << it->emp_record.ename << "," << it->emp_record.age << "," << it->emp_record.salary << endl;
                ++it;
            }
         
        } // Run complete

        // Increment runs
        runs++;

        // Close run file
        runFile.close();

        // Reset counter
        counter = 0;

        // Reset buffer record objects
        if (deptFlag) {
            for (Records& it : buffers) {
                it.dept_record.did = INT_MAX;
                it.dept_record.dname = "";
                it.dept_record.budget = 0;
                it.dept_record.managerid = INT_MAX;
                it.no_values = 0;
            }
        } else {
            for (Records& it : buffers) {
                it.emp_record.eid = INT_MAX;
                it.emp_record.ename = "";
                it.emp_record.age = -1;
                it.emp_record.salary = -1;
                it.no_values = 0;
            }
        }
    }
    
    // Function complete
    return;
}

// Performs a join on the sorted runs of the Emp and Dept relations, iterating through both and matching records on eid and managerid
void Join_Runs(fstream &joinout) {
    fstream empOut;
    fstream deptOut;
    bool empSmaller = relationRowCount("EmpSorted.csv") < relationRowCount("DeptSorted.csv");
    empOut.open("EmpSorted.csv", ios::in);
    deptOut.open("DeptSorted.csv", ios::in);
    Records joinRecord;
    int fileIndex = 0;

    tempDeptFiles;
    vector<fstream> deptFiles(11);
    vector<fstream> empFiles(11);
    vector<Records> tempFiles(11);
    tempEmpFiles;

    // Initialize file streams, load first records from each run
    for (int i = 0; i < tempEmpFiles.size(); i++) {
        empFiles.at(i).open(tempEmpFiles.at(i), ios::in);
        buffers.at(i) = Grab_Emp_Record(empFiles.at(i));
        buffers.at(i).fileStreamIndex = i;
    }

    for (int i = 0; i < tempDeptFiles.size(); i++) {
        deptFiles.at(i).open(tempDeptFiles.at(i), ios::in);
        buffers.at(i + 11) = Grab_Dept_Record(deptFiles.at(i));
        buffers.at(i + 11).fileStreamIndex = i;
    }

    // First sort of freshly load main memory
    sortEmpBuffer(buffers);
    sortDeptBuffer(buffers);
         
    // Continue as long as records remain in one file
    // As soon as one relation is exhausted, the join is complete: no more records to join
    while (buffers.at(0).no_values != -1 && buffers.at(11).no_values != -1) {
        if (empSmaller) {

            // If match is found, search for duplicate matches and perform cartisian product
            // Using one page at a time for comparison
            if (buffers.at(0).emp_record.eid == buffers.at(11).dept_record.managerid) {
                do {
                    // Join the matching records
                    joinRecord = Join_Tuples(buffers.at(0), buffers.at(11));
                    
                    // Write the join record to Join.csv
                    joinout << joinRecord.join_record.eid << "," << joinRecord.join_record.ename << "," << joinRecord.join_record.age << "," << joinRecord.join_record.salary << "," << joinRecord.join_record.did << "," << joinRecord.join_record.dname << "," << joinRecord.join_record.budget << endl;
                    
                    fileIndex = buffers.at(11).fileStreamIndex;
                    // Grab next department record
                    buffers.at(11) = Grab_Dept_Record(deptFiles.at(buffers.at(11).fileStreamIndex));
                    buffers.at(11).fileStreamIndex = fileIndex;
                    sortDeptBuffer(buffers);
                } while (buffers.at(0).emp_record.eid == buffers.at(11).dept_record.managerid);

                // Grab next employee record
                fileIndex = buffers.at(0).fileStreamIndex;
                buffers.at(0) = Grab_Emp_Record(empFiles.at(buffers.at(0).fileStreamIndex));
                buffers.at(0).fileStreamIndex = fileIndex;
                sortEmpBuffer(buffers);

            // If eid is lesser than managerid, grab next employee record
            } else if (buffers.at(0).emp_record.eid < buffers.at(1).dept_record.managerid) {
                fileIndex = buffers.at(0).fileStreamIndex;
                buffers.at(0) = Grab_Emp_Record(empFiles.at(buffers.at(0).fileStreamIndex));
                buffers.at(0).fileStreamIndex = fileIndex;
                sortEmpBuffer(buffers);
                //buffers.at(0).printEmpRecord();
            
            // If managerid is less than eid, grab next department record
            } else {
                fileIndex = buffers.at(11).fileStreamIndex;
                buffers.at(11) = Grab_Dept_Record(deptFiles.at(buffers.at(11).fileStreamIndex));
                buffers.at(11).fileStreamIndex = fileIndex;
                sortDeptBuffer(buffers);
                //buffers.at(11).printDeptRecord();
            }
        }
        else {

            // If match is found, search for duplicate matches and perform cartisian product
            // Using one page at a time for comparison
            if (buffers.at(0).emp_record.eid == buffers.at(11).dept_record.managerid) {
                    
                    do {

                        // Join the matching records
                        joinRecord = Join_Tuples(buffers.at(0), buffers.at(11));

                        // Write the join record to Join.csv
                        joinout << joinRecord.join_record.eid << "," << joinRecord.join_record.ename << "," << joinRecord.join_record.age << "," << joinRecord.join_record.salary << "," << joinRecord.join_record.did << "," << joinRecord.join_record.dname << "," << joinRecord.join_record.budget << endl;

                        // Set file stream index
                        fileIndex = buffers.at(0).fileStreamIndex;
                        // Grab next emp record
                        buffers.at(0) = Grab_Emp_Record(empFiles.at(buffers.at(0).fileStreamIndex));
                        buffers.at(0).fileStreamIndex = fileIndex;
                        sortEmpBuffer(buffers);
                    } while (buffers.at(0).emp_record.eid == buffers.at(11).dept_record.managerid);

                    // Grab next dept record
                    fileIndex = buffers.at(11).fileStreamIndex;
                    buffers.at(11) = Grab_Dept_Record(deptFiles.at(buffers.at(11).fileStreamIndex));
                    buffers.at(11).fileStreamIndex = fileIndex;
                    sortDeptBuffer(buffers);

                    do {
                    // Join the matching records
                    joinRecord = Join_Tuples(buffers.at(0), buffers.at(11));
                    
                    // Write the join record to Join.csv
                    joinout << joinRecord.join_record.eid << "," << joinRecord.join_record.ename << "," << joinRecord.join_record.age << "," << joinRecord.join_record.salary << "," << joinRecord.join_record.did << "," << joinRecord.join_record.dname << "," << joinRecord.join_record.budget << endl;
                    
                    fileIndex = buffers.at(11).fileStreamIndex;
                    // Grab next department record
                    buffers.at(11) = Grab_Dept_Record(deptFiles.at(buffers.at(11).fileStreamIndex));
                    buffers.at(11).fileStreamIndex = fileIndex;
                    sortDeptBuffer(buffers);
                    } while (buffers.at(0).emp_record.eid == buffers.at(11).dept_record.managerid);

                    // Grab next employee record
                    fileIndex = buffers.at(0).fileStreamIndex;
                    buffers.at(0) = Grab_Emp_Record(empFiles.at(buffers.at(0).fileStreamIndex));
                    buffers.at(0).fileStreamIndex = fileIndex;
                    sortEmpBuffer(buffers);
            
            // If managerid is lesser than eid, grab next dept record
            }else if (buffers.at(0).emp_record.eid > buffers.at(11).dept_record.managerid) {
                fileIndex = buffers.at(11).fileStreamIndex;
                buffers.at(11) = Grab_Dept_Record(deptFiles.at(buffers.at(11).fileStreamIndex));
                buffers.at(11).fileStreamIndex = fileIndex;
                sortDeptBuffer(buffers);
            
            // If eid is lesser than managerid, grab next emp record
            } else {
                fileIndex = buffers.at(0).fileStreamIndex;
                buffers.at(0) = Grab_Emp_Record(empFiles.at(buffers.at(0).fileStreamIndex));
                buffers.at(0).fileStreamIndex = fileIndex;
                sortEmpBuffer(buffers);
            }
        } 
    }

    // Delete temporary files from completed iteration
    for (string& file : tempEmpFiles) {

        if (remove(file.c_str()) != 0) {
            perror("Error deleting EmpSorted.csv");
        }
    }

    for (string& file : tempDeptFiles) {
        if (remove(file.c_str()) != 0) {
            perror("Error deleting DeptSorted.csv");
        }
    }
}

int main() {

    fstream empin;
    fstream deptin;
    bool empFlag = false;
    bool deptFlag = true;
    empin.open("Emp.csv", ios::in);
    deptin.open("Dept.csv", ios::in);
   
    //Creating the Join.csv file where we will store our joined results
    fstream joinout;
    joinout.open("Join.csv", ios::out | ios::trunc);

    //1. Create runs for Dept and Emp which are sorted using Sort_Buffer()
    Sort_Buffer(empFlag, empin);
    Sort_Buffer(deptFlag, deptin);

    //2. Use Merge_Join_Runs() to Join the runs of Dept and Emp relations 
    //Merge_Join_Runs(empFlag, tempEmpFiles);
    //Merge_Join_Runs(deptFlag, tempDeptFiles);

    //3. Join the sorted runs of Dept and Emp relations using Join_Runs()
    Join_Runs(joinout);

    return 0;
}