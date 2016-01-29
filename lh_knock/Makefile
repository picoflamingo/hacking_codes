all:knock

knock:knock.c
	${CC} ${CFLAGS} -o $@ $< -lpcap

.PHONY:
clean:
	rm knock.c
