#include "CourseParser.h"

void CourseParser::splitLine(const string& line, vector<string>& out)
{
	// clear the out vector
	out = vector<string>();

	// make a stream of the line so we can separate our values with getline
	stringstream lineStream(line);
	string parameter;

	// pass comma delim to getline to separate the parameters
	while (getline(lineStream, parameter, ',')) {
		// grab the parameter value
		out.push_back(parameter);
	}
}

/// <summary>
/// Read a CSV file containing course data and load it into a BST if the file is valid.
/// Returns true upon a successful data load.
/// </summary>
/// <param name="bst"></param>
/// <param name="fileName"></param>
/// <returns></returns>
bool CourseParser::PopulateBST(CourseBST& bst, const string& fileName)
{
	// create file object
	ifstream file(fileName);

	// file didn't open
	if (!file.is_open()) {
		cerr << "Could not open " << fileName << endl << endl;
		return false;
	}

	// the downloadable "ABCU_Advising_Program_Input.txt" was prefixed with UTF8 BOM encoding, causing parsing errors when read from the beginning
	char bom[3];
	file.read(bom, 3);
	if (bom[0] != '\xEF' || bom[1] != '\xBB' || bom[2] != '\xBF') {
		// read from beginning if not UTF8 BOM encoded 
		file.seekg(0);
	}

	// stores parsed course numbers for cross checking prereqs
	unordered_set<string> validPrereqs; // hashtable is better than vector here for searching

	// track the current line and file position
	int lineNumber = 0; // 0 so first line is 1
	streampos filePosition; 

	// holds line data
	string line;

	// loop over every line in the file
	while (getline(file, line)) {
		// update file position
		lineNumber++;
		filePosition = file.tellg();
		
		// holds parsed csv values
		vector<string> parameters;

		// separate values in a single line
		splitLine(line, parameters);

		// validate each line has at least 2 params
		if (parameters.size() < 2) {
			cerr << "Line " << lineNumber << " does not have at least 2 parameters" << endl << endl;
			return false;
		}

		// save our parameters, the parameters vector will be reused later
		string courseNumber = parameters.at(0);
		string courseTitle = parameters.at(1);
		vector<string> coursePrereqs;

		for (int i = 2; i < parameters.size(); i++) {
			coursePrereqs.push_back(parameters.at(i));
		}

		// keep record of the course number for prereq crosschecks
		validPrereqs.insert(courseNumber);

		// validate prereqs here
		for (auto const& prereq : coursePrereqs) {
			// prereq hasn't been encountered yet as a first course so we have to read further into the file
			if (validPrereqs.find(prereq) == validPrereqs.end()) {
				// valid flag is needed for logic at end of search 
				bool valid = false;

				// loop through remaining lines in the file
				while (getline(file, line)) {
					// parse line
					splitLine(line, parameters);

					// add the additional course numbers we encounter as we read ahead to reduce future reads
					validPrereqs.insert(parameters[0]);

					// if the prereq is at the beginnig of another line then its good
					if (prereq == parameters[0]) {
						// set valid flag
						valid = true;

						// reset file position to continue parsing from the current line
						file.seekg(filePosition);
						
						// break this innermost loop to continue checking the following prereqs
						break;
					}
				}
				
				// we got to the end of the file or broke from the search because we found the prereq
				if (!valid) { // check valid flag to know why search ended
					cerr << "Line " << lineNumber << ": " << prereq << " is an invalid prerequisite as it is not a primary course elsewhere in the file." << endl << endl;
					return false;
				}

				// must have broke loop on purpose, continue to check next prereq
			}
		}

		// a single line has been validated... it is safe to create the course object
		Course* newCourse = new Course(courseNumber, courseTitle, coursePrereqs);
		
		// add the course objext to the BST
		bst.Insert(newCourse);
	}

	// all values parsed, validated, and inserted
	// close the file and return true
	file.close();
	return true;
}
