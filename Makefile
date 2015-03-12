TARGET = traces
SRCS = main.cpp point.cpp board.cpp segment.cpp paintboard.cpp boarditem.cpp xml.cpp track.cpp
OBJS = $(SRCS:.cpp=.o)
CC = g++


CLIBS = `wx-config-3.0 --cxxflags --libs`
CFLAGS  = -g -Wall -std=c++11
GCC = g++

$(TARGET): $(OBJS)
	$(CC) $(CLIBS) $(CFLAGS) $(OBJS) -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) $(CLIBS) $< -o $@

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

include .depend

clean:
	rm -f $(OBJS) $(TARGET)
