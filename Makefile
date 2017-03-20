AR=ar
CC=clang
CXX=clang++
LD=clang++
CFLAGS=
CXXFLAGS=
LDFLAGS=
EXTRAIDIRS=
EXTRALDIRS=
TARGET=$(shell uname -m)
FPREFIX=as_
WARNS=-Weverything -Wno-cast-align -Wno-disabled-macro-expansion -Wno-missing-noreturn -Wno-missing-prototypes -Wno-padded -Wno-reserved-id-macro

test-dynamic: dynamic test

test-static: static test

test:
	${CC} ${CFLAGS} -Iinclude ${EXTRAIDIRS} -c test.c -o test.o
	${CXX} test.o -o test -L. ${EXTRALDIRS} -lasutils -ldl -lpthread -lunwind -lunwind-ptrace -lunwind-${TARGET}

dynamic: compile-dynamic
	${LD} ${LDFLAGS} -shared -o libasutils.so *.o

static: compile-static
	${AR} rcs libasutils.a *.o

compile-dynamic: clean include/as/prefix_.h
	${CC} ${CFLAGS} ${WARNS} -Iinclude ${EXTRAIDIRS} -c src/*.c -fPIC

compile-static: clean include/as/prefix_.h
	${CC} ${CFLAGS} ${WARNS} -Iinclude ${EXTRAIDIRS} -c src/*.c

clean:
	rm *.o test libasutils.a libasutils.so include/as/prefix_.h 2> /dev/null || true

include/as/prefix_.h:
	echo "#define ASUTILS_PREFIX ${FPREFIX}" > $@
