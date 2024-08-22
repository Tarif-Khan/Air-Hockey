CC = g++
CFLAGS = -Wall -Wextra -std=c++11

SRCS = air_hockey.cpp puck.cpp slider.cpp zone.cpp key.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = air_hockey

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)