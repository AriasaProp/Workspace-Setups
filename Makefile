OBJ_DIR := bin/obj
EXEC_DIR := bin/exec
# flags
FLAG_SRC := -Werror -Wall -MMD -MP
CFLAGS	 := -std=c11
LDFLAGS  := -lc

.PHONY: run
run: $(EXEC_DIR)/main
	@./$(EXEC_DIR)/main

# executables
$(EXEC_DIR)/main: FLAG_SRC += -O0 -g $(addprefix -D,$(ARGS)) $(addprefix -I./,main test)
$(EXEC_DIR)/main: $(OBJ_DIR)/main.c.o
	@mkdir -p $(@D)
	@$(CXX) $^ -o $@ $(LDFLAGS)
	@chmod +x $@

# generate c to o file
$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(@D)
	@$(CC) -c $< -o $@ $(CFLAGS) $(FLAG_SRC)

.PHONY: clean
clean:
	@echo "cleanup build files ..."
	@rm -rf bin

-include $(shell find $(OBJ_DIR) -type f -name "*.d")
