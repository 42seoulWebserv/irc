#include "Log.hpp"

Log::Log(std::ostream &os, const char *color, int level)
    : os_(os), color_(color), level_(level) {}

Log Log::debug(std::cout, LOG_WHITE, LOG_DEBUG);
Log Log::info(std::cout, LOG_GREEN, LOG_INFO);
Log Log::warn(std::cout, LOG_YELLOW, LOG_WARN);
Log Log::error(std::cerr, LOG_RED, LOG_ERROR);
