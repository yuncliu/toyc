CPP=${CXX}
LEX=flex
YACC=bison
CC=${CC}
CFLAGS=-Wall -g  -DDEBUG
LINKS	=  -lstdc++


TESTOBJS	= lex.o \
			  parser.o \
			  main.o

LEX_CC		= lex.cc
YACC_CC		= parser.cc

TEST=a.out

VPATH		 = ./

INC=-I.\
	-I./bits

LIB=-L.


all:clean $(YACC_CC) $(LEX_CC) $(TEST)


$(TEST):$(TESTOBJS)
		 $(CPP) $(CFLAGS) $(LIB) $(TESTOBJS) -o $@ $(LINKS)

%.o:%.cc
		 $(CPP) $(CFLAGS) $(INC) -c $< -o $@

%.o:%.c
		 $(CC) $(CFLAGS) $(INC) -c $< -o $@

%.cc:%.l
		 $(LEX) --header-file=lex.h -o $@ $<

%.cc:%.y
		 $(YACC) --defines=parser.h -o $@ $<

clean:
	rm -rf $(TESTOBJS)
	rm -rf $(TEST)
	rm -rf $(LEX_CC)
	rm -rf $(YACC_CC)
	rm -rf parser.h lex.h
	rm -rf core*
