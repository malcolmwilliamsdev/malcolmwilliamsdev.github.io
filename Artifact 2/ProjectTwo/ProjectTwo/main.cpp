/*
 * Project Two
 * ABCU Course Advising Program
 * 
 * Malcolm Williams
 * CS300 DSA
 * Southern New Hamsphire University
 * 5 December 2023
*/

#include "CourseParser.h"

int main() {
	// reads data
	CourseParser parser;
	// holds data
	CourseBST courseBST;

	// menu fields
	int menuChoice = 0;
	string fileName;
	string inputCourseNum;
	Course* inputCourse;
	
	// welcome message
	cout << "====ABCU Course Advising====" << endl << endl;

	// menu logic
	while (menuChoice != 9) {
		// print options
		cout << "Menu:" << endl;
		cout << "  1. Load Data" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  9. Exit" << endl << endl;
		cout << "Enter an option number: ";

		// if choice is not an integer print message
		if (!(cin >> menuChoice)) {
			//clear stream error
			cin.clear();
			
			// ignore bad input
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "Please enter the number you want from the menu." << endl << endl;
			
			// restart the loop
			continue;
		}

		cout << endl;

		switch (menuChoice) {
		case 1: // load data
			cout << "Enter the name of the course data file: ";
			cin >> fileName;
			cout << endl;

			// if tree already has data, clear the tree
			if (!courseBST.Empty()) {
				courseBST.Clear();
			}

			// read file and put data in the BST
			if (parser.PopulateBST(courseBST, fileName)) {
				cout << "Successfully loaded " << fileName << endl << endl;
			} // else error, but PopulateBST prints its own mesesages
			break;
		case 2: // print sorted course list
			// if data not loaded stop and print message
			if (courseBST.Empty()) {
				cout << "Data must be loaded first." << endl << endl;
				break;
			}

			cout << "=======Course List=======" << endl << endl;
			courseBST.PrintAllSorted();
			break;
		case 3: // search and print course
			// if data not loaded stop and print message
			if (courseBST.Empty()) {
				cout << "Data must be loaded first." << endl << endl;
				break;
			}

			cout << "Enter a course number: ";
			cin >> inputCourseNum;
			cout << endl;

			// capitalize input to avoid case errors during search
			for (auto& ch : inputCourseNum) {
				ch = toupper(ch);
			}

			// return course with specified course number
			inputCourse = courseBST.Search(inputCourseNum);
			if (inputCourse != nullptr) {
				cout << "=======" << inputCourse->GetNumber() << "=======" << endl;
				inputCourse->PrintSelf();
			}
			else {
				cout << inputCourseNum << " not found." << endl << endl;
			}
			break;
		case 9: // exit application
			cout << "Goodbye!" << endl;
			break;
		default: // entered some invalid integer
			cout << menuChoice << " is not a valid option." << endl << endl;
			break;
		}
	}
	
	return 0;
}