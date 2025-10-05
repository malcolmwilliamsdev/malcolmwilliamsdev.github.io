using CourseAdvising.Entities;
using CourseAdvising.Models;
using CourseAdvising.BusinessLogic;

// Create and run the course menu
CourseMenu menu = new CourseMenu();
CourseBusinessLogic.Initialize(menu);

if (!CourseBusinessLogic.IsDataLoaded())
{
    menu.AddOption(0, new MenuOption("Load Data", CourseBusinessLogic.LoadData, "Enter the name of the course data file: "));
}

menu.AddOption(1, new MenuOption("Print Course List", CourseBusinessLogic.PrintCourseList));
menu.AddOption(2, new MenuOption("Print Course", CourseBusinessLogic.PrintCourse, "Enter a course code: "));
menu.AddOption(9, new MenuOption("Exit", CourseBusinessLogic.Exit));

menu.Run();