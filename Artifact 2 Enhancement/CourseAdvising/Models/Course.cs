namespace CourseAdvising.Models
{
    public class Course
    {
        public string Code { get; set; }
        public string Name { get; set; }
        public string FullName => $"{Code} - {Name}";
        public List<string> PrerequisiteCodes { get; set; }

        public Course(string code, string name)
        {
            Code = code;
            Name = name;
            PrerequisiteCodes = [];
        }

        public Course(string code, string name, List<string> prerequisiteCodes)
        {
            Code = code;
            Name = name;
            PrerequisiteCodes = prerequisiteCodes;
        }

        public override string ToString()
        {
            var str = $"{FullName}";
            if (PrerequisiteCodes.Count > 0)
            {
                str += $"\n\tPrerequisites: {string.Join(", ", PrerequisiteCodes)}";
            }
            return str;
        }
    }
}
