TARGET=pathfinder
OBJS=main.o

CFLAGS=-Wall -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Wreturn-type -Wcast-qual -Wwrite-strings -Wswitch -Wshadow -Wcast-align -Wunused-parameter -Wchar-subscripts -Winline -Wnested-externs -Wredundant-decls -Wformat -O0 -g -fno-inline -DDIAGNOSTICS -Wextra -Wno-missing-field-initializers -Wno-sign-compare -fstack-protector-all -Werror
%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS)

clean:
	rm -rf $(OBJS) $(TARGET)

all: $(TARGET)
