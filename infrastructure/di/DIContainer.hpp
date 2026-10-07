#ifndef KVCACHE_DICONTAINER_H
#define KVCACHE_DICONTAINER_H
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <stdexcept>

#include "application/feature/protocol/Http.hpp"
#include "domain/abstracts/IServer.hpp"
#include "infrastructure/hashtable/HashTable.hpp"


namespace infrastructure {
    class DiContainer {
    private:
        // Храним не просто объекты, а "фабрики" (функции, которые возвращают объект).
        // Это позволит нам гибко управлять временем жизни (создавать каждый раз новый или возвращать синглтон).
        inline static std::unordered_map<std::type_index, std::function<std::shared_ptr<void>()>> factories;

    public:
        // 1. Регистрация интерфейса и конкретной реализации (Transient - каждый раз новый)
        template <typename Interface, typename Implementation, typename... Args>
        static void registerTransient(Args&&... args) {
            factories[std::type_index(typeid(Interface))] = [=]() {
                return std::static_pointer_cast<void>(std::make_shared<Implementation>(args...));
            };
        }

        // 2. Регистрация как Singleton (один объект на всю программу)
        template <typename Interface, typename Implementation, typename... Args>
        static void registerSingleton(Args&&... args) {
            // Создаем объект один раз при регистрации
            auto instance = std::make_shared<Implementation>(args...);
            factories[std::type_index(typeid(Interface))] = [instance]() {
                return std::static_pointer_cast<void>(instance);
            };
        }

        // 3. Получение (разрешение) зависимости
        template <typename Interface>
        static std::shared_ptr<Interface> resolve() {
            auto it = factories.find(std::type_index(typeid(Interface)));
            if (it == factories.end()) {
                throw std::runtime_error("Not registered");
            }

            // Вызываем фабрику и безопасно кастим void обратно в нужный интерфейс
            return std::static_pointer_cast<Interface>(it->second());
        }
    };

    // namespace di
    // {
    //     inline std::shared_ptr<domain::abstracts::IHashTable> HashTable() {
    //         static auto instance = std::make_shared<hashtable::HashTable>();
    //         return instance;
    //     }
    //
    //     inline std::unique_ptr<domain::abstracts::IServer> Server() {
    //         return std::make_unique<application::feature::HttpServer>();
    //     }
    // }
};

#endif //KVCACHE_DICONTAINER_H
