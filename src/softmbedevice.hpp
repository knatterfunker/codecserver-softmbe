#pragma once

#include "device.hpp"
#include "proto/request.pb.h"

namespace softmbe
{
    class Device: public CodecServer::Device
    {
        public:
            std::vector<std::string> getCodecs() override;
            CodecServer::Session* startSession(CodecServer::proto::Request* request) override;
    };
}
