TARGET=pathfinder
OBJS=main.o

CFLAGS=-Wall -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Wreturn-type -Wcast-qual -Wwrite-strings -Wswitch -Wshadow -Wcast-align -Wunused-parameter -Wchar-subscripts -Winline -Wnested-externs -Wredundant-decls -Wformat -g -Wextra -Wno-missing-field-initializers -Wno-sign-compare -Werror -O2

# -O0 -pg -ftest-coverage -fprofile-arcs
# -O2


%.o: %.c Makefile
	gcc $(CFLAGS) -c $^ -o $@

$(TARGET): $(OBJS) Makefile
	gcc $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -rf $(OBJS) $(TARGET)

all: $(TARGET)
