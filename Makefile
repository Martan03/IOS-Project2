CC:=cc
TARGET:=proj2
CFLAGS:=-g -Wall -Wextra -Werror -pedantic -std=gnu99

SRC:=$(wildcard src/*.c)
DOBJ:=$(patsubst src/%.c, obj/%.o, $(SRC))

-include dep.d

.DEFAULT_GOAL:=debug

.PHONY: debug clean deb pack


debug:
	mkdir -p obj/debug
	clang -MM $(SRC) | sed -r 's/^.*:.*$$/obj\/\0/' > dep.d
	make deb

deb: $(DOBJ)
	$(CC) $(CFLAGS) -lrt -pthread $^ -o $(TARGET)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm obj/*.o || true
	rm $(TARGET) || true

pack:
	zip xsleza26.zip src/* Makefile
