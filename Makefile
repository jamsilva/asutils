AR=ar
CC=clang
LD=clang
CFLAGS=
LDFLAGS=
EXTRAIDIRS=
EXTRALDIRS=
UNWINDINFO=-fasynchronous-unwind-tables -fno-ident -fomit-frame-pointer
FPREFIX=as_
WARNS=-Weverything -Wno-cast-align -Wno-disabled-macro-expansion -Wno-missing-noreturn -Wno-missing-prototypes -Wno-padded -Wno-reserved-id-macro

test-dynamic: dynamic test

test-static: static test

test:
	${CC} ${CFLAGS} ${UNWINDINFO} -Iinclude ${EXTRAIDIRS} -c test.c -o test.o
	${LD} test.o -o test -L. ${EXTRALDIRS} -lasutils -lpthread -lunwind

dynamic: compile-dynamic
	${LD} ${LDFLAGS} -shared -o libasutils.so *.o

static: compile-static
	${AR} rcs libasutils.a *.o

compile-dynamic: clean include/as/prefix_.h
	${CC} ${CFLAGS} ${UNWINDINFO} ${WARNS} -Iinclude ${EXTRAIDIRS} -c src/*.c -fPIC

compile-static: clean include/as/prefix_.h
	${CC} ${CFLAGS} ${UNWINDINFO} ${WARNS} -Iinclude ${EXTRAIDIRS} -c src/*.c

clean:
	rm *.o test libasutils.a libasutils.so include/as/prefix_.h 2> /dev/null || true

include/as/prefix_.h:
	echo "#define ASUTILS_PREFIX ${FPREFIX}" > $@
