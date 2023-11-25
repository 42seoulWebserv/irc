#include <sys/socket.h>
#include <sys/types.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

#include "ConfigLexer.hpp"
#include "ConfigMaker.hpp"
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
      ServerEventController *server = new ServerEventController(port);
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
    std::set<EventController *> deleteList;
    for (size_t i = 0; i < events.size(); i++) {
      EventController::returnType type =
          events[i].controller->handleEvent(events[i]);
      if (type == EventController::SUCCESS || type == EventController::FAIL) {
        deleteList.insert(events[i].controller);
      }
    }
    std::set<EventController *>::const_iterator it;
    for (it = deleteList.begin(); it != deleteList.end(); it++) {
      delete *it;
    }

    // sprintf(buff_snd, "%d : %s", strlen(buff_rcv), buff_rcv);
    // write(client_socket, "write", strlen(buff_snd) + 1); // +1: NULL까지
    // 포함해서 전송 close(client_socket);
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
    std::cout << "Error: too many args" << std::endl;
    return 1;
  }
  std::ifstream configFile(filepath.c_str());
  if (!configFile.is_open()) {
    std::cout << "Error: config file open error" << std::endl;
    return 1;
  }
  std::string configStr((std::istreambuf_iterator<char>(configFile)),
                        std::istreambuf_iterator<char>());
  configFile.close();
  try {
    Directive directive = ConfigLexer::run(configStr);
    RootConfig config = ConfigMaker::makeConfig(directive);
    config.printRootConfig();  // debug
    run(config);
  } catch (...) {
    return 1;
  }
  return 0;
}
