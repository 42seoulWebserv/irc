NAME = webserv
.DEFAULT_GOAL = all

CXXFLAGS = -std=c++98 -Wall -Wextra -MMD -MP -g3
SRC = \
	parser/lexer/Parser.cpp \
	parser/lexer/ParseResult.cpp \
	parser/lexer/Pattern.cpp \
	parser/lexer/PatternLetters.cpp \
	parser/lexer/PatternWord.cpp \
	parser/lexer/PatternOptional.cpp \
	parser/lexer/PatternSequence.cpp \
	ConfigLexer.cpp \
	ConfigChecker.cpp \
	ConfigMaker.cpp \
	RootConfig.cpp \
	ServerConfig.cpp \
	LocationConfig.cpp \
	Directive.cpp \
	main.cpp
.PHONY: all clean fclean re test

all: $(NAME)

DEP = $(SRC:.cpp=.d)
-include $(DEP)

$(NAME): $(SRC:.cpp=.o)
	$(CXX) $(CXXFLAGS) $(OUTPUT_OPTION) $(SRC:.cpp=.o)

$(CLASS:.cpp=.o): %.o: %.cpp %.hpp

clean:
	find . -type f \( -name '*.o' -or -name '*.d' \) -print -delete

fclean: clean
	rm -f $(NAME)

re: fclean
	make all
