OBJ_DIR := bin/obj
EXEC_DIR := bin/exec
# flags
FLAG_SRC := -Werror -Wall -MMD -MP
CFLAGS	 := -std=c11
LDFLAGS  := -lc
MAIN_OBJS := \
		$(OBJ_DIR)/src/main.c.o \
		$(OBJ_DIR)/src/text.c.o \
		$(OBJ_DIR)/src/date_parser.c.o
SCHED_OBJS := \
		$(OBJ_DIR)/src/sched.c.o \
		$(OBJ_DIR)/src/text.c.o \
		$(OBJ_DIR)/src/date_parser.c.o
STINF_OBJS := \
		$(OBJ_DIR)/src/stinf.c.o

.PHONY: run
run: $(EXEC_DIR)/main
	@./$(EXEC_DIR)/main

# executables
$(EXEC_DIR)/main: FLAG_SRC += -O0 -g $(addprefix -D,$(ARGS)) -I./src
$(EXEC_DIR)/main: $(MAIN_OBJS)
	@mkdir -p $(@D)
	@$(CC) $^ -o $@ $(LDFLAGS)
	@chmod +x $@

$(EXEC_DIR)/sched: FLAG_SRC += -O0 -g $(addprefix -D,$(ARGS)) -I./src
$(EXEC_DIR)/sched: $(SCHED_OBJS)
	@mkdir -p $(@D)
	@$(CC) $^ -o $@ $(LDFLAGS)
	@chmod +x $@

$(EXEC_DIR)/stinf: FLAG_SRC += -O0 -g $(addprefix -D,$(ARGS)) -I./src
$(EXEC_DIR)/stinf: $(STINF_OBJS)
	@mkdir -p $(@D)
	@$(CC) $^ -o $@ $(LDFLAGS)
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
