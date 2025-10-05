using CourseAdvising.Models;

namespace CourseAdvising.BusinessLogic;

public static class CourseParser
{
    // Loads course data from a csv/txt file into the provided dictionary.
    public static void LoadData(string path, Dictionary<string, Course> courseData)
    {
        bool abort = false;
        string[] lines = File.ReadAllLines(path);

        foreach (var line in lines)
        {
            if (abort) break;
            try
            {
                // Expecting format: CourseCode, CourseName, Prereq1, Prereq2, ...
                string[] parts = line.Split(',');
                if (parts.Length < 2)
                {
                    throw new FormatException("Line does not contain enough parts for a course.");
                }

                string courseCode = parts[0].Trim();
                string courseName = parts[1].Trim();
                List<string> prereqs = parts
                    .Skip(2)
                    .Select(p => p.Trim())
                    .Where(p => !string.IsNullOrEmpty(p))
                    .ToList();

                // Add or update the course in the dictionary
                courseData[courseCode] = new Course(courseCode, courseName, prereqs);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error parsing line: {line}. Exception: {ex.Message}");
                abort = true; // Stop processing further lines on error
            }
        }
    }
}
