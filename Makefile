OUTPUTS = Kalendar_server Kalendar_client
CC = gcc

all: $(OUTPUTS)

clean:
	rm -f $(OUTPUTS)
	rm -f *.o
	
.PHONY: all clean

%.o: %.c k_s_definitions.h
	$(CC) -c -o $@ $<
	
Kalendar_server: semestralka.c server.c
	gcc $^ -o $@

Kalendar_client: client.c
	gcc $^ -o $@
	
                
