#include "softmbedevice.hpp"
#include "softmbesession.hpp"
#include <iostream>

namespace softmbe {

Device::Device(int q) : uvquality(q) {}

std::vector<std::string> Device::getCodecs()
{
    return { "ambe" };
}

CodecServer::Session* Device::startSession(CodecServer::proto::Request* request) {
    softmbeSession* session = new softmbeSession(uvquality);
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
