CXXFLAGS = -Wall -ggdb -pthread

LDFLAGS = -lpthread -lm

TARGETS = client-server

all:		$(TARGETS)

client-server:    	client-server.o  process.o utils.o
			g++ -o $@ $^ $(LDFLAGS)

fifo_unsafe.o fifo_safe.o:	fifo.h

utils.o process.o:	%.o:	%.h dbc.h utils.h

clean:
			rm -f *.o *~

cleanall:		clean
			rm -f $(TARGETS)
