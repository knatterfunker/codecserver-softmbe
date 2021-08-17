#include "softmbedriver.hpp"
#include "registry.hpp"

using namespace softmbe;

std::string Driver::getIdentifier()
{
    return "softmbe";
}

Device* Driver::buildFromConfiguration(std::map<std::string, std::string> config)
{
    return new Device();
}

static int registration = CodecServer::Registry::registerDriver(new Driver());
