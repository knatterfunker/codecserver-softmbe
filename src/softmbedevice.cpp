#include "softmbedevice.hpp"
#include "softmbesession.hpp"
#include <iostream>

namespace softmbe {

std::vector<std::string> Device::getCodecs()
{
    return { "ambe" };
}

CodecServer::Session* Device::startSession(CodecServer::proto::Request* request) {
    softmbeSession* session = new softmbeSession();
    try
    {
        session->renegotiate(request->settings());
        return session;
    } catch (std::invalid_argument)
    {
        std::cerr << "invalid or unsupported channel index\n";
        return nullptr;
    }
    session->end();
    delete session;
    return nullptr;
}
}
