#ifndef KVCACHE_TYPE_H
#define KVCACHE_TYPE_H
#include <string>

#include "domain/entity/Value.hpp"

namespace domain
{
    using MapType = std::unordered_map<std::string, Value>;
}

#endif //KVCACHE_TYPE_H
