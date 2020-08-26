# Source: http://web.engr.oregonstate.edu/~rookert/cs162/03.mp4

CC = gcc
CCFLAGS = -std=gnu99
CCFLAGS += -Wall
CCFLAGS += -pedantic
CCFLAGS += -g

OBJS = smallsh.o
SRCS = smallsh.c
#HEADERS = smallsh.h

default: $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o smallsh

$(OBJS): $(SRCS)
	$(CC) $(CCFLAGS) -c $(@:.o=.c)

clean:
	rm *.o smallsh
