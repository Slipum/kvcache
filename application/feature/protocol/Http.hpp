#ifndef KVCACHE_HTTP_H
#define KVCACHE_HTTP_H
#include "domain/abstracts/IServer.hpp"
#include <httplib.h>

namespace application::feature {
    class HttpServer : public domain::abstracts::IServer {
    private:
        httplib::Server srv;
        void init_routes() {
            this->get_all();
            this->get();
            this->create();
            this->del();
        }

    public:
        HttpServer() = default;

        void run() override;

        void get() override;
        void create() override;
        void del() override;
        void get_all() override;
    };
}

#endif //KVCACHE_HTTP_H
