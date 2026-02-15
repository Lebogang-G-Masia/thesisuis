#ifndef THESISUIS_THREAD_POOL
#define THESISUIS_THREAD_POOL

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace Thesisuis {
    class Pool {
        private:
            std::vector<std::thread> workers;
            std::queue<std::function<void()>> tasks;
            std::mutex queue_mutex;
            std::condition_variable condition;
            bool stop;
        public:
            Pool(std::size_t); 
            
            template <class F>
            void enqueue(F&& f) {
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    tasks.emplace(std::forward<F>(f));
                }
                condition.notify_one();
            } 
            ~Pool();

    };
}

#endif // THESISUIS_THREAD_POOL
