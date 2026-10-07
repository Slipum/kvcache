#ifndef KVCACHE_HASH_TABLE_H
#define KVCACHE_HASH_TABLE_H
#include <shared_mutex>
#include <unordered_map>

#include "domain/abstracts/IHashTable.hpp"

namespace infrastructure::hashtable {
    class HashTable : public domain::abstracts::IHashTable
    {
    private:
        domain::MapType table;
        mutable std::shared_mutex mutex_;

    public:
        HashTable() = default;

        std::pair<domain::MapType::const_iterator, domain::MapType::const_iterator> get_iter() const override {
            return {table.begin(), table.end()};
        }

        void insert(const std::string& key, std::optional<std::chrono::steady_clock::time_point> ttl, const std::string& value) override {
            table[key] = {value, ttl};
        }

        void insert(const std::string& key, const std::string& value) override {
            table[key] = {value, std::nullopt};
        }

        domain::Value& operator[](const std::string& key) override {
            return table[key];
        }

        std::optional<domain::Value> get(const std::string& key) const override {
            std::shared_lock lock(mutex_);
            auto it = table.find(key);
            if (it != table.end()) {
                if (it->second.ttl.has_value() && it->second.ttl.value() <= std::chrono::steady_clock::now())
                    {
                    return std::nullopt;
                }
                return it->second;
            }
            return std::nullopt;
        }

        bool remove(const std::string& key) override {
            return table.erase(key) > 0;
        }

        domain::MapType::iterator begin() override
        {
            return table.begin();
        }

        domain::MapType::iterator end() override
        {
            return table.end();
        }

        size_t remove_expired() override {
            std::unique_lock lock(mutex_);
            const auto now = std::chrono::steady_clock::now();
            size_t removed_count = 0;

            for (auto it = table.begin(); it != table.end(); ) {
                if (it->second.ttl.has_value() && it->second.ttl.value() <= now) {
                    it = table.erase(it);
                    ++removed_count;
                } else {
                    ++it;
                }
            }
            return removed_count;
        }
    };
};
#endif //KVCACHE_HASH_TABLE_H
