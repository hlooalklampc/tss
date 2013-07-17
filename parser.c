/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 10 "parser.lemon"


#include "json/json.h"

#include "string.h"
#include "parser.h"
#include "scanner.h"
#include "xx.h"

static json_object *xx_ret_literal(int type, xx_parser_token *T)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_int(type));
	json_object_object_add(ret, "value", json_object_new_string(T->token));
	
	return ret;
}

static json_object *xx_ret_namespace(xx_parser_token *T)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("namespace"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	return ret;
}

static json_object *xx_ret_class(xx_parser_token *T, json_object *class_definition)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("class"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	if (class_definition) {
		json_object_object_add(ret, "definition", class_definition);		
	}

	return ret;
}

static json_object *xx_ret_class_definition(json_object *properties, json_object *methods)
{
	json_object *ret = json_object_new_object();

	if (properties) {
		json_object_object_add(ret, "properties", properties);
	}
	if (methods) {
		json_object_object_add(ret, "methods", methods);
	}

	return ret;
}

static json_object *xx_ret_class_property(int visibility, xx_parser_token *T)
{
	json_object *ret = json_object_new_object();

	if (visibility == XX_T_PUBLIC) {
		json_object_object_add(ret, "visibility", json_object_new_string("public"));	
	} else {
		json_object_object_add(ret, "visibility", json_object_new_string("protected"));	
	}
	json_object_object_add(ret, "type", json_object_new_string("property"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	return ret;
}

static json_object *xx_ret_class_method(int visibility, xx_parser_token *T, json_object *statements)
{
	json_object *ret = json_object_new_object();

	if (visibility == XX_T_PUBLIC) {
		json_object_object_add(ret, "visibility", json_object_new_string("public"));	
	} else {
		json_object_object_add(ret, "visibility", json_object_new_string("protected"));	
	}
	json_object_object_add(ret, "type", json_object_new_string("method"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	return ret;
}

static json_object *xx_ret_list(json_object *list_left, json_object *right_list)
{
	json_object *ret;
	int i, array_length;

	if (list_left) {

		ret = json_object_new_array();

		if (json_object_get_type(list_left) == json_type_array) {
			array_length = json_object_array_length(list_left);
			for (i = 0; i < array_length; i++){			
				json_object_array_add(ret, json_object_array_get_idx(list_left, i));
			}			
			//json_object_put(list_left);
		} else {
			json_object_array_add(ret, list_left);			
		}

		json_object_array_add(ret, right_list);			

		return ret;
	}

	return right_list;	
}

static json_object *xx_ret_let_statement(xx_parser_token *T, json_object *expr)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("let"));
	json_object_object_add(ret, "variable", json_object_new_string(T->token));
	json_object_object_add(ret, "expr", expr);

	return ret;
}


// 139 "parser.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    xx_TOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is xx_TOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    xx_ARG_SDECL     A static variable declaration for the %extra_argument
**    xx_ARG_PDECL     A parameter declaration for the %extra_argument
**    xx_ARG_STORE     Code to store %extra_argument into yypParser
**    xx_ARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 36
#define YYACTIONTYPE unsigned char
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy35;
  int yy71;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 59
#define YYNRULE 30
#define YYERRORSYMBOL 21
#define YYERRSYMDT yy71
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* Next are that tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
static YYACTIONTYPE yy_action[] = {
 /*     0 */    32,   10,   13,   57,   58,   56,   90,    1,    2,    3,
 /*    10 */    25,   26,   33,   34,   35,   36,   37,   38,   23,   39,
 /*    20 */    26,   67,   16,   40,   65,   50,   53,   66,   16,   40,
 /*    30 */    47,   39,   26,   14,   49,   56,   12,   50,   53,    4,
 /*    40 */    46,   51,    7,   54,   44,   45,   27,    6,   17,   24,
 /*    50 */    41,   48,   11,   61,   22,   27,   63,   27,   64,   59,
 /*    60 */    27,   52,   29,   28,   30,   55,   31,   62,   60,   82,
 /*    70 */    83,   15,    8,   17,   84,   18,   20,   85,   86,   87,
 /*    80 */    88,   67,   19,    5,    9,   21,   42,   41,   67,   67,
 /*    90 */    67,   67,   43,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     3,   26,   27,   28,   29,   30,   22,   23,   24,   25,
 /*    10 */    32,   33,   15,   16,   17,   18,   19,   20,   31,   32,
 /*    20 */    33,    7,    8,    9,    7,    8,    9,    7,    8,    9,
 /*    30 */    31,   32,   33,   28,   29,   30,    7,    8,    9,    2,
 /*    40 */     7,    3,    5,    3,   12,    6,   13,    4,   10,    7,
 /*    50 */    10,    7,    7,    0,    7,   13,    0,   13,    0,    0,
 /*    60 */    13,    4,   14,    3,   34,    4,    4,    0,    0,    4,
 /*    70 */     4,   30,    3,   10,    4,    3,   12,    4,    4,    4,
 /*    80 */     4,   35,   11,    3,    6,    6,    3,   10,   35,   35,
 /*    90 */    35,   35,   11,
};
#define YY_SHIFT_USE_DFLT (-4)
static signed char yy_shift_ofst[] = {
 /*     0 */    37,   59,   68,   53,   80,   43,   67,   69,   78,   29,
 /*    10 */    45,   56,   58,   17,   20,   -4,   63,   72,   71,   64,
 /*    20 */    79,   47,   -4,   42,   -4,   -4,   -4,   60,   48,   -3,
 /*    30 */    62,   -4,   65,   66,   70,   73,   74,   75,   76,   -4,
 /*    40 */    77,   83,   81,   32,   39,   33,   -4,   44,   -4,   -4,
 /*    50 */    38,   57,   -4,   40,   61,   -4,   -4,   14,   -4,
};
#define YY_REDUCE_USE_DFLT (-26)
static signed char yy_reduce_ofst[] = {
 /*     0 */   -16,  -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,  -25,
 /*    10 */   -26,  -26,  -26,    5,   41,  -26,  -26,  -26,  -26,  -26,
 /*    20 */   -26,  -13,  -26,  -22,  -26,  -26,  -26,  -26,  -26,   30,
 /*    30 */   -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,
 /*    40 */   -26,  -26,  -26,  -26,  -26,   -1,  -26,  -22,  -26,  -26,
 /*    50 */   -26,  -26,  -26,  -26,  -26,  -26,  -26,   41,  -26,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */    89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
 /*    10 */    89,   89,   89,   89,   89,   72,   89,   89,   89,   89,
 /*    20 */    89,   89,   74,   89,   75,   78,   80,   89,   89,   89,
 /*    30 */    89,   81,   89,   89,   89,   89,   89,   89,   89,   79,
 /*    40 */    89,   89,   89,   89,   89,   89,   76,   89,   77,   68,
 /*    50 */    89,   89,   70,   89,   89,   71,   73,   89,   69,
};
#define YY_SZ_ACTTAB (sizeof(yy_action)/sizeof(yy_action[0]))

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammer, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  int stateno;       /* The state-number */
  int major;         /* The major token value.  This is the code
                     ** number for the token at this stack level */
  YYMINORTYPE minor; /* The user-supplied minor token value.  This
                     ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
  int yyerrcnt;                 /* Shifts left before out of the error */
  xx_ARG_SDECL                /* A place to hold %extra_argument */
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void xx_Trace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *yyTokenName[] = { 
  "$",             "COMMA",         "NAMESPACE",     "IDENTIFIER",  
  "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "PUBLIC",        "PROTECTED",     "FUNCTION",      "PARENTHESES_OPEN",
  "PARENTHESES_CLOSE",  "LET",           "ASSIGN",        "INTEGER",     
  "STRING",        "DOUBLE",        "NULL",          "FALSE",       
  "TRUE",          "error",         "program",       "xx_language", 
  "xx_namespace_def",  "xx_class_def",  "xx_class_definition",  "xx_class_properties_definition",
  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_class_method_definition",  "xx_statement_list",
  "xx_statement",  "xx_let_statement",  "xx_expr",     
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "program ::= xx_language",
 /*   1 */ "xx_language ::= xx_namespace_def",
 /*   2 */ "xx_language ::= xx_class_def",
 /*   3 */ "xx_namespace_def ::= NAMESPACE IDENTIFIER DOTCOMMA",
 /*   4 */ "xx_class_def ::= CLASS IDENTIFIER BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*   5 */ "xx_class_def ::= CLASS IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /*   6 */ "xx_class_definition ::= xx_class_properties_definition",
 /*   7 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*   8 */ "xx_class_definition ::= xx_class_methods_definition",
 /*   9 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  10 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  11 */ "xx_class_property_definition ::= PUBLIC IDENTIFIER DOTCOMMA",
 /*  12 */ "xx_class_property_definition ::= PROTECTED IDENTIFIER DOTCOMMA",
 /*  13 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  14 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  15 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  16 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  17 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  18 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  19 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  20 */ "xx_statement_list ::= xx_statement",
 /*  21 */ "xx_statement ::= xx_let_statement",
 /*  22 */ "xx_let_statement ::= LET IDENTIFIER ASSIGN xx_expr DOTCOMMA",
 /*  23 */ "xx_expr ::= IDENTIFIER",
 /*  24 */ "xx_expr ::= INTEGER",
 /*  25 */ "xx_expr ::= STRING",
 /*  26 */ "xx_expr ::= DOUBLE",
 /*  27 */ "xx_expr ::= NULL",
 /*  28 */ "xx_expr ::= FALSE",
 /*  29 */ "xx_expr ::= TRUE",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *xx_TokenName(int tokenType){
#ifndef NDEBUG
  if( tokenType>0 && tokenType<(sizeof(yyTokenName)/sizeof(yyTokenName[0])) ){
    return yyTokenName[tokenType];
  }else{
    return "Unknown";
  }
#else
  return "";
#endif
}

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to xx_ and xx_Free.
*/
void *xx_Alloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(YYCODETYPE yymajor, YYMINORTYPE *yypminor){
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
// 158 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 523 "parser.c"
      break;
    case 23:
// 171 "parser.lemon"
{ json_object_put((yypminor->yy35)); }
// 528 "parser.c"
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor( yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from xx_Alloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void xx_Free(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
  (*freeProc)((void*)pParser);
}

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead            /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  /* if( pParser->yyidx<0 ) return YY_NO_ACTION;  */
  i = yy_shift_ofst[stateno];
  if( i==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
    int iFallback;            /* Fallback token */
    if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
           && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
           yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
      }
#endif
      return yy_find_shift_action(pParser, iFallback);
    }
#endif
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead            /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  i = yy_reduce_ofst[stateno];
  if( i==YY_REDUCE_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer ot the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
  if( yypParser->yyidx>=YYSTACKDEPTH ){
     xx_ARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     xx_ARG_STORE; /* Suppress warning about unused %extra_argument var */
     return;
  }
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = yyNewState;
  yytos->major = yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 22, 1 },
  { 23, 1 },
  { 23, 1 },
  { 24, 3 },
  { 25, 5 },
  { 25, 4 },
  { 26, 1 },
  { 26, 2 },
  { 26, 1 },
  { 27, 2 },
  { 27, 1 },
  { 29, 3 },
  { 29, 3 },
  { 28, 2 },
  { 28, 1 },
  { 30, 7 },
  { 30, 8 },
  { 30, 7 },
  { 30, 8 },
  { 31, 2 },
  { 31, 1 },
  { 32, 1 },
  { 33, 5 },
  { 34, 1 },
  { 34, 1 },
  { 34, 1 },
  { 34, 1 },
  { 34, 1 },
  { 34, 1 },
  { 34, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  xx_ARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<sizeof(yyRuleName)/sizeof(yyRuleName[0]) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  // <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  // <lineno> <thisfile>
  **     break;
  */
      case 0:
// 167 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy35;
}
// 775 "parser.c"
        break;
      case 1:
      case 2:
      case 21:
// 173 "parser.lemon"
{
	yygotominor.yy35 = yymsp[0].minor.yy35;
}
// 784 "parser.c"
        break;
      case 3:
// 181 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_namespace(yymsp[-1].minor.yy0);
  yy_destructor(2,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 793 "parser.c"
        break;
      case 4:
// 185 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy35);
  yy_destructor(5,&yymsp[-4].minor);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 803 "parser.c"
        break;
      case 5:
// 189 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class(yymsp[-2].minor.yy0, NULL);
  yy_destructor(5,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 813 "parser.c"
        break;
      case 6:
// 193 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class_definition(yymsp[0].minor.yy35, NULL);
}
// 820 "parser.c"
        break;
      case 7:
// 197 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class_definition(yymsp[-1].minor.yy35, yymsp[0].minor.yy35);
}
// 827 "parser.c"
        break;
      case 8:
// 201 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class_definition(NULL, yymsp[0].minor.yy35);
}
// 834 "parser.c"
        break;
      case 9:
      case 13:
      case 19:
// 205 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_list(yymsp[-1].minor.yy35, yymsp[0].minor.yy35);
}
// 843 "parser.c"
        break;
      case 10:
      case 14:
      case 20:
// 209 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_list(NULL, yymsp[0].minor.yy35);
}
// 852 "parser.c"
        break;
      case 11:
// 213 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0);
  yy_destructor(8,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 861 "parser.c"
        break;
      case 12:
// 217 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 870 "parser.c"
        break;
      case 15:
// 229 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL);	
  yy_destructor(8,&yymsp[-6].minor);
  yy_destructor(10,&yymsp[-5].minor);
  yy_destructor(11,&yymsp[-3].minor);
  yy_destructor(12,&yymsp[-2].minor);
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 883 "parser.c"
        break;
      case 16:
// 233 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-1].minor.yy35);	
  yy_destructor(8,&yymsp[-7].minor);
  yy_destructor(10,&yymsp[-6].minor);
  yy_destructor(11,&yymsp[-4].minor);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 896 "parser.c"
        break;
      case 17:
// 237 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL);	
  yy_destructor(9,&yymsp[-6].minor);
  yy_destructor(10,&yymsp[-5].minor);
  yy_destructor(11,&yymsp[-3].minor);
  yy_destructor(12,&yymsp[-2].minor);
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 909 "parser.c"
        break;
      case 18:
// 241 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, yymsp[-1].minor.yy35);	
  yy_destructor(9,&yymsp[-7].minor);
  yy_destructor(10,&yymsp[-6].minor);
  yy_destructor(11,&yymsp[-4].minor);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 922 "parser.c"
        break;
      case 22:
// 257 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_let_statement(yymsp[-3].minor.yy0, yymsp[-1].minor.yy35);
  yy_destructor(13,&yymsp[-4].minor);
  yy_destructor(14,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 932 "parser.c"
        break;
      case 23:
// 261 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0);
}
// 939 "parser.c"
        break;
      case 24:
// 265 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0);
}
// 946 "parser.c"
        break;
      case 25:
// 269 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0);
}
// 953 "parser.c"
        break;
      case 26:
