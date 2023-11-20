#include "MethodPostProcessor.hpp"

#include <fstream>
#include <iostream>

#include "FilePath.hpp"

MethodPostProcessor::MethodPostProcessor(IClient &client) : client_(client) {
  std::cout << "in POST Method" << std::endl;
}

static void writePostFile(FilePath &filepath, std::string &content) {
  std::ofstream outputFile(filepath + "post.txt");
  if (!outputFile.is_open()) {
    std::cerr << "Error: POST: std::ofstream" << std::endl;
  }
  outputFile << content;
  outputFile.close();
}

ProcessResult MethodPostProcessor::process() {
  std::cout << "in POST process" << std::endl;
  FilePath filepath = "." + client_.getLocationConfig()->getRootPath();
  filepath.append(client_.getRequest().getUri());
  filepath = filepath.toDirectoryPath();
  std::cout << "filepath: " << filepath << std::endl;
  std::string content = client_.getRequest().getBody();
  if (!filepath.isExist()) {  // error code
    std::cout << "directory exists: " << filepath << std::endl;
  } else {
    writePostFile(filepath, content);
  }
  FileWriteEventController::addEventController(filepath, content, this);
  return ProcessResult();
}

void MethodPostProcessor::onEvent(const FileWriteEventController::Event &p) {
  this->onEvent(p);
}
