/* This is a skeleton code for Optimized Merge Sort, you can make modifications as long as you meet 
   all question requirements*/  

#include <bits/stdc++.h>
#include "record_class.h"
#include <vector>

using namespace std;


//defines how many blocks are available in the Main Memory 
#define BUFFER_SIZE 22

// Records buffers[BUFFER_SIZE]; //use this class object of size 22 as your main memory
vector<Records> buffers(BUFFER_SIZE);
vector<string> tempEmpFiles;
vector<string> tempDeptFiles;

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

//Sorting the buffers in main_memory and storing the sorted records into a temporary file (runs) 
void Sort_Buffer(bool deptFlag, fstream &inputRelation) {
    string runFileName;
    Records tempRecord;
    int runs = 0;
    int counter = 0;

    // Count number of rows in inputRelation
    /*
        int rowCount = 0;
        streampos pos = inputRelation.tellg();
        string line;
        while (getline(inputRelation, line)) {
            rowCount++;
        }
        inputRelation.seekg(pos, ios::beg);
       */
    
    

    //if (deptFlag) {
        // cout<< "Sorting Dept" << endl;
    //} else {
        // cout<< "Sorting Emp" << endl;
    //}
    
    // Loop gets records from Emp.csv and loads them into the page buffer: loads a single record
    // Terminates when main memory is full or no more records to load

    // Outer loop: 
    // 1. Load records into buffer
    // 2. Sort buffer
    // 3. Write sorted buffer to temporary file
    // 4. Clear buffer
    // 5. Repeat until no more records to load

    // Terminate loop when no more records to load
    if (deptFlag)
        // cout<< "entering loop" << endl;
    while (tempRecord.no_values != -1) {

        // Load records into the buffer
        //if (deptFlag)
            // cout<< "Loading Dept records into buffer" << endl;
        //else
            // cout<< "Loading Emp records into buffer" << endl;
            
        while (counter < BUFFER_SIZE && tempRecord.no_values != -1) {
            // cout<< "counter: " << counter << endl;
            if (deptFlag) {
                tempRecord = Grab_Dept_Record(inputRelation);
                cout<< "tempRecord.did: " << tempRecord.dept_record.did << endl;
            }
            else {
                tempRecord = Grab_Emp_Record(inputRelation);
                // cout<< "tempRecord.eid: " << tempRecord.emp_record.eid << endl;
            }
            // cout<< "tempRecord.no_values: " << tempRecord.no_values << endl;
            if (tempRecord.no_values == -1) {
                break;
                }
            
            // Load record into buffer
            // cout<< "Loading record into buffer" << endl;
            // cout<< "Buffer size: " << buffers.size() << endl;
            //// cout<< "Printing buffer.at(counter): " << buffers.at(counter) << endl;
            buffers.at(counter++) = tempRecord;
            // cout<< "Record loaded into buffer" << endl;
        }

        cout << "printing unsorted buffer: " << endl;
        for (Records& it: buffers){
            if (deptFlag) {
                it.printDeptRecord();
            } else {
                it.printEmpRecord();
            }
        }
        // Sort the buffer
        if (deptFlag) {
            // cout<< "Sorting Dept buffer" << endl;
            std::sort(buffers.begin(), buffers.end(), [](const Records& a, const Records& b) {
                return a.dept_record.managerid < b.dept_record.managerid;
                for (Records& it: buffers){
                    // cout<< it.dept_record.managerid << endl;
                }
            });
        } else {
            //// cout<< "Sorting Emp buffer" << endl;
            std::sort(buffers.begin(), buffers.end(), [](const Records& a, const Records& b) {
                return a.emp_record.eid < b.emp_record.eid;
            });
        }

        cout << "printing sorted buffer: " << endl;
        for (Records& it: buffers){
            if (deptFlag) {
                it.printDeptRecord();
            } else {
                it.printEmpRecord();
            }
        }

        // Write sorted records to temporary files (runs)
        vector<Records>::iterator it = buffers.begin();
        if (deptFlag) {
            //if (rowCount < BUFFER_SIZE){
              //      runFileName = "deptSorted.csv";
                //    tempDeptFiles.push_back(runFileName);
            //} else {
            runFileName = "initialRunDept" + to_string(tempDeptFiles.size()) + ".csv";
            tempDeptFiles.push_back(runFileName);
            //}
        } else {
            //if (rowCount < BUFFER_SIZE){
              //      runFileName = "empSorted.csv";
                //    tempEmpFiles.push_back(runFileName);
            //} else {
            runFileName = "initialRunEmp" + to_string(tempEmpFiles.size()) + ".csv";
            tempEmpFiles.push_back(runFileName);
            }
        }
        
        std::fstream runFile(runFileName, std::ios::out | std::ios::trunc);
        // Write records to run file
        while (it != buffers.end() && it->no_values != -1) {
            if (deptFlag) {
                if (it->dept_record.managerid == 0) {
                    ++it;
                    continue;
                }
                //// cout<< "Writing Dept record to file: " << runFileName << endl;
                runFile << it->dept_record.did << "," << it->dept_record.dname << "," << it->dept_record.budget << "," << it->dept_record.managerid << endl;
                ++it;
            } else {
                if (it->emp_record.eid == -1) {
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
                it.dept_record.did = 0;
                it.dept_record.dname = "";
                it.dept_record.budget = 0;
                it.dept_record.managerid = 0;
                it.no_values = 0;
            }
        } else {
            for (Records& it : buffers) {
                it.emp_record.eid = -1;
                it.emp_record.ename = "";
                it.emp_record.age = -1;
                it.emp_record.salary = -1;
                it.no_values = 0;
            }
        }

        // Clear buffer if last sorting 
        //if (deptFlag ){
        //    if (tempRecord.no_values == -1) {
        //        buffers.clear();
        //        break;
        //    }
        //}
    }
    
    // Function complete
    return;
}



