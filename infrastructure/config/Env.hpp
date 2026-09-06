#ifndef KVCACHE_ENV_H
#define KVCACHE_ENV_H

#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>

namespace infrastructure::config {
    class Env {
    private:
        static void ensure_loaded() {
            static bool loaded = false;
            if (loaded) return;

            std::string env_path = std::string(PROJECT_ROOT_DIR) + "/.env";
            std::ifstream file(env_path);
            if (!file.is_open()) {
                loaded = true;
                return;
            }

            std::string line;
            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#') continue;

                std::size_t delimiter_pos = line.find('=');
                if (delimiter_pos == std::string::npos) continue;

                std::string key = line.substr(0, delimiter_pos);
                std::string value = line.substr(delimiter_pos + 1);

                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                ::setenv(key.c_str(), value.c_str(), 1);
            }
            loaded = true;
        }

        static const char* get(const char* path) {
            ensure_loaded();

            const char* env = std::getenv(path);
            if (env == nullptr) {
                throw std::runtime_error("Environment variable not found: " + std::string(path));
            }
            return env;
        }

        static int get_int(const char* path) {
            try {
                return std::stoi(get(path));
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("Environment variable " + std::string(path) + " is not a valid integer");
            } catch (const std::out_of_range&) {
                throw std::runtime_error("Environment variable " + std::string(path) + " value is out of integer range");
            }
        }

    public:
        
        inline static int PORT = get_int("PORT");
        inline static int PERIODIC_SEC = get_int("PERIODIC_SEC");
    };
}

#endif //KVCACHE_ENV_H
