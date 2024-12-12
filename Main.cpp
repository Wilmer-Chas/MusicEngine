#include <iostream>
#include <thread>
#include <mutex> 
#include <Windows.h> // may want to remove this!
#include <chrono>
#include <cstdlib>
#include <string>


// the classes exist purely for organization purposes
class Beeps
{
public:
    // upward tone
    static void produceBeep1()
    {
        Beep(300, 500);  // 300 Hz, 500ms
        Beep(600, 500);  // 300 Hz, 500ms
        Beep(1000, 500);  // 300 Hz, 500ms
        Beep(1500, 500);  // 300 Hz, 500ms
    }
    // basic beep
    static void produceBeep2()
    {
        Beep(50, 300);  // 50 Hz, 300ms
    }
    // warning sound beep
    static void produceBeep3()
    {
        Beep(400, 1000);  // 50 Hz, 500ms
    }
    //heartbeat-like beep
    static void produceBeep4()
    {
        Beep(600, 300);  // 50 Hz, 500ms
        Beep(600, 600);  // 50 Hz, 500ms
        Beep(600, 900);  // 50 Hz, 500ms
    }
};

// the classes exist purely for organization purposes
class ThreadManager {
public:
    static void countdown()
    {
        for(int i = 10; i >= 0; i--)
        {
            safe_print("Timer 1 : " + std::to_string(i) + "seconds");
            std::this_thread::sleep_for(std::chrono::milliseconds(1800));
            Beeps::produceBeep4();
        }
    }
    static void countup()
    {
        for(int i = 0; i <= 10; i++)
        {
            std::cout << "Timer 2 : " << i << "seconds" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // how does - impact it??
            Beeps::produceBeep1();
        }
    }
    static void safe_print(const std::string& message)
    {   
    std::lock_guard<std::mutex> lock(std::mutex);
    std::cout << message << std::endl;
    }
};

int ThreadActivator()
{
    std::thread t([](){
    ThreadManager::countdown();   // Countdown task
    });

    std::thread t1([](){
    ThreadManager::countup();
    });

    t.join(); // Wait for the first thread to finish
    t1.join(); // Wait for the second thread to finish


    std::cout << "Main Thread Finishes" << std::endl;

    return 0;
}

int main()
{
    ThreadActivator();
    return 0;
}