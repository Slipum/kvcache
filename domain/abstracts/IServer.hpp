#ifndef KVCACHE_ISERVER_H
#define KVCACHE_ISERVER_H
#include <string>

namespace domain::abstracts {
    class IServer {
    public:
        virtual void run() = 0;

        virtual void get() = 0;
        virtual void create() = 0;
        virtual void del() = 0;
        virtual void get_all() = 0;
    };
}



#endif //KVCACHE_ISERVER_H