//Prints out the attributes from empRecord and deptRecord when a join condition is met 
//and puts it in file Join.csv
void PrintJoin() {
    
    return;
}

//Use main memory to Merge and Join tuples 
//which are already sorted in 'runs' of the relations Dept and Emp 
void Merge_Join_Runs(bool deptFlag, vector<string> tempFiles, int iteration = 0){
   
    // Initialize function assets
    // Priority queue to hold the smallest record from each run
    std::priority_queue<Records, std::vector<Records>, CompareRecordsEmp> mainMemoryEmp;
    std::priority_queue<Records, std::vector<Records>, CompareRecordsDept> mainMemoryDept;
    Records tempRecord;
    int tmpIndex;
    std::fstream newRunFile;
    int loopIndex = 0;
    int newRuns = 0; 
    iteration++;
    int incrementer = 0;
    string newFileName;

    // Vector to hold new temporary files, for next iteration of runs
    vector<string> newTempFiles;

    int runs = tempFiles.size();
    
    // Go until there are no more runs to merge
    while (runs > 0) {
        // Open streams for as many runs as possible: up to 21 in this case
        vector<std::fstream> runFiles(BUFFER_SIZE - 1);

        // Determine how many runs to merge in this iteration, up to buffer max
        if (runs > BUFFER_SIZE - 1) {
            loopIndex = BUFFER_SIZE - 1;
        } else {
            loopIndex = runs;
        }

        // Populate the priority queue with the first record from each run
        for (int i = 0; i < loopIndex; i++) {
            // Open run file
            runFiles.at(i).open(tempFiles.at(i + incrementer), std::ios::in);
            // Load first record from run file
            if (deptFlag) {
                tempRecord = Grab_Dept_Record(runFiles.at(i));
            } else {
                tempRecord = Grab_Emp_Record(runFiles.at(i));
            }
         
         
            // EOF reached for run file, skip
            if (tempRecord.no_values == -1) {
                continue;
            }
            // Set file stream index
            tempRecord.fileStreamIndex = i;
            // Add record to priority queue
            if (deptFlag) {
                mainMemoryDept.push(tempRecord);
            } else {
                mainMemoryEmp.push(tempRecord);
            }
            
        }

        // Used to increment through tempFiles, specifically
        incrementer += loopIndex;

        // We now have a priority queue with the first record from each run loaded into main memory
        // If there are guaranteed to be more than one output file for this iteration, open as one of multiple runs
        if (runs > BUFFER_SIZE - 1 || newRuns > 0 ) {
            if (deptFlag) {
            newFileName = "newRunDept_" + to_string(iteration) + "_" + to_string(newRuns) + ".csv";
            } else {
            newFileName = "newRunEmp_" + to_string(iteration) + "_" + to_string(newRuns) + ".csv";
            }
            newRunFile.open(newFileName, std::ios::out | std::ios::trunc);
            newTempFiles.push_back(newFileName);
            newRuns++;
        } 
        // Otherwise, write to EmpSorted.csv
        else if (!deptFlag) {
            newRunFile.open("EmpSorted.csv", std::ios::out | std::ios::trunc);
        }
        else if (deptFlag) {
            newRunFile.open("DeptSorted.csv", std::ios::out | std::ios::trunc);
        } else {
            cerr << "Error: Could not open file" << endl;
            exit(-1);
        }

        // Check if file opened
        if (!newRunFile.is_open()) {
            cerr << "Error: Could not open file" << endl;
            exit(-1);
        }
        
        // Write records to new run file as long as there are records in main memory
        if (deptFlag){
        while (!mainMemoryDept.empty()) {
            
            // Retrieve smallest record
            tempRecord = mainMemoryDept.top();
            // Then eove smallest record from priority queue
            mainMemoryDept.pop();
            if (tempRecord.dept_record.did <= 0) {
                continue;
            }

            // Write smallest record to new run file
            if (deptFlag) {
                newRunFile << tempRecord.dept_record.did << "," << tempRecord.dept_record.dname << "," << tempRecord.dept_record.budget << "," << tempRecord.dept_record.managerid << endl;
            } else {
                newRunFile << tempRecord.emp_record.eid << "," << tempRecord.emp_record.ename << "," << tempRecord.emp_record.age << "," << tempRecord.emp_record.salary << endl;
            }
            // Replace smallest record in priority queue with next record from the same run
            // Make sure to correctly associate the new record with the correct file stream
            tmpIndex = tempRecord.fileStreamIndex;
          
            tempRecord = Grab_Dept_Record(runFiles.at(tempRecord.fileStreamIndex));
          
            tempRecord.fileStreamIndex = tmpIndex;

            // EOF reached for run file
            if (tempRecord.no_values == -1) {
                continue;
            } else {
                // Add replacement record to main memory
                mainMemoryDept.push(tempRecord);
            }
        }
        } else if (!deptFlag) {
            while (!mainMemoryEmp.empty()) {
            
            // Retrieve smallest record
            tempRecord = mainMemoryEmp.top();
            // Then emove smallest record from priority queue
            mainMemoryEmp.pop();

            // Write smallest record to new run file
           
            newRunFile << tempRecord.emp_record.eid << "," << tempRecord.emp_record.ename << "," << tempRecord.emp_record.age << "," << tempRecord.emp_record.salary << endl;
            
            // Replace smallest record in priority queue with next record from the same run
            // Make sure to correctly associate the new record with the correct file stream
            tmpIndex = tempRecord.fileStreamIndex;
          
            tempRecord = Grab_Emp_Record(runFiles.at(tempRecord.fileStreamIndex));
            
            tempRecord.fileStreamIndex = tmpIndex;

            // EOF reached for run file
            if (tempRecord.no_values == -1) {
                continue;
            } else {
                // Add replacement record to main memory
                mainMemoryEmp.push(tempRecord);
            }
        }
        }
        // Decrement total runs by the number processed this iteration through the loop
        runs -= loopIndex;
        // Close run file currently being written to
        newRunFile.close();
        
    } // Ends when all runs for current iteration have been merged

    // Delete temporary files from completed iteration
    for (string& file : tempFiles) {
        remove(file.c_str());
    }

    // If a new iteration of run was produced, call Merge_Runs recursively on new set of runs and new set of tempFile names
    if (newRuns > 0) {
            newRunFile.close();
            Merge_Join_Runs(deptFlag, newTempFiles, iteration);  // Recursively merge runs until all runs are merged
        }  
}

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

