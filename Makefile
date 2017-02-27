all: server client

server: server.o
	$(LINK.cpp) $^ -o $@

client: client.o
	$(LINK.cpp) $^ -o $@
