PRG=gnu.exe
GCC=gcc
MSC=cl

GCCFLAGS=-ansi -Wall -Wextra -pedantic -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement -O2
MSCFLAGS=/EHa /W4 /Za /Zc:forScope /nologo /D_CRT_SECURE_NO_DEPRECATE /D"_SECURE_SCL 0" /O2i /GL

VALGRIND_OPTIONS=-q --leak-check=full
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines

OSTYPE := $(shell uname)
ifeq (,$(findstring CYGWIN,$(OSTYPE)))
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

OBJECTS0=spvector.c
DRIVER0=driver.c

gcc0:
	$(GCC) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) $(CYGWIN) -o $(PRG)
msc0:
	$(MSC) -Fe$@.exe $(DRIVER0) $(OBJECTS0) $(MSCFLAGS) 
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24:
	echo "running test$@"
	@echo "should run in less than 200 ms"
	./$(PRG) $@ >studentout$@
	@echo "lines after the next are mismatches with master output -- see out$@"
	diff out$@ studentout$@ $(DIFF_OPTIONS)
mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8 mem9 mem10 mem11 mem12 mem13 mem14 mem15 mem16 mem17 mem18 mem19 mem20 mem21 mem22 mem23 mem24:
	echo "running memory test $@"
	@echo "should run in less than 2000 ms"
	valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null 2>difference$@
	@echo "lines after this are memory errors"; cat difference$@
clean:
	rm -f *.exe *.o *.obj *.tds *.exe.manifest student* difference*
