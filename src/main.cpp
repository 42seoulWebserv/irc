#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <vector>
#include <fstream>
#include <iterator>

#include <set>
#include <iostream>

#include "RootConfig.hpp"
#include "ServerConfig.hpp"
#include "ServerEventController.hpp"
#include "ConfigLexer.hpp"
#include "ConfigMaker.hpp"

int run(RootConfig &config) {
  int kq = kqueue();

  std::vector<ServerConfig> &serverConfigs = config.getServerConfigs();
  std::map<int, ServerEventController *> serverEventControllers;
  for (size_t i = 0; i < serverConfigs.size(); i++) {
    if (serverEventControllers.find(serverConfigs[i].getPort()) == serverEventControllers.end()) {
      int port = serverConfigs[i].getPort();
      ServerEventController *serverEventController = new ServerEventController(kq, port);
      serverEventControllers.insert(std::make_pair(serverConfigs[i].getPort(), serverEventController));
    }
  }

  std::map<int, ServerEventController *>::iterator iter;
  for (iter = serverEventControllers.begin(); iter != serverEventControllers.end(); iter++) {
    for (size_t i = 0; i < serverConfigs.size(); i++) {
      if (serverConfigs[i].getPort() == iter->first) {
        ServerEventController *serverEventController = iter->second;
        serverEventController->addServerConfig(&(serverConfigs[i]));
      }
    }
  }

  while(1) {
    struct kevent eventList[5];
    int number = kevent(kq, 0, 0, eventList, 5, NULL);
    std::vector<EventController *> deleteList;
    for (int i = 0; i < number; i++) {
      EventController *connector = reinterpret_cast<EventController *>(eventList[i].udata);
      EventController::returnType type = connector->handleEvent(eventList[i]);
      if (type == EventController::SUCCESS || type == EventController::FAIL) {
        deleteList.push_back(connector);
      }
    }
    for (size_t i = 0; i < deleteList.size(); i++) {
      delete deleteList[i];
    }

    // sprintf(buff_snd, "%d : %s", strlen(buff_rcv), buff_rcv);
    // write(client_socket, "write", strlen(buff_snd) + 1); // +1: NULL까지 포함해서 전송
    // close(client_socket);
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
  std::string configStr((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
  configFile.close();

  Directive directive = ConfigLexer::run(configStr);
  RootConfig config = ConfigMaker::makeConfig(directive);
  config.printRootConfig();

  run(config);
  return 0;
}
