/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the TINY compiler  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "lexer.h"
#include "parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode * stmt_sequence(void);
static TreeNode * statement(void);
static TreeNode * if_stmt(void);
static TreeNode * repeat_stmt(void);
static TreeNode * assign_stmt(void);
static TreeNode * read_stmt(void);
static TreeNode * write_stmt(void);
static TreeNode * myexp(void);
static TreeNode * simple_exp(void);
static TreeNode * term(void);
static TreeNode * factor(void);

static void syntaxError(char * message)
{ fprintf(listing,"\n>>> ");
  fprintf(listing,"Syntax error at line %d: %s",lineno,message);
  Error = TRUE;
}

/* This fuction can be called by the following fuctions. */
static void match(TokenType expected) 
{ if (token == expected) token = getToken();
  else {
    syntaxError("unexpected token -> ");
    printToken(token,tokenString);
    fprintf(listing,"      ");
  }
}


/* Each of the following functions represents the process of put one certain non-terminal character on the syntex tree as a node: */
/* You can write other functions referring to the format of the function  stmt_sequence(void). */

//stmt_seq->stmt{stmt}
TreeNode * stmt_sequence(void)
{ TreeNode * t = statement();
  TreeNode * p = t;
  while ((token!=ENDFILE) && (token!=END) &&
         (token!=ELSE) && (token!=UNTIL))
  { TreeNode * q;
    //match(SEMI);
    q = statement(); 
    if (q!=NULL) {
      if (t==NULL) t = p = q;
      else /* now p cannot be NULL either */
      { p->sibling = q;
        p = q;
      }
    }
  }
  return t;
}

// stmt->if_stmt | repeat_stmt | assign_stmt | read_stmt | write_stmt | 
/*FIRST(stmt) = IF REPEAT ID READ WRITE
FIRST(if_stmt) = IF
FIRST(repeat_stmt) = REPEAT
FIRST(assign_stmt) = ID
FIRST(read_stmt) = READ
FIRST(write_stmt) = WRITE*/
TreeNode * statement(void)
{
  TreeNode * tmp = NULL;
  switch (token)
  {
  case IF:
    tmp = if_stmt();break;
  case REPEAT:
    tmp = repeat_stmt();break;
  case ID:
    tmp = assign_stmt();break;
  case READ:
    tmp = read_stmt();break;
  case WRITE:
    tmp = write_stmt();break;  
  default:
    syntaxError("unexpected token");
    printToken(token, tokenString);
    match(token);break;
  }
  return tmp;
}

//repeat_stmt->REPEAT stmt_seq UNTIL exp
TreeNode * repeat_stmt(void)
{
  TreeNode* tmp = newStmtNode(RepeatK); //new repeat node
  if(tmp == NULL) {syntaxError("Can not create new stmtnode.");return NULL;}
  match(REPEAT);
  tmp->child[0] = stmt_sequence();
  match(UNTIL);
  tmp->child[1] = myexp();
  return tmp;
}

//if_stmt->IF exp THEN stmt_seq END | IF exp THEN stmt_seq ELSE stmt_seq END
/*if_stmt->IF exp THEN stmt_seq tmp
  tmp->END | ELSE stmt_seq END
*/
TreeNode * if_stmt(void)
{ 
  TreeNode* tmp = newStmtNode(IfK);
  if(tmp == NULL) {syntaxError("Can not create new stmtnode.");return NULL;}
  match(IF);

  tmp->child[0] = myexp();

  match(THEN);

  tmp->child[1] = stmt_sequence();//

  if(token == END)
    match(END);
  else if(token == ELSE)
  {
    match(ELSE);
    tmp->child[2] = stmt_sequence();
    match(END);
  }
  return tmp;
}

//assign_stmt->ID:=exp
TreeNode * assign_stmt(void)
{ 
  TreeNode* tmp = newStmtNode(AssignK);
  if(tmp == NULL) {syntaxError("Can not create new stmtnode.");return NULL;}
  if(token == ID) tmp->attr.name = copyString(tokenString);
  match(ID);
  match(ASSIGN);
  tmp->child[0] = myexp();
  return tmp;
}

