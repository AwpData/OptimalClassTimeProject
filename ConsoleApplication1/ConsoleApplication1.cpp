#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <iomanip>

#pragma warning(disable : 4996) // supresses error for the outdated 'strtok' method 
using namespace std;

struct StudentNode { // StudentNode differs from ListNode struct as it stores a 'string' instead of an 'int' value and overall holds less stuff 
	string name;
	StudentNode* next;

	StudentNode(string name, StudentNode* next = nullptr) {
		this->name = name;
		this->next = next;
	}
};

class Students { // This is the linked list for student names within a linked list for the times of a certain day 
private:
	StudentNode* front;
public:
	Students() {
		front = nullptr;
	}
	void add(string name) { // Adds student names (see main linked list program as it is relatively the same method) 
		if (front == nullptr) {
			front = new StudentNode(name);
		}
		else {
			StudentNode* current = front;
			while (current->next != nullptr) {
				current = current->next;
			}
			current->next = new StudentNode(name);
		}
	}

	void display() { // displays each name in the linked list 
		StudentNode* current = front;
		while (current != nullptr) {
			cout << current->name << " ";
			current = current->next;
		}
	}
	~Students() { // Destructor for all student linked lists 
		StudentNode* current = front;
		StudentNode* garbage = current;
		while (current != nullptr) {
			garbage = current;
			current = current->next;
			delete garbage;
		}
		front = nullptr;
	}
};

struct ListNode { // ListNode stores the 'double' time as well as a counter, students linked list, and the next node it points to 
	double time;
	int counter;
	Students* students; // Linked Node List of students 
	ListNode* next;

	ListNode(double time, ListNode* next = nullptr) {
		this->time = time;
		this->next = next;
		this->students = new Students;
		this->counter = 0;
	}
};

class LinkedList {
private:
	ListNode* front;
public:
	LinkedList() {
		front = nullptr;
	}

	void add(double time, string studentName) {
		if (front == nullptr) { // If the list is empty 
			front = new ListNode(time);
			front->counter++;
			front->students->add(studentName);
		}
		else if (time <= front->time) { // If the desired time is less than the front time, make it the new front 
			if (front->time == time) { // Linked List already has the time...
				// If-statement used to skip over the next one if we have extra times 
			}
			else { // Add this time to the linked list because it doesn't have it 
				ListNode* previous = front;
				front = new ListNode(time, previous);
			}
			front->counter++; // Either way, we have to increment counter in both cases (avoiding repetitive code) 
			front->students->add(studentName); // And add the students name 
		}
		else { // Anywhere else in the list 
			ListNode* current = front;
			ListNode* previous = front;
			while (current != nullptr && time >= current->time) { // Loops until nullptr (add to end) or if the selected node is less than our time (for ascending time order)
				// I used >= because it will also check if the time is already in this slot so I can just increment instead of adding the time
				previous = current;
				current = current->next;
			}
			if (previous->time == time) { // Checks if the time is equal to the one we looped to 
				previous->counter++; // If so, we must increment this counter instead of creating a new node 
				previous->students->add(studentName); // And add the student name 
			}
			else { // Linked List does not have this time yet, so add it 
				previous->next = new ListNode(time, current); // I set the next node from previous to be a new node that points to the next-next node from previous 
				previous->next->counter++; // This code differs from "previous->counter++" since I have to increment counter to the new one 
				previous->next->students->add(studentName); // Same for this case but for adding student name 
			}
		}
	}

	void displayList(int day, int matches = 1) { // Displays the Linked List for a given day (in numerical form) 
		ListNode* current = front;
		string dayText;

		switch (day) { // Adjusts day integer value to string for readability 
		case 0:
			dayText = "Monday";
			break;
		case 1:
			dayText = "Tuesday";
			break;
		case 2:
			dayText = "Wednesday";
			break;
		case 3:
			dayText = "Thursday";
			break;
		case 4:
			dayText = "Friday";
			break;
		case 5:
			dayText = "Saturday";
			break;
		case 6:
			dayText = "Sunday";
			break;
		}

		cout << fixed << showpoint << setprecision(1); // Quick output formatting 
		while (current != nullptr) { // While there are still students in the list 
			// FORMAT: (day): (time) (# of students) chose this: (student#) (student#) ... 
			if (current->counter >= matches) { // Default is 1, but it will only print times with (matches) # of matches 
				cout << dayText << ": "; // First print the day
				cout << current->time << " "; // Then the time
				cout << right << setw(10) << current->counter << " student(s) chose this: "; // Then how many students chose this time
				current->students->display(); // And finally display the students who chose this time via the student linked list 
				cout << endl;
			}
			current = current->next;
		}
	}

