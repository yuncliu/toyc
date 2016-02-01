CPP=${CXX}
LEX=flex
YACC=bison
CC=${CC}
CFLAGS=-Wall -g  -DDEBUG
LINKS	=  -lstdc++ -lgtest -lpthread

CCOBJS	=../lex.o \
		../parser.o \
		lex_unittest.o \
		main_test.o

TEST=../t.out

VPATH		 = ./

INC=-I.\
	-I.. \
	-I../googletest/googletest/include

LIB=-L. \
	-L../googletest/build/googlemock/gtest


all:clean $(TEST)


$(TEST):$(CCOBJS)
		 $(CPP) $(CFLAGS) $(LIB) $(CCOBJS) -o $@ $(LINKS)

%.o:%.cc
		 $(CPP) $(CFLAGS) $(INC) -c $< -o $@

%.o:%.c
		 $(CC) $(CFLAGS) $(INC) -c $< -o $@

%.cc:%.l
		 $(LEX) --header-file=lex.h -o $@ $<

%.cc:%.y
		 $(YACC) --defines=parser.h -o $@ $<

clean:
	rm -rf $(CCOBJS)
	rm -rf $(TEST)
	rm -rf core*
