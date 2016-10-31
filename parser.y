%{
#include "lex.h"
#include <memory>
#ifdef __APPLE__
#include <istream>
#endif
//#include "ASTNode.h"
#include "FlexBisonFrontEnd.h"
void yyerror(const char *s);
extern char* yytext;
#define YYSTYPE std::shared_ptr<ASTNode>
%}
%token    IDENTIFIER I_CONSTANT F_CONSTANT STRING_LITERAL FUNC_NAME SIZEOF
%token    PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token    AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token    SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token    XOR_ASSIGN OR_ASSIGN
%token    TYPEDEF_NAME ENUMERATION_CONSTANT

%token    TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token    CONST RESTRICT VOLATILE
%token    BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token    COMPLEX IMAGINARY 
%token    STRUCT UNION ENUM ELLIPSIS

%token    CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token    ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL

%start translation_unit
%%

primary_expression
    : IDENTIFIER {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = yytext;
        $$->type = ASTNode::IDENTIFIER;
    }
    | constant {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        //$$ = $1;
        $$->type = ASTNode::CONSTANT;
        $$->children.push_back($1);
        ////printf("primary [%s]\n", $1->value.c_str());
    }
    | string
    | '(' expression ')'
    | generic_selection
    ;

constant
    : I_CONSTANT {
        /* includes character_constant */
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = yytext;
        //$$->type = ASTNode::CONSTANT;
        $$->type = ASTNode::IntegerLiteral;
        ////printf("constant [%s]\n", $$->value.c_str());
    }
    | F_CONSTANT {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = yytext;
        $$->type = ASTNode::FloatingLiteral;
    }
    | ENUMERATION_CONSTANT    /* after it has been defined as such */
    ;

enumeration_constant        /* before it has been defined as such */
    : IDENTIFIER
    ;

string
    : STRING_LITERAL
    | FUNC_NAME
    ;

generic_selection
    : GENERIC '(' assignment_expression ',' generic_assoc_list ')'
    ;

generic_assoc_list
    : generic_association
    | generic_assoc_list ',' generic_association
    ;

generic_association
    : type_name ':' assignment_expression
    | DEFAULT ':' assignment_expression
    ;

postfix_expression
    : primary_expression {
        $$ = $1;
        //printf("postfix_expression from primary_expression[%s]\n", $$->value.c_str());
    }
    | postfix_expression '[' expression ']'
    | postfix_expression '(' ')'
    | postfix_expression '(' argument_expression_list ')'
    | postfix_expression '.' IDENTIFIER
    | postfix_expression PTR_OP IDENTIFIER
    | postfix_expression INC_OP
    | postfix_expression DEC_OP
    | '(' type_name ')' '{' initializer_list '}'
    | '(' type_name ')' '{' initializer_list ',' '}'
    ;

argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression
    ;

unary_expression
    : postfix_expression {
        $$ = $1;
        //printf("unary_expression from postfix_expression [%s]\n", $$->value.c_str());
    }
    | INC_OP unary_expression
    | DEC_OP unary_expression
    | unary_operator cast_expression
    | SIZEOF unary_expression
    | SIZEOF '(' type_name ')'
    | ALIGNOF '(' type_name ')'
    ;

unary_operator
    : '&'
    | '*'
    | '+'
    | '-'
    | '~'
    | '!'
    ;

cast_expression
    : unary_expression {
        $$ = $1;
        //printf("cast from unary_expression[%s]\n", $$->value.c_str());
    }
    | '(' type_name ')' cast_expression
    ;

multiplicative_expression
    : cast_expression {
        $$ = $1;
        //printf("multiplicative_expression from cast_expression[%s]\n", $$->value.c_str());
    }
    | multiplicative_expression '*' cast_expression
    | multiplicative_expression '/' cast_expression
    | multiplicative_expression '%' cast_expression
    ;

additive_expression
    : multiplicative_expression {
        $$ = $1;
    }
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;

shift_expression
    : additive_expression {
        $$ = $1;
        //printf("shift_expression [%s]\n", $$->value.c_str());
    }
    | shift_expression LEFT_OP additive_expression
    | shift_expression RIGHT_OP additive_expression
    ;

relational_expression
    : shift_expression {
        $$ = $1;
    }
    | relational_expression '<' shift_expression
    | relational_expression '>' shift_expression
    | relational_expression LE_OP shift_expression
    | relational_expression GE_OP shift_expression
    ;

