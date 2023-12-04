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
	-I./src/controller \

SRC_DIR := src
BUILD_DIR := build

CXX := c++
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror -MMD -MP -g3 $(INC_DIR)
ifdef DEBUG
CXXFLAGS += -fsanitize=address -DLOG_LEVEL=4
endif

# ===============================================

MAIN_DIR := $(SRC_DIR)/
MAIN_NAME := \
	main.cpp \
	Request.cpp \
	Response.cpp \

CONFIG_DIR := $(SRC_DIR)/config/
CONFIG_NAME := \
	Directive.cpp \
	LocationConfig.cpp \
	RootConfig.cpp \
	ServerConfig.cpp \

CONFIGCHECKER_DIR := $(SRC_DIR)/config/configChecker/
CONFIGCHECKER_NAME := \
	ConfigChecker.cpp \

CONFIGMAKER_DIR := $(SRC_DIR)/config/configMaker/
CONFIGMAKER_NAME := \
	ConfigMaker.cpp \

CONFIGLEXER_DIR := $(SRC_DIR)/config/configLexer/
CONFIGLEXER_NAME := \
	ConfigLexer.cpp \

CONTROLLER_DIR := $(SRC_DIR)/controller/
CONTROLLER_NAME := \
	EventController.cpp \
	ServerEventController.cpp \
	ClientEventController.cpp \
	CgiEventController.cpp \

PASER_LEXER_DIR := $(SRC_DIR)/parser/lexer/
PASER_LEXER_NAME := \
	Parser.cpp \
	ParseResult.cpp \
	Pattern.cpp \
	PatternLetters.cpp \
	PatternOptional.cpp \
	PatternSequence.cpp \
	PatternWord.cpp \

PROCESSOR_DIR := $(SRC_DIR)/processor/
PROCESSOR_NAME := \
	AcceptClientProcessor.cpp \
	ProcessResult.cpp \
	StartProcessor.cpp \
	ParseRequestBodyProcessor.cpp \
	ParseRequestChunkProcessor.cpp \
	ParseRequestHeadProcessor.cpp \
	CgiProcessor.cpp \
	CgiInProcessor.cpp \
	CgiOutProcessor.cpp \
	MethodDeleteProcessor.cpp \
	MethodGetProcessor.cpp \
	MethodPostProcessor.cpp \
	MethodPutProcessor.cpp \
	ErrorPageProcessor.cpp \
	SelectMethodProcessor.cpp \
	ProvideFileProcessor.cpp \
	WaitProcessor.cpp \
	RedirectionProcessor.cpp \

UTIL_DIR := $(SRC_DIR)/util/
UTIL_NAME := \
	Log.cpp \
	String.cpp \
	FilePath.cpp \
	DataStream.cpp \
	StringBuffer.cpp \

ifeq ($(shell uname), Linux)
MULTIPLEXER_DIR	= $(SRC_DIR)/multiplexer/linux/
else
MULTIPLEXER_DIR	= $(SRC_DIR)/multiplexer/mac/
endif
MULTIPLEXER_NAME := \
	Multiplexer.cpp \

SRCS := \
	$(addprefix $(MAIN_DIR), $(MAIN_NAME)) \
	$(addprefix $(CONFIG_DIR), $(CONFIG_NAME)) \
	$(addprefix $(CONFIGCHECKER_DIR), $(CONFIGCHECKER_NAME)) \
	$(addprefix $(CONFIGLEXER_DIR), $(CONFIGLEXER_NAME)) \
	$(addprefix $(CONFIGMAKER_DIR), $(CONFIGMAKER_NAME)) \
	$(addprefix $(CONTROLLER_DIR), $(CONTROLLER_NAME)) \
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
	$(CONTROLLER_DIR) \
	$(PASER_LEXER_DIR) \
	$(PROCESSOR_DIR) \
	$(UTIL_DIR) \
	$(MULTIPLEXER_DIR) \

vpath %.cpp $(SRCS_DIR)

OBJS := $(SRCS:.cpp=.o)
OBJS_FILES := $(patsubst $(SRC_DIR)%,$(BUILD_DIR)%,$(OBJS))

DEPS := $(SRCS:.cpp=.d)
DEPS_FILES := $(patsubst $(SRC_DIR)%,$(BUILD_DIR)%,$(DEPS))

-include $(DEPS_FILES)

# ===============================================

all: $(NAME)

$(NAME): $(OBJS_FILES)
	$(CXX) $(CXXFLAGS) $(OBJS_FILES) -o $(NAME)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
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
	$(MAKE) -j DEBUG=1 LOG_LEVEL=1

.PHONY: all clean fclean re debug
