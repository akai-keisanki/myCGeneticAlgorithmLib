OBJ_DIR = obj

SOURCE_FILES = $(wildcard src/*.c)
OBJECT_FILES = $(SOURCE_FILES:src/%.c=$(OBJ_DIR)/%.o)

LIB_TARGET_FILE = libmycgen.so
LIB_COMPILE_FLAGS = -Iinclude

TEST_LINK_FLAGS = -lm -L. $(LIB_TARGET_FILE:lib%.so=-l%)
TEST_COMPILE_FLAGS = -Iinclude

CC = cc

$(LIB_TARGET_FILE): $(OBJECT_FILES)
	$(CC) -shared -o $@ $^

$(OBJ_DIR)/%.o: src/%.c $(OBJ_DIR)
	$(CC) -c -o $@ $< $(LIB_COMPILE_FLAGS)

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(LIB_TARGET_FILE)
	rm -f test.o test

test: test.o $(LIB_TARGET_FILE)
	$(CC) -o $@ test.o $(TEST_LINK_FLAGS)

test.o: test.c
	$(CC) -c -o $@ $^ $(TEST_COMPILE_FLAGS)

run_test: test
	LD_LIBRARY_PATH=. ./test
