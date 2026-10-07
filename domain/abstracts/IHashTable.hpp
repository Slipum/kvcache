#ifndef KVCACHE_IHASH_TABLE_H
#define KVCACHE_IHASH_TABLE_H
#include <string>
#include <optional>

#include "type.hpp"

namespace domain::abstracts {
    class IHashTable {
    public:
        virtual ~IHashTable() = default;

        virtual void insert(const std::string& key, std::optional<std::chrono::steady_clock::time_point> ttl, const std::string& value) = 0;
        virtual void insert(const std::string& key, const std::string& value) = 0;
        virtual Value& operator[](const std::string& key) = 0;

        virtual std::optional<Value> get(const std::string& key) const = 0;
        virtual std::pair<MapType::const_iterator, MapType::const_iterator> get_iter() const = 0;

        virtual bool remove(const std::string& key) = 0;

        virtual MapType::iterator begin() = 0;
        virtual MapType::iterator end() = 0;

        virtual size_t remove_expired() = 0;
    };
};
#endif //KVCACHE_IHASH_TABLE_H
