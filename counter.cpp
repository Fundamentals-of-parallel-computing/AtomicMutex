#include "counter.h"

#include <iostream>
#include <ostream>
#include <thread>

Counter::Counter(int iteration) : iteration(iteration) {}

std::chrono::duration<double> Counter::measureTime(std::function<void()> func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return end - start;
}

Counter::Printer Counter::mutex() {
    int result = 0;
    return Printer(
        measureTime([this, &result]() {
            auto function = [this, &result]() {
                for (int i = 0; i < iteration; i++) {
                    mtx.lock();
                    result++;
                    mtx.unlock();
                }
            };
            std::thread t1(function);
            std::thread t2(function);

            t1.join();
            t2.join();
        }),
        result);
}

Counter::Printer Counter::atomic() {
    return Printer(

        measureTime([this]() {
            auto function = [this]() {
                for (int i = 0; i < iteration; i++) {
                    atomic_value.fetch_add(1, std::memory_order_relaxed);
                }
            };

            std::thread t1(function);
            std::thread t2(function);

            t1.join();
            t2.join();
        }),
        atomic_value.load());
}

Counter::Printer Counter::nosync() {
    int result = 0;
    return Printer(
        measureTime([&]() {
            auto function = [this, &result]() {
                for (int i = 0; i < iteration; i++) {
                    result++;
                }
            };

            std::thread t1(function);
            std::thread t2(function);

            t1.join();
            t2.join();
        }),
        result);
}

Counter::Printer::Printer(
    std::chrono::duration<double> completionTime, int result)
    : _result(result), completionTime(completionTime) {}

int Counter::Printer::get() {
    return _result;
}

void Counter::Printer::print() {
    auto seconds =
        std::chrono::duration_cast<std::chrono::seconds>(completionTime);
    auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(completionTime);
    auto microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(completionTime);
    auto nanoseconds =
        std::chrono::duration_cast<std::chrono::nanoseconds>(completionTime);

    std::cout << "Результат подсчета: " << _result << std::endl;
    std::cout << "Время выполнения: " << seconds.count() << " секунд, "
              << (milliseconds.count()) << " миллисекунд, "
              << (microseconds.count()) << " микросекунд, "
              << (nanoseconds.count()) << " наносекунд" << std::endl;
}
