#include "../include/thread_pool.hpp"
#include <atomic>
#include <queue>

using namespace Thesisuis;

Pool::Pool(std::size_t num_workers) : stop(false) {
    for (std::size_t i = 0; i < num_workers; i++) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                std::unique_lock<std::mutex> lock(queue_mutex);
                condition.wait(lock, [this] { return stop || !tasks.empty(); });
                if (stop || tasks.empty()) return;
                task = std::move(tasks.front());
                tasks.pop();
                }
                task();
            }
        });
    }
}

template <class F>
void Pool::enqueue(F&& f) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace(std::forward<F>(f));
    }
    condition.notify_one();
}

Pool::~Pool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker: workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}
