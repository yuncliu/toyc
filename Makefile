CPP=${CXX}
LEX=flex
YACC=bison
CC=${CC}
CFLAGS=-Wall -g  -DDEBUG
LINKS	=  -lstdc++


EXEOBJS	= lex.o \
			  parser.o \
			  main.o

LEX_CC		= lex.cc
YACC_CC		= parser.cc

EXE=a.out
TEST=t.out

VPATH		 = ./

INC=-I.\
	-I./bits

LIB=-L.


all:clean $(YACC_CC) $(LEX_CC) $(EXE)

test: $(TEST)

gtest:
	git submodule update
	mkdir googletest/build
	cd googletest/build;cmake ..;make


$(TEST):
	cd unittest;make


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
