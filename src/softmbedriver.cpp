#include "softmbedriver.hpp"
#include "registry.hpp"

using namespace softmbe;

std::string Driver::getIdentifier()
{
    return "softmbe";
}

Device* Driver::buildFromConfiguration(std::map<std::string, std::string> config)
{
    if (config.find("uvquality") == config.end())
    {
        std::cerr << "Using default uvquality of 3 for softmbe" << std::endl;
        return new Device(3);
    }

    try
    {
        int uvquality = stoi(config["uvquality"]);
        std::cerr << "Using uvquality of " << uvquality << " for softmbe" << std::endl;
        return new Device(uvquality);
    } catch (std::invalid_argument)
    {
        std::cerr << "Unable to process uvquality. Using default of 3 for softmbe" << std::endl;
        return new Device(3);
    }
}

static int registration = CodecServer::Registry::registerDriver(new Driver());
