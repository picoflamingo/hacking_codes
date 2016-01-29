all: pocrypter target

SRC_COMMON=pocrypt.c
# Optimisation re-orders code putting dummy before secure_function
# In that case we have to the section
CFLAGS=-O0
#CFLAGS=-g -O0 -DDUMP

pocrypter: pocrypter.c ${SRC_COMMON}
	${CC} ${CFLAGS} $< ${SRC_COMMON} -o $@

target: target.c ${SRC_COMMON} 
	${CC} ${CFLAGS} $< ${SRC_COMMON} -o $@


.PHONY:
clean:
	rm pocrypter target
