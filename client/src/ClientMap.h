#pragma once
#include <memory>
#include "Map.h"
#include "TSVector.h"
#include "NetInclude.h"
#include "Params.h"

namespace sim
{
    class ClientMap : public Map
    {
    public:
        ClientMap(WinConsole &, params::MapConfig &, TSVector<Entity> *, const std::unique_ptr<net::Connection<params::MessageType>> *);
        virtual ~ClientMap();
    
        virtual void run(size_t update_freq = 1) override;
    
    private:
        //connection to asio
        net::Message<params::MessageType> 
        const std::unique_ptr<net::Connection<params::MessageType>> *mptr_connection;

    };
}