void Join_Runs(fstream &joinout) {
    cout << "Joining runs" << endl;
    fstream empOut;
    fstream deptOut;
    empOut.open("EmpSorted.csv", ios::in);
    deptOut.open("DeptSorted.csv", ios::in);


    Records empRecord;
    Records deptRecord;
    Records joinRecord;
    Records tempRecord;
    streampos tempPosBefore;
    streampos tempPosAfter;

    empRecord = Grab_Emp_Record(empOut);
    buffers.at(0) = empRecord;
    deptRecord = Grab_Dept_Record(deptOut);
    buffers.at(1) = deptRecord;
    while (empRecord.no_values != -1 || deptRecord.no_values != -1) {

        // If match is found, search for duplicate matches and perform cartisian product
        if (empRecord.emp_record.eid == deptRecord.dept_record.managerid) {
            tempRecord = deptRecord;
            tempPosBefore = deptOut.tellg();
            // cout<< "tempPosBefore: " << tempPosBefore << endl;
            // Compare successive records in department file to find duplicates
            // Emp record remains the same
            while (empRecord.emp_record.eid == deptRecord.dept_record.managerid) {
                joinRecord = Join_Tuples(empRecord, deptRecord);
                joinout << joinRecord.join_record.eid << "," << joinRecord.join_record.ename << "," << joinRecord.join_record.age << "," << joinRecord.join_record.salary << "," << joinRecord.join_record.did << "," << joinRecord.join_record.dname << "," << joinRecord.join_record.budget << endl;
                deptRecord = Grab_Dept_Record(deptOut);
                buffers.at(1) = deptRecord;
            }

            // Reset department file stream position to before comparison, so we can do the same with the matching dept record
            tempPosAfter = deptOut.tellg();
            // cout<< "tempPosAfter: " << tempPosAfter << endl;
            deptOut.seekg(tempPosBefore);
            // cout<< "Asert: deptOut.seekg(tempPosBefore): " << tempPosBefore << " == " << deptOut.tellg() << endl;

            deptRecord = tempRecord;
            // Compare successive records in employee file to find duplicates
            // Dept record remains the same
            while (empRecord.emp_record.eid == deptRecord.dept_record.managerid) {
                joinRecord = Join_Tuples(empRecord, deptRecord);
                joinout << joinRecord.join_record.eid << "," << joinRecord.join_record.ename << "," << joinRecord.join_record.age << "," << joinRecord.join_record.salary << "," << joinRecord.join_record.did << "," << joinRecord.join_record.dname << "," << joinRecord.join_record.budget << endl;
                empRecord = Grab_Emp_Record(empOut);
                buffers.at(0) = empRecord;
            }
            // Reset dept stream to pos after initial comparison
            
            deptOut.seekg(tempPosAfter);
            // cout<< "Assert: deptOut.seekg(tempPosAfter): " << deptOut.tellg() << " == " << tempPosAfter << endl;

        // If eid is less than managerid, grab next employee record
        } else if (empRecord.emp_record.eid < deptRecord.dept_record.managerid) {
            empRecord = Grab_Emp_Record(empOut);
            buffers.at(0) = empRecord;
        // If managerid is less than eid, grab next department record
        } else {
            deptRecord = Grab_Dept_Record(deptOut);
            buffers.at(1) = deptRecord;
        }
    }
    cout << "Join complete" << endl;
}




    /*
    Grab first record from each file
    Compare the records on eid and managerid
    If match, join the records and write to Join.csv

    If not and eid < managerid, grab next record from EmpSorted.csv
    If not and eid > managerid, grab next record from DeptSorted.csv

    Check for eof
    Handle duplicates
    Other edge cases?
    
    */


int main() {

    //Open file streams to read and write
    //Opening out two relations Emp.csv and Dept.csv which we want to join
    fstream empin;
    fstream deptin;
    bool empFlag = false;
    bool deptFlag = true;
    empin.open("Emp.csv", ios::in);
    deptin.open("Dept.csv", ios::in);
   
    //Creating the Join.csv file where we will store our joined results
    fstream joinout;
    joinout.open("Join.csv", ios::out | ios::app);

    //1. Create runs for Dept and Emp which are sorted using Sort_Buffer()
    Sort_Buffer(empFlag, empin);
   
    Sort_Buffer(deptFlag, deptin);


    //2. Use Merge_Join_Runs() to Join the runs of Dept and Emp relations 
    //Merge_Join_Runs(empFlag, tempEmpFiles, 0);
    //Merge_Join_Runs(deptFlag, tempDeptFiles, 0);

    //Join_Runs(joinout);
    


    //Please delete the temporary files (runs) after you've joined both Emp.csv and Dept.csv

    return 0;
}
