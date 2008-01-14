%pure-parser
%error-verbose
%parse-param { Linker& linker }
%parse-param { HoeCore::StringPool& pool }
%parse-param { Scaner& lex }
%lex-param   { HoeCore::StringPool& pool }
%lex-param   { Scaner& lex }
	//%parse-param {int *randomness}

%{
#define YYDEBUG 1
#define YYERROR_VERBOSE 1

#include "StdAfx.h"
#include "scan.h"
#include "linker.h"

using namespace HoeCore;

static int yylex(union YYSTYPE * l, HoeCore::StringPool& pool, Scaner& lex)
{ 
	return lex.Lex(pool,l);
}

static int yyerror(Linker& linker, HoeCore::StringPool& pool, Scaner& lex, char* err)
{
	fprintf(stderr, "%s(%d) : ", (const tchar*)lex.GetIdentifier(), lex.GetLine());
	fprintf(stderr, err, lex.GetText());
	fprintf(stderr, "\n");
	return 0;
}

PInterface * pint = NULL;
VectorUniversal vec;

#define DONE(p) if (p) { pint->Done();pint = NULL; }

%}

%union {
  const char * string;
  double real;
  long long num;
}

%token TK_Stream TK_StreamFile TK_Texture TK_TextureFile
%token TK_Material TK_MaterialFile TK_IndexFile
%token TK_Picture TK_Model TK_Index TK_Namespace
%token <string>TK_name  
%token <num> TK_num  
%token <real> TK_real  
%token <real> TK_perc  
%token <string> TK_string

%%
file: 
	res
;
res : resource
	| res resource
;
resource
		: '\n'
		| stream
		| picture
		| index
		| model
		| namespace
        | { pint = &linker; } attribute
;
namespace:
		TK_Namespace TK_name '\n'
		 { linker.PushNamespace($2); } 
		res
		'~' TK_Namespace
		 { linker.PopNamespace(); }
;
stream:	TK_Stream TK_name { pint  = linker.AddObject($2, ERT_Stream, lex.GetLocation()); } 
			'[' { /* start fvf */ } fvf ']' '\n'
			stream_data
		'~' TK_Stream '\n'
;
fvf:	TK_name
		| fvf '|' TK_name
;
stream_data:
		stream_data_row '\n'
		| stream_data stream_data_row '\n'
;
stream_data_row
		: stream_data_item
		| stream_data_row ',' stream_data_item
		| stream_data_row ';' stream_data_item
;
stream_data_item
		: TK_num
		| TK_real 
;	
picture: 
		TK_Picture TK_name '\n' { pint = linker.AddObject($2, ERT_Picture, lex.GetLocation()); }
		  attributes
		'~' TK_Picture '\n' { DONE(pint); }
;
attributes
		: attribute
		| '\n'
		| attributes attribute
		| attributes '\n'
attribute
		: TK_name '=' TK_name
			{ pint->AddProp($1, $3); } '\n' 
		| TK_name '=' TK_string
			{ pint->AddProp($1, $3); } '\n' 
		| TK_name '=' TK_num
			{ pint->AddProp($1, $3); } '\n' 
		| TK_name '=' TK_real
			{ pint->AddProp($1, (Universal::TReal)$3); } '\n' 
		| TK_name '=' TK_perc
			{ pint->AddProp($1, Universal((Universal::TReal)$3, 
								Universal::TypePercent)); } '\n' 
		| TK_name '=' vector
			{ pint->AddProp($1, vec); } '\n' 
        | TK_name '(' func_params ')'
            { pint->Func($1, vec); } '\n'
;
func_params
		: func_param
		| func_params func_param
func_param
        : TK_name { vec.Set($1); }
        | TK_perc { vec.Set((float)$1); }
        | TK_num { vec.Set($1); }
        | TK_string { vec.Set($1); }
;
index:	TK_Index TK_name '\n'
		index_data
		'~' TK_Index '\n'
;
index_data
		: '\n'
		| TK_num
		| index_data ',' TK_num
		| index_data '\n'
		| index_data '\n' TK_num
;
model:	TK_Model model_name '\n'
		'~' TK_Model '\n'
;
model_name
		: TK_name
		| TK_name '(' ')'
		| TK_name '(' model_param ')'
;
model_param
		: TK_name
		| model_param ',' TK_name
;
vector: '(' vector_item ')'
;
vector_item
		: TK_num 
		  { vec.Set($1); }
		| vector_item ',' TK_num
		  { vec.Add($3); }
;
%%
