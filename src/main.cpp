#include <assert.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

#include "ConfigLexer.hpp"
#include "ConfigMaker.hpp"
#include "FilePath.hpp"
#include "RootConfig.hpp"
#include "ServerConfig.hpp"
#include "ServerEventController.hpp"

void testFilePath() {
  {  // constructor test
    FilePath root("/");
    assert(root.getPath() == "/");
    root.appendPath("test");
    assert(root.getPath() == "/test");

    FilePath current2("");
    assert(current2.getPath() == "");
    current2.appendPath("test");
    assert(current2.getPath() == "test");
  }
  {  // appendPath test
    FilePath path("/test1");
    assert(path.getPath() == "/test1");
    path.appendPath("test2");
    assert(path.getPath() == "/test1/test2");
    path.appendPath("test3");
    assert(path.getPath() == "/test1/test2/test3");
    path.appendPath("/test4");
    assert(path.getPath() == "/test1/test2/test3/test4");
    path.appendPath("/test5/");
    assert(path.getPath() == "/test1/test2/test3/test4/test5/");
  }
  {  // isExist test
    FilePath makefile("Makefile");
    assert(makefile.isExist());

    FilePath longFilePath("we-are-project-name-is-irc-but-it-is-not-irc");
    assert(!longFilePath.isExist());
  }
  {  // isDirectory, isFile test
    FilePath srcDir("src");
    assert(srcDir.isDirectory());
    assert(!srcDir.isFile());

    FilePath srcDirWithSlash("src/");
    assert(srcDirWithSlash.isDirectory());
    assert(!srcDir.isFile());

    FilePath makefile("Makefile");
    assert(!makefile.isDirectory());
    assert(makefile.isFile());

    FilePath makefileWithSlash("Makefile/");
    assert(!makefileWithSlash.isDirectory());
    assert(!makefileWithSlash.isFile());
  }
  {  // toDirectoryPath, toFilePath test
    FilePath path2("src");
    assert(path2.toFilePath() == "src");
    assert(path2.toDirectoryPath() == "src/");

    FilePath path1("src/");
    assert(path1.toFilePath() == "src");
    assert(path1.toDirectoryPath() == "src/");

    FilePath longPath1("is/not/irc/");
    assert(longPath1.toFilePath() == "is/not/irc");
    assert(longPath1.toDirectoryPath() == "is/not/irc/");

    FilePath longPath2("is/not/irc");
    assert(longPath2.toFilePath() == "is/not/irc");
    assert(longPath2.toDirectoryPath() == "is/not/irc/");
  }
  {  // getExtension, getFileName, getDirectory test
    FilePath path1("src/main.cpp");
    assert(FilePath::getExtension(path1) == "cpp");
    assert(FilePath::getFileName(path1) == "main.cpp");
    assert(FilePath::getDirectory(path1) == "src");

    FilePath path2("src/");
    assert(FilePath::getExtension(path2) == "");
    assert(FilePath::getFileName(path2) == "");
    assert(FilePath::getDirectory(path2) == "src");

    FilePath path3("src");
    assert(FilePath::getExtension(path3) == "");
    assert(FilePath::getFileName(path3) == "src");
    assert(FilePath::getDirectory(path3) == "");
  }
}

int run(RootConfig &config) {
  int kq = kqueue();

  std::vector<ServerConfig> &serverConfigs = config.getServerConfigs();
  std::map<int, ServerEventController *> serverEventControllers;
  for (size_t i = 0; i < serverConfigs.size(); i++) {
    if (serverEventControllers.find(serverConfigs[i].getPort()) ==
        serverEventControllers.end()) {
      int port = serverConfigs[i].getPort();
      ServerEventController *serverEventController =
          new ServerEventController(kq, port);
      serverEventControllers.insert(
          std::make_pair(serverConfigs[i].getPort(), serverEventController));
    }
  }

  std::map<int, ServerEventController *>::iterator iter;
  for (iter = serverEventControllers.begin();
       iter != serverEventControllers.end(); iter++) {
    for (size_t i = 0; i < serverConfigs.size(); i++) {
      if (serverConfigs[i].getPort() == iter->first) {
        ServerEventController *serverEventController = iter->second;
        serverEventController->addServerConfig(&(serverConfigs[i]));
      }
    }
  }

  while (1) {
    struct kevent eventList[5];
    int number = kevent(kq, 0, 0, eventList, 5, NULL);
    std::vector<EventController *> deleteList;
    for (int i = 0; i < number; i++) {
      EventController *connector =
          reinterpret_cast<EventController *>(eventList[i].udata);
      EventController::returnType type = connector->handleEvent(eventList[i]);
      if (type == EventController::SUCCESS || type == EventController::FAIL) {
        deleteList.push_back(connector);
      }
    }
    for (size_t i = 0; i < deleteList.size(); i++) {
      delete deleteList[i];
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

  Directive directive = ConfigLexer::run(configStr);
  RootConfig config = ConfigMaker::makeConfig(directive);
  // config.printRootConfig();

  // run(config);
  testFilePath();

  return 0;
}
