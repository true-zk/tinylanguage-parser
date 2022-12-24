# tinylanguage-parser
a parser for tiny language<br>

file structure & discription:<br>
- main.c            : program entry : int main() <br>
- globals.h         : data structure<br>
- util.c util.h     : printToken printSyntaxTree copyString createNewNode...<br>
- lexer.c lexer.h   : tiny language lexer : call getToken to return next symbol<br>
- parses.c parse.h  : tiny language parser: parse the input code and output a syntax tree<br>
  
usage:<br>
 - x86<br>
  1. open a terminal in build<br>
  2. $make clean : clean build files<br>
  3. $make parse : build the tiny language parser<br>
  4. $make testp : test the sample in ./build/test/sample.tny<br>
  
 cite:<br>
 - introduction to tiny language<br>
 1. [reference 1 ](https://blog.csdn.net/qq_41112170/article/details/106891811)<br>
 2. [reference 2 ](https://slideplayer.com/slide/7733294/)<br>

  ps:<br>
 - a curriculum design for fundamentals of compiling<br>