	~LinkedList() { // Calls after program finishes to free up memory we made with the linked lists 
		ListNode* current = front;
		ListNode* garbage;
		while (current != nullptr) {
			garbage = current;
			current = current->next;
			delete garbage->students; // calls student's destructor to delete the student linked list attached to the day's time's linked node 
			delete garbage;
		}
		front = nullptr;
	}
};
int main()
{
	string fileName;
	ifstream timeFile;
	LinkedList startTimes[7]; // 0 = mon, 1 = tues, 2 = weds, 3 = thurs, 4 = fri, 5 = sat, 6 = sun
		// Creates individual linked lists for each day of the week to store times in them 
	bool moreFiles = true;

	while (moreFiles) { // This is the main program on a loop (if the user wants to add more files that is...) 
		while (true) { // This is file validation loop
			cout << "\nPlease enter the name of your time file ('cancel' to cancel)" << endl;
			getline(cin, fileName); // Gets file name
			if (fileName == "cancel") { // If the user enters cancel, then it will break out of file parsing 
				break;
			}
			timeFile.open(fileName); // Attempts to open it
			if (timeFile.fail()) { // Failure
				cout << "Error: could not find file, try again" << endl;
				continue; // try again
			}
			else { // Success!
				cout << "Opening: " << fileName << endl;
				break; // move on 
			}
		}
		string input = "";
		int dayIndex = 0;
		while (getline(timeFile, input)) { // This is the file parsing loop 
			if (input == "") { // Some files have an extra blank line in them, so just in case this happens, I break to avoid crash 
				break;
			}

			const char* s = input.c_str(); // Converts string to c-string
			char* sCopy = new char[input.length() + 1]; // Creates another char array
			strncpy_s(sCopy, input.length() + 1, s, _TRUNCATE); // Copies the c-string 's' (which is made up of characters) into our char array 'sCopy'
			char* pch; // pch will be our string token splitter 

			pch = strtok(sCopy, ":"); // First get the day which is delimitted by a ':'
			string day = pch; // sets day = pch 
			if (day.find("mon") != -1) { // Each of these if-statements adjust the index of my Linked List array which holds each day 
				dayIndex = 0;
			}
			else if (day.find("tue") != -1) {
				dayIndex = 1;
			}
			else if (day.find("wed") != -1) {
				dayIndex = 2;
			}
			else if (day.find("thu") != -1) {
				dayIndex = 3;
			}
			else if (day.find("fri") != -1) {
				dayIndex = 4;
			}
			else if (day.find("sat") != -1) {
				dayIndex = 5;
			}
			else if (day.find("sun") != -1) {
				dayIndex = 6;
			}
			pch = strtok(NULL, " ,"); // Gets the first number by using 'space' and 'comma' as delimitters 
			if (pch == NULL) { // This will fire if there is no semi-colon or space
				cout << "\nError (line 236), file is poorly formatted. Cannot read the first time, please reformat or enter another file\n" << endl;
			}
			while (pch != NULL) { // While pch can stil find more numbers (if pch is initially null, this won't execute at all
				string preParsedTime = pch; // first I have to set the chars to a string
				double time;
				try {
					time = stod(preParsedTime); // Tries to convert the given string to a double value 
				}
				catch (exception e) { // This will execute if the time cannot be processed to a double 
					cout << "\n Error (line 246), found a badly formatted time -> (" << pch << "). I am skipping over the rest of the file.\n" << endl;
					break;
				}
				// Then I parse string to double (because of possible half-hour times at '.5' value) 
				cout << "Time " << time << " processed..." << endl;
				startTimes[dayIndex].add(time, fileName); // Then I add the time to my LinkedList array based on the day index 
				pch = strtok(NULL, " ,"); // Gets the next number if same delimitters 

			}
			delete[] sCopy; // Free up memory!
		}

		// After file parsing....
		cout << "\nDo you have any more files? (Y/N)" << endl; // Asks if there are anymore files 
		cin.clear();
		string choice;
		cin >> choice;
		if (choice == "Y" || choice == "y") { // If yes
			cin.clear();
			timeFile.close(); // close previous file so we can open another one 
			cin.ignore();
			continue;
		}
		else { // If no, then we display the student list 		
			moreFiles = false; // No more files to add
			int matches = -1; // Variable to check for matches
			while (matches != 0) { // While the user does not want to quit (matches == 0)
				cout << "\nFull Time Data:\n" << endl;
				for (int i = 0; i < 7; i++) { // First it prints the entire list of all days and times 
					startTimes[i].displayList(i);
					cout << "\n";
				}

				// MINIMUM MATCH CODE 
				cout << "How many matches should I display minimum? (0 to quit)" << endl;
				cin >> matches; // Gets number of matches user wants minimum 
				if (matches == 0) { // if = 0, then quit 
					cout << "Quitting program..." << endl;
					break;
				}
				else if (matches < 0 || matches > 4) { // This is out of range checker which is counted as invalid input 
					cout << "Invalid input, press ENTER to try again" << endl;
					cin.ignore();
					cin.get();
				}
				else { // Valid input 
					cout << "Time Data limited to " << matches << " minimum matches:\n" << endl;
					for (int i = 0; i < 7; i++) { // Displays user's minimum matches by replacing the default argument of the second parameter
						startTimes[i].displayList(i, matches);
					}
					cout << "\nPress ENTER to continue" << endl; // Just a brief pause (or wait until the user presses ENTER) so they can read the list 
					cin.ignore();
					cin.get();
				}
			}
		}
	}
	timeFile.close();
} // Destructor called here to free up all memory used by the linked lists 