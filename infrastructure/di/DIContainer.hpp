#ifndef KVCACHE_DICONTAINER_H
#define KVCACHE_DICONTAINER_H
#include <memory>

#include "application/feature/protocol/Http.hpp"
#include "domain/abstracts/IServer.hpp"
#include "infrastructure/hashtable/HashTable.hpp"

namespace infrastructure::di {
    inline std::shared_ptr<domain::abstracts::IHashTable> HashTable() {
        static auto instance = std::make_shared<hashtable::HashTable>();
        return instance;
    }

    inline std::unique_ptr<domain::abstracts::IServer> Server() {
        return std::make_unique<application::feature::HttpServer>();
    }
};

#endif //KVCACHE_DICONTAINER_H