equality_expression
    : relational_expression {
        $$ = $1;
    }
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;

and_expression
    : equality_expression {
        $$ = $1;
    }
    | and_expression '&' equality_expression
    ;

exclusive_or_expression
    : and_expression {
        $$ = $1;
        //printf("and_expression [%s]\n", $$->value.c_str());
    }
    | exclusive_or_expression '^' and_expression
    ;

inclusive_or_expression
    : exclusive_or_expression {
        $$ = $1;
    }
    | inclusive_or_expression '|' exclusive_or_expression
    ;

logical_and_expression
    : inclusive_or_expression {
        $$ = $1;
    }
    | logical_and_expression AND_OP inclusive_or_expression
    ;

logical_or_expression
    : logical_and_expression {
        $$ = $1;
        //printf("logical_or_expression [%s]\n", $$->value.c_str());
    }
    | logical_or_expression OR_OP logical_and_expression
    ;

conditional_expression
    : logical_or_expression {
        $$ = $1;
    }
    | logical_or_expression '?' expression ':' conditional_expression
    ;

assignment_expression
    : conditional_expression {
        $$ = $1;
        //printf("assignment_expression [%s]\n", $$->value.c_str());
    }
    | unary_expression assignment_operator assignment_expression
    ;

assignment_operator
    : '='
    | MUL_ASSIGN
    | DIV_ASSIGN
    | MOD_ASSIGN
    | ADD_ASSIGN
    | SUB_ASSIGN
    | LEFT_ASSIGN
    | RIGHT_ASSIGN
    | AND_ASSIGN
    | XOR_ASSIGN
    | OR_ASSIGN
    ;

expression
    : assignment_expression {
        $$ = $1;
        //printf("expression [%s]\n", $$->value.c_str());
    }
    | expression ',' assignment_expression
    ;

constant_expression
    : conditional_expression    /* with constraints */
    ;

declaration
    : declaration_specifiers ';'
    | declaration_specifiers init_declarator_list ';' {
        //printf("declaration [%s] [%s]\n", $1->value.c_str(), $2->value.c_str());
        //$$ = std::shared_ptr<ASTNode>(new ASTNode());
        //std::shared_ptr<VarDecl> varDecl(new VarDecl());
        //varDecl->type_specifier = $1->value;
        //varDecl->children.push_back($2);
        //$$ = std::dynamic_pointer_cast<ASTNode>(varDecl);
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = "VarDecl";
        $$->type = ASTNode::VarDecl;
        $$->children.push_back($1);
        $$->children.push_back($2);
    }
    | static_assert_declaration
    ;

declaration_specifiers
    : storage_class_specifier declaration_specifiers
    | storage_class_specifier
    | type_specifier declaration_specifiers
    | type_specifier {
        $$ = $1;
    }
    | type_qualifier declaration_specifiers
    | type_qualifier
    | function_specifier declaration_specifiers
    | function_specifier
    | alignment_specifier declaration_specifiers
    | alignment_specifier
    ;

init_declarator_list
    : init_declarator {
        $$ = $1;
    }
    | init_declarator_list ',' init_declarator
    ;

init_declarator
    : declarator '=' initializer {
        //printf("init_declarator [%s]= [%s]\n", $1->value.c_str(), $3->value.c_str());
        $$ = std::shared_ptr<ASTNode>(new ASTNode);
        $$->value = "init_declarator";
        $$->children.push_back($1);
        $$->children.push_back($3);
        $$->type = ASTNode::EXPR;
    }
    | declarator
    ;

storage_class_specifier
    : TYPEDEF    /* identifiers must be flagged as TYPEDEF_NAME */
    | EXTERN
    | STATIC
    | THREAD_LOCAL
    | AUTO
    | REGISTER
    ;

type_specifier
    : VOID
    | CHAR
    | SHORT
    | INT {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = yytext;
        $$->type = ASTNode::TYPE_SPECIFIER;
    }
    | LONG
    | FLOAT
    | DOUBLE {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = yytext;
        $$->type = ASTNode::TYPE_SPECIFIER;
    }
    | SIGNED
    | UNSIGNED
    | BOOL
    | COMPLEX
    | IMAGINARY          /* non-mandated extension */
    | atomic_type_specifier
    | struct_or_union_specifier
    | enum_specifier
    | TYPEDEF_NAME        /* after it has been defined as such */
    ;

struct_or_union_specifier
    : struct_or_union '{' struct_declaration_list '}'
    | struct_or_union IDENTIFIER '{' struct_declaration_list '}'
    | struct_or_union IDENTIFIER
    ;

