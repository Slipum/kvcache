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
        std::string kid_str = req.path_params.at("kid");
        try
        {
            nlohmann::json value = nlohmann::json::object();
            if (auto table = infrastructure::di::HashTable()->get(kid_str))
            {
                value[kid_str]["value"] = table->value;
                if (table->ttl)
                    value[kid_str]["ttl"] = std::format("{}", table->ttl->time_since_epoch());
                else
                    value[kid_str]["ttl"] = nullptr;
                res.set_content(value.dump(), "application/json; charset=utf-8");
            } else
            {
                throw std::exception();
            }
        } catch (const std::exception&) {
            res.status = 404;
            res.set_content("Not found", "text/plain; charset=utf-8");
        } catch (...) {
            res.status = 500;
            res.set_content("Unknown Error", "text/plain; charset=utf-8");
        }
    });
}

void HttpServer::get_all() {
    srv.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        try {
            nlohmann::json result = nlohmann::json::object();
            auto [begin, end] = infrastructure::di::HashTable()->get_iter();
            for (auto it = begin; it != end; ++it) {
                result[it->first]["value"] = it->second.value;
                if (it->second.ttl)
                    result[it->first]["ttl"] = it->second.ttl->time_since_epoch().count();
                else
                    result[it->first]["ttl"] = nullptr;
            }
            res.set_content(
                result.dump(),
                "application/json; charset=utf-8"
            );
        }
        catch (const std::exception& e) {
            res.status = 500;
            res.set_content(
                "Internal server error",
                "text/plain; charset=utf-8"
            );
        }
    });
}

void HttpServer::del()
{
    srv.Delete("/:kid", [](const httplib::Request& req, httplib::Response& res) {
        std::string kid_str = req.path_params.at("kid");
        try {
            if (!infrastructure::di::HashTable()->remove(kid_str))
                throw std::exception();
            res.status = 204;
        } catch (const std::exception&) {
            res.status = 404;
            res.set_content("Not found", "text/plain; charset=utf-8");
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
            if (infrastructure::di::HashTable()->get(kv.key)) {
                res.status = 409;
                nlohmann::json error_json = {{"error", "Key '" + kv.key + "' already exists"}};
                res.set_content(error_json.dump(), "application/json; charset=utf-8");
                return;
            }
            if (kv.value.ttl)
            {
                infrastructure::di::HashTable()->insert(kv.key, std::chrono::system_clock::now() + kv.value.ttl->time_since_epoch(), kv.value.value);
            } else
            {
                infrastructure::di::HashTable()->insert(kv.key, std::nullopt, kv.value.value);
            }
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