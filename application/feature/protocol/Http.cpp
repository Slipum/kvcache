#include "Http.hpp"

#include <json.hpp>

#include "infrastructure/config/Env.hpp"
#include "infrastructure/di/DIContainer.hpp"

using namespace application::feature;

void HttpServer::run() {
    this->init_routes();
    std::cout << "HTTP server started on http://localhost:" << infrastructure::config::Env::PORT << std::endl;
    srv.listen("0.0.0.0", infrastructure::config::Env::PORT);
}

void HttpServer::get() {
    srv.Get("/:kid", [](const httplib::Request& req, httplib::Response& res) {
        try {
            if (req.path_params.find("kid") == req.path_params.end()) {
                res.status = 400;
                res.set_content("Bad Request: missing 'kid' parameter", "text/plain; charset=utf-8");
                return;
            }
            const std::string kid_str = req.path_params.at("kid");
            auto table = infrastructure::DiContainer::resolve<domain::abstracts::IHashTable>()->get(kid_str);
            if (!table) {
                res.status = 404;
                res.set_content("Not found", "text/plain; charset=utf-8");
                return;
            }

            nlohmann::json response_json;
            response_json["value"] = table->value;

            if (table->ttl.has_value()) {
                auto duration = table->ttl.value() - std::chrono::steady_clock::now();
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
                response_json["ttl"] = ms > 0 ? ms : 0;
            } else {
                response_json["ttl"] = nullptr;
            }

            res.set_content(response_json.dump(), "application/json; charset=utf-8");
        }
        catch (const std::exception& e) {
            res.status = 500;
            res.set_content(e.what(), "text/plain; charset=utf-8");
        }
        catch (...) {
            res.status = 500;
            res.set_content("Unknown Error", "text/plain; charset=utf-8");
        }
    });
}

void HttpServer::get_all() {
    srv.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        try {
            const auto& table_ptr = infrastructure::DiContainer::resolve<domain::abstracts::IHashTable>();
            if (!table_ptr) {
                res.status = 500;
                res.set_content("Internal server error: cache unavailable", "text/plain; charset=utf-8");
                return;
            }

            nlohmann::json result = nlohmann::json::object();
            for (const auto& [key, item] : *table_ptr) {
                result[key] = {
                    {"value", item.value},
                    {"ttl", item.ttl ? nlohmann::json(item.ttl->time_since_epoch().count()) : nullptr}
                };
            }

            res.set_content(result.dump(), "application/json; charset=utf-8");
        }
        catch (const std::exception& e) {
            res.status = 500;
            res.set_content(e.what(), "text/plain; charset=utf-8");
        }
    });
}

void HttpServer::del()
{
    srv.Delete("/:kid", [](const httplib::Request& req, httplib::Response& res) {
        std::string kid_str = req.path_params.at("kid");
        try {
            if (!infrastructure::DiContainer::resolve<domain::abstracts::IHashTable>()->remove(kid_str))
            {
                res.status = 404;
                res.set_content("Not found", "text/plain; charset=utf-8");
                return;
            }
            res.status = 204;
        } catch (...) {
            res.status = 500;
            res.set_content("Unknown Error", "text/plain; charset=utf-8");
        }
    });
}

void HttpServer::create()
{
    srv.Post("/", [](const httplib::Request& req, httplib::Response& res)
    {
        try {
            auto body = nlohmann::json::parse(req.body);
            auto kv = body.get<domain::KeyValue>();
            if (infrastructure::DiContainer::resolve<domain::abstracts::IHashTable>()->get(kv.key)) {
                res.status = 409;
                nlohmann::json error_json = {{"error", "Key '" + kv.key + "' already exists"}};
                res.set_content(error_json.dump(), "application/json; charset=utf-8");
                return;
            }
            if (kv.value.ttl)
                infrastructure::DiContainer::resolve<domain::abstracts::IHashTable>()->insert(kv.key, std::chrono::steady_clock::now() + kv.value.ttl->time_since_epoch(), kv.value.value);
            else
                infrastructure::DiContainer::resolve<domain::abstracts::IHashTable>()->insert(kv.key, kv.value.value);

            res.set_content(body.dump(), "application/json; charset=utf-8");
            res.status = 201;
        }
        catch (const std::exception& e) {
            std::string error_msg = e.what();

            size_t bracket_pos = error_msg.find("]");
            if (bracket_pos != std::string::npos) error_msg = error_msg.substr(bracket_pos + 2);

            nlohmann::json error_json = {{"error", error_msg}};

            res.status = 400;
            res.set_content(error_json.dump(), "application/json; charset=utf-8");
        }
        catch (...) {
            res.status = 500;
            res.set_content("Unknown Error", "text/plain; charset=utf-8");
        }
    });
}