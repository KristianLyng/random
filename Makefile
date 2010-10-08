TARGET=pathfinder
OBJS=main.o

%.o: %.c
	gcc -Wall -c $^ -o $@

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS)

clean:
	rm -rf $(OBJS) $(TARGET)

all: $(TARGET)
