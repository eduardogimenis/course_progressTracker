#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

void clearScreen() 
{
    std::cout << "\033[2J\033[1;1H"; 
}

struct course
{
    std::string c_name;
    int c_NumberOfModules;
    auto getName()
    {
        return c_name;
    }
    auto getNumbModules()
    {
        return c_NumberOfModules;
    }
};

class CertificationClass
{
public:
    auto getName()
    {
        return m_name;
    }
    int getNumberCourses()
    {
        return m_NumberOfCourses;
    }
    auto getCertfVector()
    {
        return m_CertificationVector;
    }
    bool getFirstTime()
    {
        return m_firstTime;
    }
    int getDoneMod()
    {
        return m_DoneModules;
    }
    void setName(std::string_view name)
    {
        m_name = name;
    }
    void setNumberCourses(int numberOfCourses)
    {
        m_NumberOfCourses = numberOfCourses;
    }
    void setFirstTime(bool hasIT)
    {
        m_firstTime = hasIT;
    }
    void setDoneMod(int doneMod)
    {
        m_DoneModules = doneMod;
    }
    void InitializeCertification()
    {
        std::string temp_name;
        int temp_numbModules;
        std::cout 
        << "Welcome to the certification program tracker.\n"
        << "------------/-----------/-------------/------------\n"
        << "Here you will put down your certification's:\n"
        << "number of courses, name of said courses, and how many modules there are in each.\n"
        << "If this is your first time using, or you have restarted your progress:\n"
        << "You will be prompted to enter number of modules done (in the certification as a whole).\n"
        << "And then everytime you start the program, you will add however many more modules you have completed,\nand you will be shown your progress.\n"
        << "\n\nAny key + Enter to continue...";
        std::cin >> temp_name;

        clearScreen();
        std::cout << "Let's start by inputting total number of courses in the certification: \n";
        std::cin >> m_NumberOfCourses;
        if (m_NumberOfCourses <= 0)
        {
            std::cout << "Please enter a valid amount.\n";
            m_NumberOfCourses = 0;
            return;
        }
        clearScreen();
        std::cout << "Now lets get information for each course: \n";
        for (int i = 0; i < m_NumberOfCourses; i++)
        {
            course temp_course;
            std::cout << "For course number " << i+1 << ", enter name: \n";
            std::cin.ignore(); // Clear any leftover newline from previous input
            std::getline(std::cin, temp_name); // Use getline to read the entire line
            std::cout << "Now enter number of modules: \n";
            std::cin >> temp_numbModules; 
            temp_course.c_name = temp_name;
            temp_course.c_NumberOfModules = temp_numbModules;
            m_CertificationVector.push_back(temp_course);
            m_TotalModules += temp_numbModules;
            clearScreen();
        }
        std::cout << "Now you should be all set for the next part!\n";
    }
    void addProgress()
    {
        if (m_firstTime)
        {
            std::cout << "Have you started on your certification?\n";
            std::cout << "If so, enter number of completed modules as a whole number\nIf not, enter 0\n";
            std::cin >> m_DoneModules;
        }
        else
        {
            int temp_current_done;
            std::cout << "    Total number of modules: " << m_TotalModules << ".\n";
            std::cout << "Number of current completed: " << m_DoneModules << ".\n";
            std::cout << "How many more have you completed since last time? ";
            std::cin >> temp_current_done;
            m_DoneModules += temp_current_done;
        }
    }
    void seeCertification() 
    {
        clearScreen();
        int runningTotalCompletedModules = 0; 
        std::string temp = "temp";

        for (int i = 0; i < m_CertificationVector.size(); i++) {
            runningTotalCompletedModules += m_CertificationVector.at(i).getNumbModules(); // Accumulate completed modules

            std::cout << "    course name: " << m_CertificationVector.at(i).getName() << "\n";
            std::cout << "numb of modules: " << m_CertificationVector.at(i).getNumbModules() << " ";

            if (runningTotalCompletedModules <= m_DoneModules) { 
                std::cout << " done!    :)"; 
            }
            else
            {
                std::cout << " not done :(";
            }

            std::cout << "\n\n"; // Add a newline for better readability
        }

        double percentage{0};
        percentage = (static_cast<double>(m_DoneModules) / m_TotalModules) * static_cast<double>(100);
        std::cout << "    total modules: " << m_TotalModules << ".\n";
        std::cout << "completed modules: " << m_DoneModules << ".\n";
        std::cout << "  percentage done: " << percentage << "%.\n";
        std::cout << "\n\nAny number + Enter to continue...";
        std::getline(std::cin, temp);
    }
    void saveData(const std::string& filename) 
    {
        std::ofstream outfile(filename); 
        if (!outfile.is_open()) {
            // Handle file open error
            return;
        }

        // Write certification name, number of courses, total modules, completed modules, and first-time flag
        outfile << m_name << '\n' 
                << m_NumberOfCourses << '\n'
                << m_TotalModules << '\n'
                << m_DoneModules << '\n'
                << m_firstTime << '\n';

        // Write each course's name and number of modules
        for (const auto& course : m_CertificationVector) {
            outfile << course.c_name << '\n'
                    << course.c_NumberOfModules << '\n';
        }

        outfile.close();
    }
    bool loadData(const std::string& filename) 
    {
        std::ifstream infile(filename);
        if (!infile.is_open()) {
            // Handle file open error (maybe return false to indicate failure)
            return false;
        }

        // Read certification name, number of courses, total modules, completed modules, and first-time flag
        std::getline(infile, m_name);
        infile >> m_NumberOfCourses 
            >> m_TotalModules 
            >> m_DoneModules 
            >> m_firstTime;
        infile.ignore(); // Consume newline after reading the boolean

        // Clear existing course data
        m_CertificationVector.clear();

        // Read each course's name and number of modules
        for (int i = 0; i < m_NumberOfCourses; ++i) {
            course tempCourse;
            std::getline(infile, tempCourse.c_name);
            infile >> tempCourse.c_NumberOfModules;
            infile.ignore(); // Consume newline

            m_CertificationVector.push_back(tempCourse);
        }

        infile.close();
        return true; // Indicate successful load
    }
    void restartData() 
    {
        // Reset all member variables to their default/initial states
        m_name = "";
        m_NumberOfCourses = 0;
        m_DoneModules = 0;
        m_TotalModules = 0;
        m_firstTime = true;
        m_CertificationVector.clear();

        // You might also want to clear the saved data file here if appropriate
    }
private:
    std::string m_name;
    int m_NumberOfCourses {0};
    int m_DoneModules {0};
    int m_TotalModules {0};
    bool m_firstTime = true;
    std::vector<course> m_CertificationVector;
};


int main()
{
    int choice;
    CertificationClass currentCertification;
    currentCertification.loadData("certification_data.csv");
    if (currentCertification.getFirstTime())
    {
        currentCertification.InitializeCertification();
        currentCertification.addProgress();
        currentCertification.setFirstTime(false);
    }
    while(1)
    {
        std::cout
        << "\n---------/Menu/----------\n"
        << "1. See progress.\n"
        << "2. Add progress.\n"
        << "3. Save & Exit.\n"
        << "4. Delete saved data.\n";
        std::cin >> choice;
        if (choice < 1 || choice > 4)
        {
            std::cout << "Enter one of the options.\n";
            break;
        }   
        switch(choice)
        {
            case 1:
                currentCertification.seeCertification();
                break;
            case 2:
                currentCertification.addProgress();
                break;
            case 3:
                currentCertification.saveData("certification_data.csv");
                exit(0);
            case 4:
                currentCertification.restartData();
                currentCertification.InitializeCertification();
                currentCertification.addProgress();
                currentCertification.setFirstTime(false);
            default:
                std::cout << "choose between the options.\n";
                break;
        }
    }
}