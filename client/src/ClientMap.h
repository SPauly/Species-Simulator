#pragma once
#include <memory>
#include "TSVector.h"
#include "NetMsg.h"
#include "NetConnection.h"
#include "Params.h"
#include "Map.h"

namespace sim
{
    class ClientMap : public Map
    {
    public:
        ClientMap(WinConsole &, params::MapConfig &, TSVector<Entity> *, std::unique_ptr<net::Connection<params::MessageType>> *);
        virtual ~ClientMap();
    
        virtual void run(size_t update_freq = 1) override;
    
    private:
        //connection to asio
        net::Message<params::MessageType> m_entity_message;
        std::unique_ptr<net::Connection<params::MessageType>> *mptr_connection;
    };
}