struct_or_union
    : STRUCT
    | UNION
    ;

struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;

struct_declaration
    : specifier_qualifier_list ';'    /* for anonymous struct/union */
    | specifier_qualifier_list struct_declarator_list ';'
    | static_assert_declaration
    ;

specifier_qualifier_list
    : type_specifier specifier_qualifier_list
    | type_specifier
    | type_qualifier specifier_qualifier_list
    | type_qualifier
    ;

struct_declarator_list
    : struct_declarator
    | struct_declarator_list ',' struct_declarator
    ;

struct_declarator
    : ':' constant_expression
    | declarator ':' constant_expression
    | declarator
    ;

enum_specifier
    : ENUM '{' enumerator_list '}'
    | ENUM '{' enumerator_list ',' '}'
    | ENUM IDENTIFIER '{' enumerator_list '}'
    | ENUM IDENTIFIER '{' enumerator_list ',' '}'
    | ENUM IDENTIFIER
    ;

enumerator_list
    : enumerator
    | enumerator_list ',' enumerator
    ;

enumerator    /* identifiers must be flagged as ENUMERATION_CONSTANT */
    : enumeration_constant '=' constant_expression
    | enumeration_constant
    ;

atomic_type_specifier
    : ATOMIC '(' type_name ')'
    ;

type_qualifier
    : CONST
    | RESTRICT
    | VOLATILE
    | ATOMIC
    ;

function_specifier
    : INLINE
    | NORETURN
    ;

alignment_specifier
    : ALIGNAS '(' type_name ')'
    | ALIGNAS '(' constant_expression ')'
    ;

declarator
    : pointer direct_declarator
    | direct_declarator {
        $$ = $1;
    }
    ;

direct_declarator
    : IDENTIFIER {
        //printf("id [%s]\n", yytext);
        $$ = std::shared_ptr<ASTNode>(new ASTNode);
        $$->value = yytext;
        $$->type = ASTNode::IDENTIFIER;
    }
    | '(' declarator ')'
    | direct_declarator '[' ']'
    | direct_declarator '[' '*' ']'
    | direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
    | direct_declarator '[' STATIC assignment_expression ']'
    | direct_declarator '[' type_qualifier_list '*' ']'
    | direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
    | direct_declarator '[' type_qualifier_list assignment_expression ']'
    | direct_declarator '[' type_qualifier_list ']'
    | direct_declarator '[' assignment_expression ']'
    | direct_declarator '(' parameter_type_list ')' {
        $$ = $1;
        $$->type = ASTNode::IDENTIFIER;
        $$->children.push_back($3);
    }
    | direct_declarator '(' ')' {
        //printf("id [%s]\n", yytext);
        $$ = $1;
        //$$->value = yytext;
        $$->type = ASTNode::IDENTIFIER;
    }
    | direct_declarator '(' identifier_list ')'
    ;

pointer
    : '*' type_qualifier_list pointer
    | '*' type_qualifier_list
    | '*' pointer
    | '*'
    ;

type_qualifier_list
    : type_qualifier
    | type_qualifier_list type_qualifier
    ;


parameter_type_list
    : parameter_list ',' ELLIPSIS
    | parameter_list {
        $$ = $1;
    }
    ;

parameter_list
    : parameter_declaration {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->children.push_back($1);
        $$->value = "parameter_list";
    }
    | parameter_list ',' parameter_declaration {
        $$ = $1;
        $$->children.push_back($3);
    }
    ;

parameter_declaration
    : declaration_specifiers declarator {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = "parameter_declaration";
        $$->children.push_back($1);
        $$->children.push_back($2);
    }
    | declaration_specifiers abstract_declarator
    | declaration_specifiers
    ;

identifier_list
    : IDENTIFIER
    | identifier_list ',' IDENTIFIER
    ;

type_name
    : specifier_qualifier_list abstract_declarator
    | specifier_qualifier_list
    ;

abstract_declarator
    : pointer direct_abstract_declarator
    | pointer
    | direct_abstract_declarator
    ;

