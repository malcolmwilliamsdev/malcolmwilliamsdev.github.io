namespace CourseAdvising.Models;

public class MenuOption
{
    public string Description { get; private set; }
    public Action<string> Action { get; private set; }
    public string? Prompt { get; private set; }

    public MenuOption(string description, Action<string> action, string? prompt = null)
    {
        Description = description;
        Action = action;
        Prompt = prompt;
    }

    public void Execute(string input) => Action(input);
}