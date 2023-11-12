#include <thread>
#include <string>
#include <algorithm>
#include <iostream>
#include <csignal>
#include <vector>
#include <atomic>
#include <mutex>

std::vector<std::thread> threads;
std::atomic<bool> stopFlag = false;
std::mutex coutMutex;

void PrintNumbersWithInterval()
{
    while (!stopFlag)
    {
        coutMutex.lock();
        std::cout << static_cast<float>(std::rand()) / RAND_MAX << '\n';
        coutMutex.unlock();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "PrintNumbersWithInterval thread finished.\n";
}

void PrintReversedString(std::string& string)
{
    std::reverse(string.begin(), string.end());
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << string << '\n';
}

void ReadInput()
{
    while (!stopFlag)
    {
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty())
        {
            PrintReversedString(input);
        }
    }
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout
        << "ReadInput thread finished.\n";
}

void SignalHandler(int signal)
{
    if (signal == SIGINT)
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Signal received. Exiting...\n";
        stopFlag = true;
    }
}

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::signal(SIGINT, SignalHandler);
    threads.emplace_back(PrintNumbersWithInterval);
    threads.emplace_back(ReadInput);

    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    std::cout << "Program finished\n";
    return 0;
}
