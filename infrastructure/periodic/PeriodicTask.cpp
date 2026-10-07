#include "PeriodicTask.hpp"

using namespace infrastructure::periodic;

void PeriodicTask::delete_ttl() {
    auto table = DiContainer::resolve<domain::abstracts::IHashTable>();
    size_t removed = table->remove_expired();

    if (removed > 0) {
        std::cout << "[TTL Cleaner] Removed " << removed << " expired keys.\n";
    }
}
