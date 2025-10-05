using CourseAdvising.Entities;
using CourseAdvising.Models;

namespace CourseAdvising.BusinessLogic;

public static class CourseBusinessLogic
{
    private static CourseMenu? menuRef;
    private static Dictionary<string, Course> courseData = new();
    private static bool dataLoaded = false;

    // Initializes the course data by loading from the default file.
    public static void Initialize(CourseMenu menu)
    {
        menuRef = menu;
        try
        {
            CourseParser.LoadData("./Resources/input.txt", courseData);
            dataLoaded = courseData.Count > 0;
            if (dataLoaded)
                Console.WriteLine("Successfully loaded ./Resources/input.txt\n");
        }
        catch
        {
            Console.WriteLine("Automatic data loading failed. Please use 'Load Data' to load a file.\n");
        }
    }

    // Loads course data from a specified file.
    public static void LoadData(string path)
    {
        string filename = path.Trim();
        if (string.IsNullOrWhiteSpace(filename))
        {
            Console.WriteLine("No filename entered.");
            return;
        }
        try
        {
            courseData.Clear();
            CourseParser.LoadData(filename, courseData);
            dataLoaded = courseData.Count > 0;
            if (dataLoaded)
                Console.WriteLine($"Successfully loaded {filename}\n");
            else
                Console.WriteLine($"No courses loaded from {filename}.");
        }
        catch
        {
            dataLoaded = false;
            Console.WriteLine($"Failed to load {filename}.");
        }
    }

    // Checks if the data is loaded.
    public static bool IsDataLoaded() => dataLoaded;

    // Prints a list of all courses and their prerequisites.
    public static void PrintCourseList(string _) // requires no argument
    {
        if (!ValidateDataLoaded()) return;

        Console.WriteLine("=======Course List=======\n");
        foreach (Course course in courseData.Values.OrderBy(c => c.Code)) // Sort courses by code
        {
            Console.WriteLine(course);
            Console.WriteLine();
        }
    }

    // Prints details of a specific course code entered by the user.
    public static void PrintCourse(string input)
    {
        if (!ValidateDataLoaded()) return;

        string courseNumber = input;
        if (string.IsNullOrWhiteSpace(courseNumber))
        {
            Console.WriteLine("No course number entered.");
            return;
        }

        string key = courseNumber.Trim().ToUpper();
        if (courseData.TryGetValue(key, out Course? course))
        {
            Console.WriteLine($"\n======={course.Code}=======");
            Console.WriteLine(course);
        }
        else
        {
            Console.WriteLine($"{key} not found.");
        }
    }

    // Exits the course menu
    public static void Exit(string _)
    {
        menuRef?.Exit();
    }

    // Validates that data has been loaded before proceeding with operations that require it.
    private static bool ValidateDataLoaded()
    {
        if (!dataLoaded)
        {
            Console.WriteLine("Data must be loaded first.\n");
            return false;
        }
        return true;
    }
}
