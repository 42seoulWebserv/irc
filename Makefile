NAME := webserv

.DEFAULT_GOAL := all

INC_DIR := \
	-I./src \
	-I./src/config \
	-I./src/parser/lexer \
	-I./src/config/configChecker \
	-I./src/config/configLexer \
	-I./src/config/configMaker \
	-I./src/processor \
	-I./src/util \
	-I./src/multiplexer \

SRC_DIR := ./src
BUILD_DIR := ./build

CXX := c++
CXXFLAGS := -std=c++98 -Wall -Wextra -MMD -MP -g3 -Weffc++ $(INC_DIR)
ifdef DEBUG
CXXFLAGS += -fsanitize=address
endif

# ===============================================

MAIN_DIR := ./src/
MAIN_NAME := \
	main.cpp \
	EventController.cpp \
	ServerEventController.cpp \
	ClientEventController.cpp \
	Request.cpp \
	Response.cpp \

CONFIG_DIR := ./src/config/
CONFIG_NAME := \
	Directive.cpp \
	LocationConfig.cpp \
	RootConfig.cpp \
	ServerConfig.cpp \

CONFIGCHECKER_DIR := ./src/config/configChecker/
CONFIGCHECKER_NAME := \
	ConfigChecker.cpp \

CONFIGMAKER_DIR := ./src/config/configMaker/
CONFIGMAKER_NAME := \
	ConfigMaker.cpp \

CONFIGLEXER_DIR := ./src/config/configLexer/
CONFIGLEXER_NAME := \
	ConfigLexer.cpp \

PASER_LEXER_DIR := ./src/parser/lexer/
PASER_LEXER_NAME := \
	Parser.cpp \
	ParseResult.cpp \
	Pattern.cpp \
	PatternLetters.cpp \
	PatternOptional.cpp \
	PatternSequence.cpp \
	PatternWord.cpp \

PROCESSOR_DIR := ./src/processor/
PROCESSOR_NAME := \
	ProcessResult.cpp \
	StartProcessor.cpp \
	ParseRequestProcessor.cpp \
	CgiProcessor.cpp \
	MethodDeleteProcessor.cpp \
	MethodGetProcessor.cpp \
	MethodPostProcessor.cpp \
	ErrorPageProcessor.cpp \
	SelectMethodProcessor.cpp \
	ProvideFileProcessor.cpp \
	WaitProcessor.cpp \
	RedirectionProcessor.cpp \

UTIL_DIR := ./src/util/
UTIL_NAME := \
	FilePath.cpp \
	DataStream.cpp \
	StringBuffer.cpp \

ifeq ($(shell uname), Linux)
MULTIPLEXER_DIR	= ./src/multiplexer/linux/
else
MULTIPLEXER_DIR	= ./src/multiplexer/mac/
endif
MULTIPLEXER_NAME := \
	Multiplexer.cpp \

SRCS := \
	$(addprefix $(MAIN_DIR), $(MAIN_NAME)) \
	$(addprefix $(CONFIG_DIR), $(CONFIG_NAME)) \
	$(addprefix $(CONFIGCHECKER_DIR), $(CONFIGCHECKER_NAME)) \
	$(addprefix $(CONFIGLEXER_DIR), $(CONFIGLEXER_NAME)) \
	$(addprefix $(CONFIGMAKER_DIR), $(CONFIGMAKER_NAME)) \
	$(addprefix $(PASER_LEXER_DIR), $(PASER_LEXER_NAME)) \
	$(addprefix $(PROCESSOR_DIR), $(PROCESSOR_NAME)) \
	$(addprefix $(UTIL_DIR), $(UTIL_NAME)) \
	$(addprefix $(MULTIPLEXER_DIR), $(MULTIPLEXER_NAME)) \

SRCS_DIR := \
	$(MAIN_DIR) \
	$(CONFIG_DIR) \
	$(CONFIGCHECKER_DIR) \
	$(CONFIGLEXER_DIR) \
	$(CONFIGMAKER_DIR) \
	$(PASER_LEXER_DIR) \
	$(PROCESSOR_DIR) \
	$(UTIL_DIR) \
	$(MULTIPLEXER_DIR) \

vpath %.cpp $(SRCS_DIR)

%.o	: %.cpp
	$(CXX) $(CXXFLAGS) -MD -c -o $@ $^

OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)
OBJS_FILES := $(addprefix $(BUILD_DIR)/, $(notdir $(OBJS)))

-include $(DEPS)

# ===============================================

all: $(BUILD_DIR) $(NAME)

$(NAME): $(OBJS_FILES)
	$(CXX) $(CXXFLAGS) $(OBJS_FILES) -o $(NAME)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) -r $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

debug:
	$(MAKE) fclean
	$(MAKE) -j DEBUG=1

.PHONY: all clean fclean re debug
