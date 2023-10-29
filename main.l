%{

int total =0 ;

%}

%option noyywrap




%%




#.* {total++; fprintf(yyout,"Directiva preprocesor - %s\n\n",yytext);}

[''|,|;(|)|{|}|.|_] {total++; fprintf(yyout,"Delimitator - %s\n\n",yytext);}

"="  {total++; fprintf(yyout,"Operator de atribuire - %s\n\n",yytext);}

"+"|"-"|"*"|"/"  {total++; fprintf(yyout,"Operator aritmetic - %s\n\n",yytext);}

"and"|"or"|"not"|"nand"|"xor"|"nor"|"xnor"  {total++; fprintf(yyout,"Operatori logici - %s\n\n",yytext);}

"<="|">="|"++"|"!="|"=="|"<"|">"  {total++; fprintf(yyout,"Operator relational - %s\n\n",yytext);}

("int")|("if")|("else")|("while")|("do")|("break")|("continue")|("double")|("float")|("return")|("EOF") {total++; fprintf(yyout,"Cuvant cheie - %s\n\n",yytext);}

[a-zA-Z_][a-zA-Z0-9_]* {total++; fprintf(yyout,"Identificator - %s\n\n",yytext);}

[0-9]+ {total++; fprintf(yyout,"Numar intreg - %s\n\n",yytext);}

["]([^"\\\n]|\\.|\\\n)*["] {total++; fprintf(yyout,"Sir - %s\n\n",yytext);}

"//".*" "*    {total++;fprintf(yyout,"Comentariu pe o linie - %s\n\n",yytext);}

\/\*(.*\n)*.*\*\/    {total++;fprintf(yyout,"Comentariu pe mai multe linii - %s\n\n",yytext);}



. {fprintf(yyout,"",yytext);}




[\t\n]+



%%

main()
{
extern FILE *yyin, *yyout; 
  
    
    yyin = fopen("input.txt", "r"); 
  
   
    yyout = fopen("Output.txt", "w"); 
  
    yylex();

	fprintf(yyout,"\n\n\n\n\n Total Tokens = %d",total);
	 

    return 0; 
}