#define NOMINMAX

#include <iostream>
#include <thread>
#include <mutex> 
#include <Windows.h> // may want to remove this!
#include <chrono>
#include <cstdlib>
#include <string>
#include <functional>
#include <vector>
#include <limits>

// the classes exist purely for organization purposes
class Beeps
{
public:
    // basic beep
    static void basicBeep()
    {
        Beep(50, 300);  // 50 Hz, 300ms
    }
    // upward tone
    static void upwardBeep()
    {
        Beep(300, 500);  // 300 Hz, 500ms
        Beep(600, 500);  // 300 Hz, 500ms
        Beep(1000, 500);  // 300 Hz, 500ms
        Beep(1500, 500);  // 300 Hz, 500ms
    }
    // warning sound beep
    static void warningBeep()
    {
        Beep(400, 1000);  // 50 Hz, 500ms
    }
    //heartbeat-like beep
    static void heartbeatBeep()
    {
        Beep(600, 300);  // 50 Hz, 500ms
        Beep(600, 600);  // 50 Hz, 500ms
        Beep(600, 900);  // 50 Hz, 500ms
    }
};

// the classes exist purely for organization purposes
class ThreadManager {
public:
    static void countdown(std::function<void()> chosenBeep)
    {


        for(int i = 10; i >= 0; i--)
        {
            safe_print("Timer 1 : " + std::to_string(i) + "seconds");
            std::this_thread::sleep_for(std::chrono::milliseconds(1800));
            chosenBeep();
        }
    }
    static void safe_print(const std::string& message)
    {   
    std::lock_guard<std::mutex> lock(std::mutex);
    std::cout << message << std::endl;
    }
};

class ThreadCreator
{
private: 
    std::string threadName;
    std::function<void()> chosenBeep;
    int threadIndex = 0;
public:
    ThreadCreator(std::string threadName, std::function<void()> chosenBeep) : threadName(threadName), chosenBeep(chosenBeep){}


    void startThread()
    {
        // Correct lambda capture for chosenBeep and threadName
        std::thread threadObj([this]() {
            std::cout << "Thread '" << threadName << "' is starting." << std::endl;
            ThreadManager::countdown(chosenBeep);   // Countdown task with beep function
        });

        // Detach the thread so it runs concurrently
        threadObj.detach();  // Detach it so it runs independently
    }
};

void Menu()
{
    int choice = 0;
    int beepChoice = 0;
    std::function<void()> chosenBeep;  // Store the selected beep function
    std::vector<std::function<void()>> threadFunctions;  // Store the countdown functions to execute later
    std::vector<std::string> threadNames;  // Store the thread names (optional)
    int threadIndex = 0; // would preferably want a way to make this string but add a value to it through int that gets converted back
    std::string inputname;
    do {
        std::cout << "Press 1 to create a thread" << std::endl;
        std::cout << "Press 5 if you wish to exit the menu and begin the beeping" << std::endl;

         while (true) {
            std::cin >> choice;

            // Check if the input is valid
            if (std::cin.fail()) {
                std::cin.clear();  // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
                std::cout << "Invalid input, please enter a number: " << std::endl;
            } else {
                break;  // Break the loop if valid input
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch(choice)
        {
        case 1:
            std::cout << "what should be the name for this thread?" << std::endl;
            std::cin >> inputname;

            std::cout << "Choose a beep type" << std::endl;
            std::cout << "1. Standard beep" << std::endl;
            std::cout << "2. Upward Beep" << std::endl;
            std::cout << "3. Warning Beep" << std::endl;
            std::cout << "4. heartbeatBeep" << std::endl;

            std::cin >> beepChoice;

            switch(beepChoice)
            {
                case 1:
                    chosenBeep = &Beeps::basicBeep;
                    break;
                case 2:
                    chosenBeep = &Beeps::upwardBeep;
                    break;
                case 3:
                    chosenBeep = &Beeps::warningBeep;
                    break;
                case 4:
                    chosenBeep = &Beeps::heartbeatBeep;
                    break;
                default: 
                    std::cout << "invalid choice defaulting to standard Beep" << std::endl;
                    chosenBeep = &Beeps::basicBeep;
                    break;
            }
            std::cout << "Creating a new thread" << std::endl;
            threadNames.push_back(inputname);
            threadFunctions.push_back([chosenBeep]() {
                ThreadManager::countdown(chosenBeep);
            });
            break;

        case 5:
            std::cout << "exitting menu" << std::endl;
            break;
        default:
            std::cout << "invalid choice, please try again." << std::endl;
            break;
        }
    } while(choice != 5);

    std::cout << "Activating all threads..." << std::endl;
    std::vector<std::thread> threads;
    for (size_t i = 0; i < threadFunctions.size(); ++i)
    {
        threads.push_back(std::thread(threadFunctions[i]));  // Start each thread
    }

    // Wait for all threads to finish
    for (auto& t : threads)
    {
        if (t.joinable())  // Check if the thread is joinable
        {
            t.join();  // Wait for the thread to finish
        }
    }

    std::cout << "All threads finished!" << std::endl;
}

int ThreadActivator()
{ /*
    std::cout << "what should be the name for this thread?" << std::endl;
    std::string inputname;
    std::cin >> inputname;
    ThreadCreator thread1(inputname); */

    Menu();

    std::cout << "Main Thread Finishes" << std::endl;

    return 0;
}

int main()
{
    ThreadActivator();
    return 0;
}