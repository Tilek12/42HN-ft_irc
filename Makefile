# Color codes
GREEN	= \033[0;32m
VIOLET	= \033[0;35m
RESET	= \033[0m

# Compiler
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17
# LEAKS = -L../../../LeakSanitizer -llsan -lc++ -Wno-gnu-include-next -I ../../../LeakSanitizer

# Directories
OBJ_DIR = obj
SRC_DIR = src

# Variables
NAME = ircserv
TEST_CHANNEL = test_channel
TEST_TILEK = test_tilek
SRC =	$(SRC_DIR)/Server.cpp \
		$(SRC_DIR)/serverCmdHandler.cpp \
		$(SRC_DIR)/parse_cmds.cpp \
		$(SRC_DIR)/Client.cpp \
		$(SRC_DIR)/client_c_h.cpp \
		$(SRC_DIR)/commands.cpp \
		$(SRC_DIR)/Channel.cpp \
		$(SRC_DIR)/ChannelCmds.cpp \
		$(SRC_DIR)/channelHelperFcts.cpp \
		$(SRC_DIR)/channelCmdsUtils.cpp \
		$(SRC_DIR)/channelCmdsUserRole.cpp \
		$(SRC_DIR)/channelCmdHandler.cpp \
		$(SRC_DIR)/channelCmdsUtilsJoin.cpp \
		$(SRC_DIR)/channelCmdsUtilsMode.cpp

OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Compile the executable
all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ)
# @$(CXX) $(CXXFLAGS) $(OBJ) $(SRC_DIR)/main.cpp /Users/llacsivy/LeakSanitizer/liblsan.dylib -o $(NAME) -g
	@$(CXX) $(CXXFLAGS) $(OBJ) $(SRC_DIR)/main.cpp -o $(NAME)
#> /dev/null 2>&1
	@echo "$(GREEN)object files created.$(RESET)"
	@echo "$(GREEN)$(NAME) created. $(RESET)"

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Create OBJ_DIR
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Clean up object files
clean:
	@rm -rf $(OBJ_DIR) > /dev/null 2>&1
	@echo "$(VIOLET)object files deleted.$(RESET)"

# Clean up object files and executable file
fclean: clean
	@rm -f $(NAME) > /dev/null 2>&1
	@echo "$(VIOLET)$(NAME) deleted.$(RESET)"

# Recompile the project
re: fclean all

.PHONY: all clean fclean re test
