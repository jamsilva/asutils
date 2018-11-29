AR=ar
CC=clang
LD=${CC}
STRIP=strip
CFLAGS=
LDFLAGS=-Wl,-O1
STRIPFLAGS=--strip-debug --remove-section=.comment --remove-section=.gnu.version --remove-section=.jcr --remove-section=.note --remove-section=.note.ABI-tag --remove-section=.note.gnu.build-id --remove-section=.note.gnu.gold-version
EXTRAIDIRS=
EXTRALDIRS=
UNWINDINFO=-funwind-tables -fno-ident -fomit-frame-pointer -fvisibility=protected -fvisibility-inlines-hidden
WARNS=-Weverything -Wno-disabled-macro-expansion -Wno-missing-noreturn -Wno-padded -Wno-reserved-id-macro
O=z

test-dynamic: dynamic test

test-static: static test

test:
	${CC} -O${O} ${CFLAGS} ${UNWINDINFO} -Iinclude ${EXTRAIDIRS} -c test.c -o test.o
	${LD} test.o -o test -L. ${EXTRALDIRS} -lasutils -lpthread -lunwind
	${STRIP} ${STRIPFLAGS} test

dynamic: compile-dynamic
	${LD} ${LDFLAGS} -shared -o libasutils.so *.o
	${STRIP} ${STRIPFLAGS} libasutils.so

static: compile-static
	${AR} rcs libasutils.a *.o

compile-dynamic: clean
	${CC} -O${O} ${CFLAGS} ${UNWINDINFO} ${WARNS} -Iinclude ${EXTRAIDIRS} -c src/*.c -fPIC

compile-static: clean
	${CC} -O${O} ${CFLAGS} ${UNWINDINFO} ${WARNS} -Iinclude ${EXTRAIDIRS} -c src/*.c

clean:
	rm *.o test libasutils.a libasutils.so 2> /dev/null || true
