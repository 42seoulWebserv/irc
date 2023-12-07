#include <signal.h>

#include "ConfigLexer.hpp"
#include "ConfigMaker.hpp"
#include "Log.hpp"
#include "Multiplexer.hpp"
#include "RootConfig.hpp"
#include "ServerConfig.hpp"
#include "ServerEventController.hpp"

#define BACKLOG 20

static void mainloop() {
  Multiplexer &multiplexer = Multiplexer::getInstance();

  while (1) {
    std::vector<Multiplexer::Event> events = multiplexer.wait(BACKLOG);
    for (size_t i = 0; i < events.size(); i++) {
      if (events[i].controller->isDeprecated() == false) {
        events[i].controller->handleEvent(events[i]);
      }
    }
    multiplexer.deleteAddedControllers();
  }
}

static void createServers(RootConfig &config) {
  config.printRootConfig();
  std::vector<ServerConfig> &serverConfigs = config.getServerConfigs();
  std::map<int, ServerEventController *> servers;
  for (size_t i = 0; i < serverConfigs.size(); i++) {
    if (servers.find(serverConfigs[i].getPort()) == servers.end()) {
      int port = serverConfigs[i].getPort();
      ServerEventController *server =
          ServerEventController::addEventController(port);
      if (server == NULL) {
        throw std::runtime_error("open server error");
      }
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
}

static std::string getConfigStr(int argc, char **argv) {
  std::string filepath;
  if (argc < 2) {
    filepath = "default.conf";
  } else if (argc == 2) {
    filepath = argv[1];
  } else {
    throw std::runtime_error("too many args");
  }
  std::ifstream configFile(filepath.c_str());
  if (!configFile.is_open()) {
    throw std::runtime_error("config file open error");
  }
  std::string configStr =
      std::string(std::istreambuf_iterator<char>(configFile),
                  std::istreambuf_iterator<char>());
  configFile.close();
  return configStr;
}

static void installSignals() {
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    throw std::runtime_error("fail to install signal");
  }
}

int main(int argc, char **argv) {
  try {
    std::string configStr = getConfigStr(argc, argv);
    Directive directive = ConfigLexer::makeDirective(configStr);
    RootConfig config = ConfigMaker::makeConfig(directive);
    installSignals();
    createServers(config);
    mainloop();
  } catch (const std::exception &e) {
    Log::error << "error: " << e.what() << NL;
    return 1;
  }
  return 0;
}
