CC := g++
CFLAGS := -Wall -g -pthread
TARGET_TRAVELMONITOR := travelMonitorClient
TARGET_MONITOR := monitorServer

SRCS_TRAVELMONITOR := $(filter-out monitorServer.cpp ,$(wildcard *.cpp))
SRCS_MONITOR := $(filter-out main.cpp ,$(wildcard *.cpp))
OBJS_TRAVELMONITOR := $(patsubst %.cpp,%.o, $(SRCS_TRAVELMONITOR))
OBJS_MONITOR := $(patsubst %.cpp,%.o, $(SRCS_MONITOR))

all: $(TARGET_TRAVELMONITOR) $(TARGET_MONITOR)
$(TARGET_TRAVELMONITOR): $(OBJS_TRAVELMONITOR)
	$(CC) $(CFLAGS) -o $@ $^
$(TARGET_MONITOR): $(OBJS_MONITOR)
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
clean:
	rm -rf $(TARGET_TRAVELMONITOR) $(TARGET_MONITOR) *.o

.PHONY: all clean