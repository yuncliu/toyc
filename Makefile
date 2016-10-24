CPP=${CXX}
LEX=flex
YACC=bison
CC=${CC}
CFLAGS=-Wall -std=c++11 -g -fno-exceptions -fno-rtti -fno-common -Wcast-qual \
	   -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
LINKS	=  -lstdc++ -lm `llvm-config --ldflags --system-libs --libs`

EXEOBJS	= lex.o \
		  parser.o \
		  Expr.o \
		  Stmt.o \
		  Visitor.o \
		  DumpVisitor.o \
		  LLVMVisitor.o \
		  FlexBisonFrontEnd.o \
		  main.o

LEX_CC		= lex.cc
YACC_CC		= parser.cc

EXE=cc
TEST=t.out

VPATH		 = ./

INC=-I.\
	-I`llvm-config --includedir`

LIB=-L.


all: $(YACC_CC) $(LEX_CC) $(EXE)

test: $(YACC_CC) $(LEX_CC)
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
