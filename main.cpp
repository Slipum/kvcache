#include "infrastructure/di/DIContainer.hpp"
#include "infrastructure/periodic/PeriodicTask.hpp"

int main()
{
    std::atomic<bool> stop{false};
    infrastructure::periodic::PeriodicTask periodicTask;
    std::thread worker(&infrastructure::periodic::PeriodicTask::task, &periodicTask, std::ref(stop));

    infrastructure::di::Server()->run();

    stop.store(true);
    if (worker.joinable()) {
        worker.join();
    }

    return 0;
}
