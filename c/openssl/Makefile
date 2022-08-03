
all:




%.o: %.c
	$(CC) -o $@ -c $<


sslreq: ssl_req.o hexdump.o
	$(CC) -o $@ $^ -lssl -lcrypto


req1: req1.o hexdump.o
	$(CC) -o $@ $^ -lssl -lcrypto


req2: req2.o hexdump.o
	$(CC) -o $@ $^ -lssl -lcrypto


req3: req3.o hexdump.o
	$(CC) -o $@ $^

all: sslreq req1 req2 req3
