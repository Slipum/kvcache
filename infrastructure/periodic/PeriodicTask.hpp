#ifndef KVCACHE_PERIODICTASK_HPP
#define KVCACHE_PERIODICTASK_HPP
#include <thread>

#include "infrastructure/config/Env.hpp"
#include "infrastructure/di/DIContainer.hpp"
#include "infrastructure/hashtable/HashTable.hpp"

namespace infrastructure::periodic {
    class PeriodicTask {
    private:
        std::condition_variable cv_;
        std::mutex cv_dummy_mutex_;

        static void delete_ttl();

    public:
        void task(const std::atomic<bool>& stop) {
            while (!stop.load()) {
                delete_ttl();

                std::unique_lock<std::mutex> lock(cv_dummy_mutex_);
                cv_.wait_for(lock, std::chrono::seconds(config::Env::PERIODIC_SEC), [&stop]() {
                    return stop.load();
                });
            }
        }

        void stop() {
            cv_.notify_all();
        }
    };
}

#endif //KVCACHE_PERIODICTASK_HPP
