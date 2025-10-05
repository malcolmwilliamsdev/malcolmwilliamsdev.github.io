#include "Course.h"

/// <summary>
/// Default constructor
/// </summary>
Course::Course() {};

/// <summary>
/// Constructor with definitions
/// </summary>
/// <param name="courseNum"></param>
/// <param name="courseTitle"></param>
/// <param name="coursePrereqs"></param>
Course::Course(string& courseNum, string& courseTitle, vector<string>& coursePrereqs)
{
	number = courseNum;
	title = courseTitle;
	prereqNumbers = coursePrereqs;
}

/// <summary>
/// Assign course number
/// </summary>
/// <param name="courseNum">Course Number</param>
void Course::SetNumber(string& courseNum)
{
    this->number = courseNum;
}

/// <summary>
/// Assign course title
/// </summary>
/// <param name="courseTitle">Course Title</param>
void Course::SetTitle(string& courseTitle)
{
    this->title = courseTitle;
}

/// <summary>
/// Assign course prerequisite numbers
/// </summary>
/// <param name="coursePrereqs">Address of prerequisite vector</param>
void Course::SetPrereqs(vector<string>& coursePrereqs)
{
    this->prereqNumbers = coursePrereqs;
}

/// <summary>
/// Returns course number
/// </summary>
string Course::GetNumber()
{
    return this->number;
}

/// <summary>
/// Returns course title
/// </summary>
string Course::GetTitle()
{
    return this->title;
}

/// <summary>
/// Print course number, title, and prerequisites to std output
/// </summary>
void Course::PrintSelf()
{
    // print number/title
	cout << number << " - " << title;

    // print prerequisites
    if (!prereqNumbers.empty()) {
        cout << endl << "    Prerequisites: ";

        // grab position of last index
        auto lastPrereq = prereqNumbers.end() - 1;

        // iterate over prereqs
        for (auto prereq = prereqNumbers.begin(); prereq < prereqNumbers.end(); prereq++) {
            // print each prereq
            cout << *prereq;

            // print trailing comma if not last
            if (prereq != lastPrereq) {
                cout << ", ";
            }
        }
    }

    

    // new lines for console readability
    cout << endl << endl;
}

