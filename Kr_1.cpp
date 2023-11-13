#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <string>

class Bank
{
public:
    Bank(const int startTotal)
    {
        this->total = startTotal;
        std::thread thread([this]()
            { StartUserThread(); });
    }

    void HandleOperations(const std::vector<std::vector<double>>& operations)
    {
        std::vector<std::thread> threads;
        threadsDone.clear();
        threadsDone = std::vector<std::atomic<bool>>(operations.size());
        for (int i = 0; i < operations.size(); ++i)
        {
            std::vector<double> operation = operations[i];
            threads.emplace_back([this, operation, i]()
                { HandleOperation(operation, i); });
        }

        threads.emplace_back([this]()
            { HandleUnsuccessOperations(); });
        for (std::thread& thread : threads)
        {
            thread.join();
        }
        for (auto val : unsuccessOperations)
        {
            std::cout << val.first << ' ';
        }
    }
    static const std::chrono::milliseconds operationDelay;
    static const std::chrono::milliseconds sleepTime;
    static const int maxTries = 4;

private:
    void StartUserThread()
    {
        while (true)
        {
            std::string input;
            std::getline(std::cin, input);
            if (input.empty())
            {
                continue;
            }
            if (input == "end")
            {
                return;
            }
            if (input == "total")
            {
                PrintTotal();
            }
        }
    }

    std::mutex mutex;

    bool TryDoOperation(const double val)
    {
        std::atomic<bool> isSuccess = true;
        std::this_thread::sleep_for(operationDelay);
        if (val >= 0)
        {
            ChangeTotal(val);
        }
        else
        {
            if (total + val < 0)
            {
                isSuccess = false;
            }
            else
            {
                ChangeTotal(val);
            }
        }
        if (isSuccess)
        {
            std::this_thread::sleep_for(sleepTime);
        }
        return isSuccess;
    }

    void HandleOperation(const std::vector<double>& operation, const int threadIndex)
    {
        for (double val : operation)
        {
            mutex.lock();
            if (!TryDoOperation(val))
            {
                unsuccessOperations.push_back({ val, 0 });
            }
            mutex.unlock();
        }
        threadsDone[threadIndex] = true;
    }

    void HandleUnsuccessOperations()
    {
        int isDone = false;
        while (!isDone)
        {
            mutex.lock();
            std::vector<std::pair<double, int>> newUnsuccessOperations;
            for (auto val : unsuccessOperations)
            {
                if (val.second++ > maxTries)
                {
                    continue;
                }
                if (!TryDoOperation(val.first))
                {
                    newUnsuccessOperations.push_back(val);
                }
            }
            unsuccessOperations = newUnsuccessOperations;
            std::this_thread::sleep_for(sleepTime);
            bool flag = true;
            for (int i = 0; i < threadsDone.size(); ++i)
            {
                if (!threadsDone[i])
                {
                    flag = false;
                    break;
                }
            }
            if (flag && unsuccessOperations.empty())
            {
                isDone = true;
            }
            mutex.unlock();
        }
    }

    void ChangeTotal(const double amount)
    {

        const double oldTotal = total;

        total += amount;

        std::cout << "Old total: " << oldTotal << ". Operation:" << (amount >= 0 ? "deposit" : "withdraw") << " " << std::abs(amount) << ". New total: " << total << "\n";
    }

    void PrintTotal()
    {
        std::cout << "Total: " << total << "\n";
    }
    std::vector<std::pair<double, int>> unsuccessOperations;
    std::vector<std::atomic<bool>> threadsDone;
    double total = 0;
};

const std::chrono::milliseconds Bank::operationDelay(50);
const std::chrono::milliseconds Bank::sleepTime(2300);

int main(int argc, char const* argv[])
{
    std::vector<std::vector<double>> v = { {-90,-70, 200}, {100, -20, 10}, {-40, -30, 55} };
    Bank bank(100);
    bank.HandleOperations(v);
    std::cout << "Scene 1 end"<<std::endl;

    std::vector<std::vector<double>> v1 = { {100, -20, 10}, {-40, -30, 55} };
    Bank bank1(100);
    bank1.HandleOperations(v1);
    std::cout << "Scene 2 end" << std::endl;
    return 0;
}
