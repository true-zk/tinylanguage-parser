# tinylanguage-parser
a parser for tiny language

file structure & discription:
  main.c            : program entry : int main()
  globals.h         : data structure
  util.c util.h     : printToken printSyntaxTree copyString createNewNode...
  lexer.c lexer.h   : tiny language lexer : call getToken to return next symbol
  parses.c parse.h  : tiny language parser: parse the input code and output a syntax tree
  
usage:
  x86
  1. open a terminal in build
  2. $make clean : clean build files
  3. $make parse : build the tiny language parser
  4. $make testp : test the sample in ./build/test/sample.tny
  
 cite:
  introduction to tiny language
  [reference 1 ](https://blog.csdn.net/qq_41112170/article/details/106891811)
  [reference 2 ](https://slideplayer.com/slide/7733294/)

  ps:
    a curriculum design for fundamentals of compiling
