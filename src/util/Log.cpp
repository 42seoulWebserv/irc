#include "Log.hpp"

Log::Log(const char *color, int level) : color_(color), level_(level) {}

Log Log::debug(LOG_WHITE, LOG_DEBUG);
Log Log::info(LOG_GREEN, LOG_INFO);
Log Log::warn(LOG_YELLOW, LOG_WARN);
Log Log::error(LOG_RED, LOG_ERROR);
