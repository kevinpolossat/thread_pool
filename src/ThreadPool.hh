//
// Created by kevin on 28/04/17.
//

#ifndef THREAD_POOL_THREADPOOL_HH
#define THREAD_POOL_THREADPOOL_HH

#include <mutex>
#include <condition_variable>
#include <vector>
#include <deque>
#include <thread>
#include <algorithm>
#include <future>

class ThreadPool {
public:
    ThreadPool(std::size_t nThread = std::thread::hardware_concurrency()): _run(true) {
        if (!nThread) {
            throw std::range_error("nThread must be strictly positive");
        }
        for (std::size_t i = 0; i < nThread; ++i) {
            _workers.emplace_back([this]() {
                std::function<void()> f;
                for (;;) {
                    {
                        std::unique_lock<std::mutex> u_lock(_mutex);
                        _cond_var.wait(u_lock, [this](){ return !_run || !_deque.empty(); });
                        if (!_run && _deque.empty()) {
                            return ;
                        }
                        f = std::move(_deque.back());
                        _deque.pop_back();
                    }
                    f();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> u_lock(_mutex);
            _run = false;
        }
        _cond_var.notify_all();
        std::for_each(_workers.begin(), _workers.end(), [](auto & x) {
            x.join();
        });
    }

    ThreadPool(ThreadPool const &)              = delete;
    ThreadPool operator=(ThreadPool const &)    = delete;

    template<typename T, typename... Args>
    auto submit(T && f, Args &&... args) -> std::future<decltype(f(args...))> {
        auto task = std::make_shared<std::packaged_task<decltype(f(args...))()>>(
                std::bind(std::forward<T>(f), std::forward<Args>(args)...)
        );
        {
            std::unique_lock<std::mutex> u_lock(_mutex);
            _deque.emplace_front([task]() {
                (*task)();
            });
        }
        _cond_var.notify_one();
        return task->get_future();
    }

private:
    std::vector<std::thread>            _workers;
    bool                                _run;
    std::deque<std::function<void()>>   _deque;
    std::mutex                          _mutex;
    std::condition_variable             _cond_var;
};

#endif //THREAD_POOL_THREADPOOL_HH