// 273 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0);
}
// 960 "parser.c"
        break;
      case 27:
// 277 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_literal(XX_T_NULL, NULL);
  yy_destructor(18,&yymsp[0].minor);
}
// 968 "parser.c"
        break;
      case 28:
// 281 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_literal(XX_T_FALSE, NULL);
  yy_destructor(19,&yymsp[0].minor);
}
// 976 "parser.c"
        break;
      case 29:
// 285 "parser.lemon"
{
	yygotominor.yy35 = xx_ret_literal(XX_T_TRUE, NULL);
  yy_destructor(20,&yymsp[0].minor);
}
// 984 "parser.c"
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yypParser,yygoto);
  if( yyact < YYNSTATE ){
    yy_shift(yypParser,yyact,yygoto,&yygotominor);
  }else if( yyact == YYNSTATE + YYNRULE + 1 ){
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  xx_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  xx_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  xx_ARG_FETCH;
#define TOKEN (yyminor.yy0)
// 141 "parser.lemon"


	//fprintf(stderr, "error!\n");

	if (status->scanner_state->start_length) {
		fprintf(stderr, "Syntax error, %s", status->scanner_state->start);
	} else {
		fprintf(stderr, "EOF");
	}

	//status->syntax_error_len = 48 + Z_STRLEN_P(status->scanner_state->active_file);
	//status->syntax_error = emalloc(sizeof(char) * status->syntax_error_len);
	

	status->status = XX_PARSING_FAILED;

// 1043 "parser.c"
  xx_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  xx_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  xx_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "xx_Alloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void xx_(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  xx_TOKENTYPE yyminor       /* The value for the token */
  xx_ARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
    if( yymajor==0 ) return;
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  xx_ARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,yymajor);
    if( yyact<YYNSTATE ){
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      if( yyendofinput && yypParser->yyidx>=0 ){
        yymajor = 0;
      }else{
        yymajor = YYNOCODE;
      }
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else if( yyact == YY_ERROR_ACTION ){
      int yymx;
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_shift_action(yypParser,YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }else{
      yy_accept(yypParser);
      yymajor = YYNOCODE;
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}

#define SUCCESS 1
#define FAILURE 0

const xx_token_names xx_tokens[] =
{
	{ XX_T_INTEGER,       		"INTEGER" },
	{ XX_T_DOUBLE,        		"DOUBLE" },
	{ XX_T_STRING,        		"STRING" },
	{ XX_T_IDENTIFIER,    		"IDENTIFIER" },
	{ XX_T_AT,	   	    		"@" },
	{ XX_T_COMMA,	   	    	"," },
	{ XX_T_ASSIGN,       		"=" },
	{ XX_T_COLON,            	":" },
	{ XX_T_PARENTHESES_OPEN,  	"(" },
	{ XX_T_PARENTHESES_CLOSE, 	")" },
	{ XX_T_BRACKET_OPEN,     	"{" },
	{ XX_T_BRACKET_CLOSE,    	"}" },
 	{ XX_T_SBRACKET_OPEN,    	"[" },
	{ XX_T_SBRACKET_CLOSE,   	"]" },	
	{  0, NULL }
};

/**
 * Wrapper to alloc memory within the parser
 */
static void *xx_wrapper_alloc(size_t bytes){
	return malloc(bytes);
}

/**
 * Wrapper to free memory within the parser
 */
static void xx_wrapper_free(void *pointer){
	//free(pointer);
}

/**
 * Creates a parser_token to be passed to the parser
 */
static void xx_parse_with_token(void* xx_parser, int opcode, int parsercode, xx_scanner_token *token, xx_parser_status *parser_status){

	xx_parser_token *pToken;

	pToken = malloc(sizeof(xx_parser_token));
	pToken->opcode = opcode;
	pToken->token = token->value;
	pToken->token_len = token->len;
	pToken->free_flag = 1;

	xx_(xx_parser, parsercode, pToken, parser_status);

	token->value = NULL;
	token->len = 0;
}

/**
 * Creates an error message when it's triggered by the scanner
 */
static void xx_scanner_error_msg(xx_parser_status *parser_status){

	/*char *error, *error_part;
	XX_scanner_state *state = parser_status->scanner_state;

	//PHALCON_INIT_VAR(*error_msg);
	ALLOC_INIT_ZVAL(*error_msg);
	if (state->start) {
		error = emalloc(sizeof(char) * (128 + state->start_length +  Z_STRLEN_P(state->active_file)));
		if (state->start_length > 16) {
			error_part = estrndup(state->start, 16);
			sprintf(error, "Parsing error before '%s...' in %s on line %d", error_part, Z_STRVAL_P(state->active_file), state->active_line);
			efree(error_part);
		} else {
			sprintf(error, "Parsing error before '%s' in %s on line %d", state->start, Z_STRVAL_P(state->active_file), state->active_line);
		}
		ZVAL_STRING(*error_msg, error, 1);
	} else {
		error = emalloc(sizeof(char) * (64 + Z_STRLEN_P(state->active_file)));
		sprintf(error, "Parsing error near to EOF in %s", Z_STRVAL_P(state->active_file));
		ZVAL_STRING(*error_msg, error, 1);
	}
	efree(error);*/
}

/**
 * Receives the comment tokenizes and parses it
 */
int xx_parse_annotations(){

	/*zval *error_msg = NULL;

	ZVAL_NULL(result);

	if (Z_TYPE_P(comment) != IS_STRING) {
		//phalcon_throw_exception_string(phalcon_annotations_exception_ce, SL("Comment must be a string") TSRMLS_CC);
		return FAILURE;
	}

	if(XX_internal_parse_annotations(&result, comment, file_path, line, &error_msg TSRMLS_CC) == FAILURE){
		//phalcon_throw_exception_string(phalcon_annotations_exception_ce, Z_STRVAL_P(error_msg), Z_STRLEN_P(error_msg) TSRMLS_CC);
		return FAILURE;
	}*/

	return SUCCESS;
}

/**
 * Parses a comment returning an intermediate array representation
 */
int xx_parse_program(char *program, unsigned int program_length) {

	char *error;
	xx_scanner_state *state;
	xx_scanner_token token;
	int scanner_status, status = SUCCESS, start_lines;
	xx_parser_status *parser_status = NULL;
	void* xx_parser;
	/*zval processed_comment;*/

	/**
	 * Check if the comment has content
	 */
	/*if (!Z_STRVAL_P(comment)) {
		ZVAL_BOOL(*result, 0);
		return FAILURE;
	}

	if (Z_STRLEN_P(comment) < 2) {
		ZVAL_BOOL(*result, 0);
		return SUCCESS;
	}*/

	/**
	 * Start the reentrant parser
	 */
	xx_parser = xx_Alloc(xx_wrapper_alloc);

	parser_status = malloc(sizeof(xx_parser_status));
	state = malloc(sizeof(xx_scanner_state));

	parser_status->status = XX_PARSING_OK;
	parser_status->scanner_state = state;
	parser_status->ret = NULL;
	parser_status->token = &token;
	parser_status->syntax_error = NULL;

	/**
	 * Initialize the scanner state
	 */
	state->active_token = 0;
	state->start = program;
	state->start_length = 0;
	state->mode = XX_MODE_RAW;
	//state->active_file = file_path;

	/**
	 * Possible start line
	 */
	/*if (Z_TYPE_P(line) == IS_LONG) {
		state->active_line = Z_LVAL_P(line) - start_lines;
	} else {
		state->active_line = 1;
	}*/

	state->end = state->start;

	while (0 <= (scanner_status = xx_get_token(state, &token))) {

		state->active_token = token.opcode;

		state->start_length = (program + program_length - state->start);

		switch (token.opcode) {

			case XX_T_IGNORE:
				break;

			case XX_T_NAMESPACE:
				xx_(xx_parser, XX_NAMESPACE, NULL, parser_status);
				break;
			case XX_T_CLASS:
				xx_(xx_parser, XX_CLASS, NULL, parser_status);
				break;
			case XX_T_PUBLIC:
				xx_(xx_parser, XX_PUBLIC, NULL, parser_status);
				break;
			case XX_T_PROTECTED:
				xx_(xx_parser, XX_PROTECTED, NULL, parser_status);
				break;
			case XX_T_FUNCTION:
				xx_(xx_parser, XX_FUNCTION, NULL, parser_status);
				break;
			case XX_T_LET:
				xx_(xx_parser, XX_LET, NULL, parser_status);
				break;
			case XX_T_DOTCOMMA:
				xx_(xx_parser, XX_DOTCOMMA, NULL, parser_status);
				break;
			/*case XX_T_COMMA:
				xx_(xx_parser, XX_COMMA, NULL, parser_status);
				break;*/
			case XX_T_ASSIGN:
				xx_(xx_parser, XX_ASSIGN, NULL, parser_status);
				break;
			/*case XX_T_COLON:
				xx_(xx_parser, XX_COLON, NULL, parser_status);
				break;*/

			case XX_T_PARENTHESES_OPEN:
				xx_(xx_parser, XX_PARENTHESES_OPEN, NULL, parser_status);
				break;
			case XX_T_PARENTHESES_CLOSE:
				xx_(xx_parser, XX_PARENTHESES_CLOSE, NULL, parser_status);
				break;

			case XX_T_BRACKET_OPEN:
				xx_(xx_parser, XX_BRACKET_OPEN, NULL, parser_status);
				break;
			case XX_T_BRACKET_CLOSE:
				xx_(xx_parser, XX_BRACKET_CLOSE, NULL, parser_status);
				break;

			/*case XX_T_SBRACKET_OPEN:
				xx_(xx_parser, XX_SBRACKET_OPEN, NULL, parser_status);
				break;
			case XX_T_SBRACKET_CLOSE:
				xx_(xx_parser, XX_SBRACKET_CLOSE, NULL, parser_status);
				break;*/

			case XX_T_NULL:
				xx_(xx_parser, XX_NULL, NULL, parser_status);
				break;
			case XX_T_TRUE:
				xx_(xx_parser, XX_TRUE, NULL, parser_status);
				break;
			case XX_T_FALSE:
				xx_(xx_parser, XX_FALSE, NULL, parser_status);
				break;

			case XX_T_INTEGER:
				xx_parse_with_token(xx_parser, XX_T_INTEGER, XX_INTEGER, &token, parser_status);
				break;
			case XX_T_DOUBLE:
				xx_parse_with_token(xx_parser, XX_T_DOUBLE, XX_DOUBLE, &token, parser_status);
				break;
			case XX_T_STRING:
				xx_parse_with_token(xx_parser, XX_T_STRING, XX_STRING, &token, parser_status);
				break;
			case XX_T_IDENTIFIER:
				xx_parse_with_token(xx_parser, XX_T_IDENTIFIER, XX_IDENTIFIER, &token, parser_status);
				break;
			/*case XX_T_ARBITRARY_TEXT:
				XX_parse_with_token(xx_parser, XX_T_ARBITRARY_TEXT, XX_ARBITRARY_TEXT, &token, parser_status);
				break;*/

			default:
				parser_status->status = XX_PARSING_FAILED;
				fprintf(stderr, "Scanner: unknown opcode %d\n", token.opcode);
				/*if (!*error_msg) {
					error = emalloc(sizeof(char) * (48 + Z_STRLEN_P(state->active_file)));
					sprintf(error, "Scanner: unknown opcode %d on in %s line %d", token.opcode, Z_STRVAL_P(state->active_file), state->active_line);
					//PHALCON_INIT_VAR(*error_msg);
					ALLOC_INIT_ZVAL(*error_msg);
					ZVAL_STRING(*error_msg, error, 1);
					efree(error);
				}*/				
				break;
		}

		if (parser_status->status != XX_PARSING_OK) {
			status = FAILURE;
			break;
		}

		state->end = state->start;
	}

	if (status != FAILURE) {
		switch (scanner_status) {
			case XX_SCANNER_RETCODE_ERR:
			case XX_SCANNER_RETCODE_IMPOSSIBLE:
				//if (!*error_msg) {
					//XX_scanner_error_msg(parser_status, error_msg TSRMLS_CC);
				//}
				fprintf(stderr, "scanner error\n");
				status = FAILURE;
				break;
			default:
				xx_(xx_parser, 0, NULL, parser_status);
		}
	}

	state->active_token = 0;
	state->start = NULL;

	if (parser_status->status != XX_PARSING_OK) {
		status = FAILURE;
		/*if (parser_status->syntax_error) {
			if (!*error_msg) {
				//PHALCON_INIT_VAR(*error_msg);
				ALLOC_INIT_ZVAL(*error_msg);
				ZVAL_STRING(*error_msg, parser_status->syntax_error, 1);
			}
			efree(parser_status->syntax_error);
		}*/
		fprintf(stderr, "error!\n");
	}

	

	xx_Free(xx_parser, xx_wrapper_free);

	if (status != FAILURE) {
		if (parser_status->status == XX_PARSING_OK) {
			fprintf(stderr, "%s\n", json_object_to_json_string(parser_status->ret));
			/*if (parser_status->ret) {
				ZVAL_ZVAL(*result, parser_status->ret, 0, 0);
				ZVAL_NULL(parser_status->ret);
				zval_ptr_dtor(&parser_status->ret);
			} else {
				array_init(*result);
			}*/
		}
	}

	//efree(Z_STRVAL(processed_comment));*/

	free(parser_status);
	free(state);

	return status;
}

int main(int argc, char **argv) {

	FILE *fp;
	char ch;
	char *program;
	int i;

	program = malloc(sizeof(char) * 2048);

	if (argc > 0) {		
		
		fp = fopen(argv[1], "r");
		if (!fp) {
			fprintf(stderr, "Cant open file\n");
			exit(1);
		}

		i = 0;
		while (!feof(fp)) {
			ch = fgetc(fp);
			program[i++] = ch;
		}
		program[i - 1] = '\0';
		fclose(fp);

		//fprintf(stderr, "%s\n", program);

		xx_parse_program(program, i - 1);
	}
}