
# Color codes
GREEN	= \033[0;32m
VIOLET	= \033[0;35m
RESET	= \033[0m

# Compiler
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

# Directories
OBJ_DIR = obj
SRC_DIR = src
TEST_DIR = tests

# Variables
NAME = ircserv
TEST_CHANNEL = test_channel
TEST_TILEK = test_tilek
SRC =	$(SRC_DIR)/Server.cpp \
		$(SRC_DIR)/Channel.cpp \
		$(SRC_DIR)/Command.cpp
		
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Compile the executable
all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) $(SRC_DIR)/main.cpp -o $(NAME) > /dev/null 2>&1
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
re: fclean all test

# Run tests
test: $(OBJ) $(TEST_CHANNEL) $(TEST_TILEK)
	@./$(TEST_CHANNEL)
	@./$(TEST_TILEK)
	@echo "$(GREEN)Tests ran successfully.$(RESET)"

# Compile the test executable
$(TEST_CHANNEL): $(OBJ) $(TEST_DIR)/test_channel.cpp
	@$(CXX) $(CXXFLAGS) $(OBJ) $(TEST_DIR)/test_channel.cpp -o $(TEST_CHANNEL) 

$(TEST_TILEK): $(TEST_DIR)/test_tilek.cpp
	@$(CXX) $(CXXFLAGS) $(OBJ) $(TEST_DIR)/test_tilek.cpp -o $(TEST_TILEK) 

.PHONY: all clean fclean re test