//read_stmt->READ ID
TreeNode * read_stmt(void)
{ 
  TreeNode* tmp = newStmtNode(ReadK);
  if(tmp == NULL) {syntaxError("Can not create new stmtnode.");return NULL;}
  match(READ);
  if(token == ID) tmp->attr.name = copyString(tokenString);
  match(ID);
  return tmp;
}

//write_stmt->WRITE exp
TreeNode * write_stmt(void)
{ 
  TreeNode* tmp = newStmtNode(WriteK);
  if(tmp == NULL) {syntaxError("Can not create new stmtnode.");return NULL;}
  match(WRITE);
  tmp->child[0] = myexp();
  return tmp;
}

//exp->simple_exp<simple_exp | simple_exp=simple_exp | simple_exp
/*exp->simple_exp tmp
  tmp-> < simple_exp | = simple_exp | epsilon
  follow(exp) = follow(tmp) = ) THEN + follow(stmt_seq)
  follow(stmt_seq) = first(stmt) + end until else $
  first(stmt) = IF REPEAT ID READ WRITE
*/
TreeNode * myexp(void)
{ 
  TreeNode* tmp = simple_exp();
  if( token == LT || token == EQ)
  {
    TreeNode* tmp1 = newExpNode(OpK);
    if(tmp1 == NULL)  {syntaxError("Can not create new expnode."); return NULL;}
    tmp1->attr.op = token;
    tmp1->child[0] = tmp;
    tmp = tmp1;
    match(token);
    tmp->child[1] = simple_exp();
    return tmp;
  }
  else if (token != RPAREN && token != THEN && token != END && token != UNTIL
          && token != ELSE && token != ENDFILE && token != IF && token != REPEAT
          && token != ID && token != READ && token != WRITE)
  {
    syntaxError("unexpected token");
    printToken(token, tokenString);
    match(token);
  }
  return tmp;
}

//simple_exp->simple_exp+term | simple_exp-term | term
/*simple_exp-> simple_exp tmp | term
  tmp-> + term | - term*/
/*simple_exp-> term { +|- term }*/
TreeNode * simple_exp(void)
{
  TreeNode* tmp = term();
  while (token == PLUS || token == MINUS)
  {
    TreeNode* tmp1 = newExpNode(OpK);
    if(tmp1 == NULL) {syntaxError("Can not create new expnode."); return NULL;}
    tmp1->child[0] = tmp;
    tmp1->attr.op = token;
    tmp = tmp1;
    match(token);
    tmp->child[1] = term();
  }
  return tmp;
}

//term->term*factor | term/factor | factor
/*term->factor{ *|/ factor }*/
TreeNode * term(void)
{ 
  TreeNode* tmp = factor();
  while (token == TIMES || token == OVER)
  {
    TreeNode* tmp1 = newExpNode(OpK);
    if(tmp1 == NULL) {syntaxError("Can not create new expnode."); return NULL;}
    tmp1->child[0] = tmp;
    tmp1->attr.op = token;
    tmp = tmp1;
    match(token);
    tmp1->child[1] = factor();
  }
  return tmp;
}

//factor->(exp) | NUM | ID 
TreeNode * factor(void)
{ 
  if(token == LPAREN)
  {
    match(LPAREN);
    TreeNode* tmp = myexp();
    match(RPAREN);
    return tmp;
  }
  else if (token == NUM)
  {
    TreeNode* tmp = newExpNode(ConstK);
    if(tmp == NULL) {syntaxError("Can not create new expnode."); return NULL;}
    tmp->attr.val = atoi(tokenString);
    match(NUM);
    return tmp;
  }
  else if (token == ID)
  {
    TreeNode* tmp = newExpNode(IdK);
    if(tmp == NULL) {syntaxError("Can not create new expnode."); return NULL;}
    tmp->attr.name = copyString(tokenString);
    match(ID);
    return tmp;
  }
  else
  {
    syntaxError("unexpected token -> ");
    printToken(token,tokenString);
    match(token);
    return NULL;
  }
}


/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
TreeNode * parse(void)
{ TreeNode * t;
  token = getToken(); /*get one token each time and parse it */
  t = stmt_sequence();
  if (token!=ENDFILE)
    syntaxError("Code ends before file\n");
  return t;
}
