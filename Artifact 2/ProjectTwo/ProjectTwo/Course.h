#pragma once

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Course
{
private:
	string number;
	string title;
	vector<string> prereqNumbers;

public:
	Course();
	Course(string& courseNum, string& courseTitle, vector<string>& coursePrereqs);

	void SetNumber(string& courseNum);
	void SetTitle(string& courseTitle);
	void SetPrereqs(vector<string>& coursePrereqs);

	string GetNumber();
	string GetTitle();

	void PrintSelf();
};