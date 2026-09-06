#ifndef KVCACHE_PERIODICTASK_HPP
#define KVCACHE_PERIODICTASK_HPP
#include <thread>

#include "infrastructure/config/Env.hpp"
#include "infrastructure/di/DIContainer.hpp"
#include "infrastructure/hashtable/HashTable.hpp"

namespace infrastructure::periodic
{
    class PeriodicTask {
    private:
        void delete_ttl();

    public:
        void task(std::atomic<bool>& stop) {
            while (!stop.load()) {
                delete_ttl();
                std::this_thread::sleep_for(std::chrono::seconds(config::Env::PERIODIC_SEC));
            }
        }
    };
}

#endif //KVCACHE_PERIODICTASK_HPP
