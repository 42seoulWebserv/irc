NAME = webserv

.DEFAULT_GOAL = all

INC_DIR = ./
SRC_DIR = ./
BUILD_DIR = ./build

RM = rm -rf
CXXFLAGS = -std=c++98 -Wall -Wextra -MMD -MP -g3 -I $(INC_DIR)

# ===============================================

LEXER_DIR = ./parser/lexer/
LEXER_NAME = \
	Parser.cpp \
	ParseResult.cpp \
	Pattern.cpp \
	PatternLetters.cpp \
	PatternWord.cpp \
	PatternOptional.cpp \
	PatternSequence.cpp

CHECKER_DIR = ./
CHECKER_NAME = \
	ConfigLexer.cpp \
	ConfigChecker.cpp \
	ConfigMaker.cpp \
	RootConfig.cpp \
	ServerConfig.cpp \
	LocationConfig.cpp \
	Directive.cpp \
	main.cpp

SRCS = \
	$(addprefix $(LEXER_DIR), $(LEXER_NAME)) \
	$(addprefix $(CHECKER_DIR), $(CHECKER_NAME))

vpath %.cpp $(LEXER_DIR) $(CHECKER_DIR)

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)
OBJS_FILES = $(addprefix $(BUILD_DIR)/, $(notdir $(OBJS)))

-include $(DEPS)

# ===============================================

all: $(NAME)

$(NAME): $(OBJS_FILES)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS_FILES)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o : %.cpp $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