direct_abstract_declarator
    : '(' abstract_declarator ')'
    | '[' ']'
    | '[' '*' ']'
    | '[' STATIC type_qualifier_list assignment_expression ']'
    | '[' STATIC assignment_expression ']'
    | '[' type_qualifier_list STATIC assignment_expression ']'
    | '[' type_qualifier_list assignment_expression ']'
    | '[' type_qualifier_list ']'
    | '[' assignment_expression ']'
    | direct_abstract_declarator '[' ']'
    | direct_abstract_declarator '[' '*' ']'
    | direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'
    | direct_abstract_declarator '[' STATIC assignment_expression ']'
    | direct_abstract_declarator '[' type_qualifier_list assignment_expression ']'
    | direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'
    | direct_abstract_declarator '[' type_qualifier_list ']'
    | direct_abstract_declarator '[' assignment_expression ']'
    | '(' ')'
    | '(' parameter_type_list ')'
    | direct_abstract_declarator '(' ')'
    | direct_abstract_declarator '(' parameter_type_list ')'
    ;

initializer
    : '{' initializer_list '}'
    | '{' initializer_list ',' '}'
    | assignment_expression {
        $$ = $1;
        //printf("initializer [%s]\n", $$->value.c_str());
    }
    ;

initializer_list
    : designation initializer
    | initializer
    | initializer_list ',' designation initializer
    | initializer_list ',' initializer
    ;

designation
    : designator_list '='
    ;

designator_list
    : designator
    | designator_list designator
    ;

designator
    : '[' constant_expression ']'
    | '.' IDENTIFIER
    ;

static_assert_declaration
    : STATIC_ASSERT '(' constant_expression ',' STRING_LITERAL ')' ';'
    ;

statement
    : labeled_statement
    | compound_statement
    | expression_statement
    | selection_statement
    | iteration_statement
    | jump_statement {
        $$ = $1;
    }
    ;

labeled_statement
    : IDENTIFIER ':' statement
    | CASE constant_expression ':' statement
    | DEFAULT ':' statement
    ;

compound_statement
    : '{' '}' {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = "compound_statement";
    }
    | '{'  block_item_list '}' {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = "compound_statement";
        $$->children.swap($2->children);
        //$$->children.insert($$->children.end(), $2->children.begin(), $2->children.end());
        //$$->children.push_back($2);
    }
    ;

block_item_list
    : block_item {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = "block_item_list";
        $$->children.push_back($1);
    }
    | block_item_list block_item {
        $$ = $1;
        $$->children.push_back($2);
    }
    ;

block_item
    : declaration {
        $$ = $1;
    }
    | statement {
        $$ = $1;
    }
    ;

expression_statement
    : ';'
    | expression ';'
    ;

selection_statement
    : IF '(' expression ')' statement ELSE statement
    | IF '(' expression ')' statement
    | SWITCH '(' expression ')' statement
    ;

iteration_statement
    : WHILE '(' expression ')' statement
    | DO statement WHILE '(' expression ')' ';'
    | FOR '(' expression_statement expression_statement ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
    | FOR '(' declaration expression_statement ')' statement
    | FOR '(' declaration expression_statement expression ')' statement
    ;

jump_statement
    : GOTO IDENTIFIER ';'
    | CONTINUE ';'
    | BREAK ';'
    | RETURN ';' {
        $$ = $1;
    }
    | RETURN expression ';' {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        $$->value = "return statement";
        $$->type = ASTNode::STATEMENT;
        $$->children.push_back($2);
    }
    ;

translation_unit
    : external_declaration {
        $$ = FlexBisonFrontEnd::ast;
        //FlexBisonFrontEnd::ast = std::shared_ptr<ASTNode>(new ASTNode());
        $$->children.push_back($1);
        $$->value = "TranslationUnitDecl";
        $$->type = ASTNode::TranslationUnitDecl;
    }
    | translation_unit external_declaration {
        $$ = $1;
        $$->children.push_back($2);
    }
    ;

external_declaration
    : function_definition {
        $$ = $1;
    }
    | declaration {
        $$ = $1;
    }
    ;

function_definition
    : declaration_specifiers declarator declaration_list compound_statement {
        printf("function1\n");
    }
    | declaration_specifiers declarator compound_statement {
        $$ = std::shared_ptr<ASTNode>(new ASTNode());
        //$$->value = "function";
        $$->value = "FunctionDecl";
        $$->type = ASTNode::FunctionDecl;
        $$->children.push_back($1);
        $$->children.push_back($2);
        $$->children.push_back($3);
    }
    ;

declaration_list
    : declaration
    | declaration_list declaration
    ;

%%
#include <stdio.h>

void yyerror(const char *s)
{
    fflush(stdout);
    fprintf(stderr, "*** %s\n", s);
}
