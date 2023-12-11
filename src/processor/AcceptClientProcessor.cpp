#include "AcceptClientProcessor.hpp"

#include "ClientEventController.hpp"
#include "Log.hpp"

AcceptClientProcessor::AcceptClientProcessor(IServer& server)
    : server_(server) {}

ProcessResult AcceptClientProcessor::process() {
  int clientSocket = server_.acceptClient();
  if (clientSocket == -1) {
    return ProcessResult().setError(true);
  }
  if (ClientEventController::addEventController(
          clientSocket, server_.getServerConfigs()) == NULL) {
    return ProcessResult().setError(true);
  }
  Log::info << clientSocket << ": client accepted" << NL;
  return ProcessResult();
}
