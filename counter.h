#ifndef COUNTER_H
#define COUNTER_H

#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>

class Counter {
public:
    explicit Counter(int iteration);

private:
    class Printer {
    public:
        explicit Printer(
            std::chrono::duration<double> completionTime, int result);
        int get();
        void print();

    private:
        int _result;
        std::chrono::duration<double> completionTime;
    };
    std::chrono::duration<double> measureTime(std::function<void()> func);

public:
    Printer mutex();
    Printer atomic();
    Printer nosync();

private:
    std::mutex mtx;
    std::atomic<int> atomic_value = 0;
    int iteration;
};

#endif  // COUNTER_H
