#include <signal.h>

#include "ConfigLexer.hpp"
#include "ConfigMaker.hpp"
#include "Log.hpp"
#include "Multiplexer.hpp"
#include "RootConfig.hpp"
#include "ServerConfig.hpp"
#include "ServerEventController.hpp"

int run(RootConfig &config) {
  Multiplexer &Multiplexer = Multiplexer::getInstance();

  std::vector<ServerConfig> &serverConfigs = config.getServerConfigs();
  std::map<int, ServerEventController *> servers;
  for (size_t i = 0; i < serverConfigs.size(); i++) {
    if (servers.find(serverConfigs[i].getPort()) == servers.end()) {
      int port = serverConfigs[i].getPort();
      ServerEventController *server =
          ServerEventController::addEventController(port);
      server->init();
      servers.insert(std::make_pair(serverConfigs[i].getPort(), server));
    }
  }

  std::map<int, ServerEventController *>::iterator iter;
  for (iter = servers.begin(); iter != servers.end(); iter++) {
    for (size_t i = 0; i < serverConfigs.size(); i++) {
      if (serverConfigs[i].getPort() == iter->first) {
        ServerEventController *server = iter->second;
        server->addServerConfig(&(serverConfigs[i]));
      }
    }
  }

  while (1) {
    std::vector<Multiplexer::Event> events = Multiplexer.wait(5);
    for (size_t i = 0; i < events.size(); i++) {
      events[i].controller->handleEvent(events[i]);
    }
    Multiplexer::getInstance().deleteAddedControllers();
  }
  return 0;
}

int main(int argc, char **argv) {
  std::string filepath;
  if (argc < 2) {
    filepath = "default.conf";
  } else if (argc == 2) {
    filepath = argv[1];
  } else {
    Log::error << "too many args" << NL;
    return 1;
  }
  std::ifstream configFile(filepath.c_str());
  if (!configFile.is_open()) {
    Log::error << "config file open error" << NL;
    return 1;
  }
  std::string configStr((std::istreambuf_iterator<char>(configFile)),
                        std::istreambuf_iterator<char>());
  configFile.close();
  try {
    Directive directive = ConfigLexer::run(configStr);
    RootConfig config = ConfigMaker::makeConfig(directive);
    config.printRootConfig();  // debug
    signal(SIGPIPE, SIG_IGN);
    run(config);
  } catch (const std::exception &e) {
    Log::error << "error: " << e.what() << NL;
    return 1;
  }
  return 0;
}
