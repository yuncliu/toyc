CPP=${CXX}
LEX=flex
YACC=bison
CC=${CC}
CFLAGS=-Wall -std=c++11 -g -DDEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
LINKS	=  -lstdc++ -lm `llvm-config-3.7 --ldflags --libs --system-libs --libs core`

EXEOBJS	= lex.o \
			parser.o \
			ast.o \
			main.o

LEX_CC		= lex.cc
YACC_CC		= parser.cc

EXE=a.out
TEST=t.out

VPATH		 = ./

INC=-I.\
	-I/usr/lib/llvm-3.7/include

LIB=-L.


all:clean $(YACC_CC) $(LEX_CC) $(EXE)

test: clean $(YACC_CC) $(LEX_CC)
	cd unittest;make

gtest:
	git submodule init
	git submodule update
	mkdir googletest/build
	cd googletest/build;cmake ..;make



$(EXE):$(EXEOBJS)
		 $(CPP) $(CFLAGS) $(LIB) $(EXEOBJS) -o $@ $(LINKS)

%.o:%.cc
		 $(CPP) $(CFLAGS) $(INC) -c $< -o $@

%.o:%.c
		 $(CC) $(CFLAGS) $(INC) -c $< -o $@

%.cc:%.l
		 $(LEX) --header-file=lex.h -o $@ $<

%.cc:%.y
		 $(YACC) --defines=parser.h -o $@ $<

clean:
	rm -rf $(EXEOBJS)
	rm -rf $(EXE)
	rm -rf $(LEX_CC)
	rm -rf $(YACC_CC)
	rm -rf parser.h lex.h
	rm -rf core*
	cd unittest;make clean
