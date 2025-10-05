#pragma once

#include "CourseBST.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class CourseParser
{
private:
	void splitLine(const string& line, vector<string>& out);

public:
	bool PopulateBST(CourseBST& bst, const string& fileName);
};

