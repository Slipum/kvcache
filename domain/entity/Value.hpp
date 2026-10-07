#ifndef KVCACHE_VALUE_HPP
#define KVCACHE_VALUE_HPP
#include <string>

#include "json.hpp"

namespace nlohmann {
    template <>
    struct adl_serializer<std::chrono::seconds> {
        static void to_json(json& j, const std::chrono::seconds& s) {
            j = s.count();
        }

        static void from_json(const json& j, std::chrono::seconds& s) {
            if (j.is_number()) {
                s = std::chrono::seconds(j.get<int64_t>());
            } else {
                throw std::runtime_error("Expected a number for chrono::seconds");
            }
        }
    };

    template <>
    struct adl_serializer<std::chrono::steady_clock::time_point> {
        static void to_json(json& j, const std::chrono::steady_clock::time_point& tp) {
            j = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
        }

        static void from_json(const json& j, std::chrono::steady_clock::time_point& tp) {
            if (j.is_number()) {
                auto duration = std::chrono::seconds(j.get<int64_t>());
                tp = std::chrono::steady_clock::time_point(duration);
            } else {
                throw std::runtime_error("Expected a number for steady_clock::time_point");
            }
        }
    };
}

namespace domain
{
    struct Value
    {
        std::string value;
        std::optional<std::chrono::steady_clock::time_point> ttl = std::nullopt;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Value, value, ttl)
    };

    struct KeyValue {
        std::string key;
        domain::Value value;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(KeyValue, key, value);
}

#endif //KVCACHE_VALUE_HPP
