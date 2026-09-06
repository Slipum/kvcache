#include "PeriodicTask.hpp"

using namespace infrastructure::periodic;

void PeriodicTask::delete_ttl()
{
    auto [begin, end] = di::HashTable()->get_iter();
    for (auto it = begin; it != end; ++it) {
        if (it->second.ttl <= std::chrono::system_clock::now())
        {
            std::cout << "Deleting ttl with key: " << it->first << "\n";
            di::HashTable()->remove(it->first);
        }
    }
}
