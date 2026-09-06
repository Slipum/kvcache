#ifndef KVCACHE_HASH_TABLE_H
#define KVCACHE_HASH_TABLE_H
#include <unordered_map>

#include "domain/abstracts/IHashTable.hpp"

namespace infrastructure::hashtable {
    class HashTable : public domain::abstracts::IHashTable
    {
    private:
        domain::MapType table;
    public:
        HashTable() = default;

        std::pair<domain::MapType::const_iterator, domain::MapType::const_iterator> get_iter() const override {
            return {table.begin(), table.end()};
        }

        void insert(const std::string& key, std::optional<std::chrono::system_clock::time_point> ttl, const std::string& value) override {
            table[key] = {value, ttl};
        }

        void insert(const std::string& key, const std::string& value) override {
            table[key] = {value, std::nullopt};
        }

        domain::Value& operator[](const std::string& key) override {
            return table[key];
        }

        std::optional<domain::Value> get(const std::string& key) const override {
            auto it = table.find(key);
            if (it != table.end()) {
                return it->second;
            }
            return std::nullopt;
        }

        bool remove(const std::string& key) override {
            return table.erase(key) > 0;
        }
    };
};
#endif //KVCACHE_HASH_TABLE_H
