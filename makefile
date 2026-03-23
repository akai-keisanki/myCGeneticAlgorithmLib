OBJ_DIR = obj

SOURCE_FILES = $(wildcard src/*.c)
OBJECT_FILES = $(SOURCE_FILES:src/%.c=$(OBJ_DIR)/%.o)

LIB_TARGET_FILE = libmycgen.so

CC = cc

$(LIB_TARGET_FILE): $(OBJECT_FILES)
	$(CC) -shared -o $@ $^

$(OBJ_DIR)/%.o: src/%.c $(OBJ_DIR)
	$(CC) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(LIB_TARGET_FILE)
	rm -f test.o test

test: test.o $(LIB_TARGET_FILE)
	$(CC) -o $@ test.o -lm -L. $(LIB_TARGET_FILE:lib%.so=-l%)

test.o: test.c
	$(CC) -c -o $@ $^

run_test: test
	LD_LIBRARY_PATH=. ./test
