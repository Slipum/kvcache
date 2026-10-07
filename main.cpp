#include "infrastructure/di/DIContainer.hpp"
#include "infrastructure/periodic/PeriodicTask.hpp"

int main()
{
    infrastructure::DiContainer::registerSingleton<domain::abstracts::IHashTable, infrastructure::hashtable::HashTable>();
    infrastructure::DiContainer::registerSingleton<domain::abstracts::IServer, application::feature::HttpServer>();

    std::atomic<bool> stop{false};
    infrastructure::periodic::PeriodicTask periodicTask;

    std::thread worker(&infrastructure::periodic::PeriodicTask::task, &periodicTask, std::cref(stop));

    infrastructure::DiContainer::resolve<domain::abstracts::IServer>()->run();

    stop.store(true);
    periodicTask.stop();

    if (worker.joinable()) {
        worker.join();
    }

    return 0;
}
