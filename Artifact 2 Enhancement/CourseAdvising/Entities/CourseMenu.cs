using CourseAdvising.Models;

namespace CourseAdvising.Entities;

public class CourseMenu
{
    private readonly string _title = "ABCU Course Advising";
    private Dictionary<int, MenuOption> options { get; set; }
    private bool quit;

    public CourseMenu()
    {
        options = [];
    }

    // Add a menu option with a unique integer key
    public void AddOption(int key, MenuOption option)
    {
        options[key] = option;
    }

    // Main loop to display the menu and handle user input
    public void Run()
    {
        quit = false;
        while (!quit)
        {
            Display();
            ProcessInput();
            Console.WriteLine();
        }
    }

    // Exit the menu loop
    public void Exit()
    {
        quit = true;
    }

    // Display the menu options
    private void Display()
    {
        Console.WriteLine($"===={_title}====\n");
        Console.WriteLine("Menu:");
        foreach (var (key, option) in options.OrderBy(o => o.Key))
        {
            Console.WriteLine($"  {key}. {option.Description}");
        }
        Console.Write("\nSelect an option number: ");
    }

    // Interpret user input and execute the corresponding menu option
    private void ProcessInput()
    {
        var input = Console.ReadLine(); // Read user input
        if (int.TryParse(input, out int optionKey) && options.ContainsKey(optionKey))
        {
            // Execute the selected option with an optional argument
            var option = options[optionKey];
            string argument = string.Empty;
            if (!string.IsNullOrEmpty(option.Prompt))
            {
                Console.Write(option.Prompt);
                argument = Console.ReadLine() ?? string.Empty;
            }
            option.Execute(argument);
        }
        else
        {
            Console.WriteLine("Invalid option. Please try again.");
        }
    }
}