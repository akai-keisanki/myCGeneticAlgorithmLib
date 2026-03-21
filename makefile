SOURCE_FILES = $(wildcard src/*.c)
OBJECT_FILES = $(SOURCE_FILES:src/%.c=obj/%.o)

CC = cc

test: test.o $(OBJECT_FILES)
	$(CC) -o $@ $^ -lm

test.o: test.c
	$(CC) -c -o $@ $^

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) -c -o $@ $^

clean:
	rm -r obj/
	rm test.o test
