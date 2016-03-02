CPP=${CXX}
LEX=flex
YACC=bison
CC=${CC}
CFLAGS=-Wall -std=c++11 -g -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
LINKS	=  -lstdc++ -lgtest -lpthread -lm \
		`llvm-config --ldflags --libs --system-libs --libs`

CCOBJS	=../lex.o \
		../parser.o \
		../ast.o \
		../Visitor.o \
		ast_test.o \
		main_test.o

TEST=../t.out

VPATH		 = ./

INC=-I.\
	-I.. \
	-I../googletest/googletest/include \
	-I/usr/lib/llvm-3.7/include


LIB=-L. \
	-L../googletest/build/googlemock/gtest


all: $(TEST)


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
