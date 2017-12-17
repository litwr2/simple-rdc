//it parses any C-expression using the plain recursive descending method
//it supports only integer numbers and simplified string literals
//it doesn't support type casts and compound terms
//all other features have to work

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<stack>
#define MAXSIZE 10000000
using namespace std;

char *infix, *postfix;
char s[MAXSIZE], r[MAXSIZE];

void expression(), assignment_expression(), unary_expression(), conditional_expression(), logical_or_expression(), logical_and_expression(), inclusive_or_expression(), exclusive_or_expression(), and_expression(), equality_expression(), relational_expression(), shift_expression(), additive_expression(), multiplicative_expression(), cast_expression(), postfix_expression(), prefix_expression(), primary_expression(), suffix_expression(), identifier(), type_name();
int argument_expression_list();

void error(unsigned n) {
   cerr << "Error!\n";
   exit(n);
}
void expression() {
   assignment_expression();
   while (*infix == ',') {
      infix++;
      assignment_expression();
      strcpy(postfix, "@pop ");
      postfix += 5;
   }
}
void assignment_expression() {
   char *save_infix = infix, *save_postfix = postfix;
   unary_expression();
   //check_lvalue();
   char sign = *infix;
   if (sign == '=' || sign && strchr("+-/*%^|&",  sign) && infix[1] == '=' || (!strncmp("<<", infix, 2) || !strncmp(">>", infix, 2)) && infix[2] == '=') {
      infix++;
      if (sign != '=')
         if (sign == '<' || sign == '>')
            infix += 2;
         else
            infix++;
      assignment_expression();
      if (sign == '=')
         strcpy(postfix, "= "), postfix += 2;
      else if (sign == '<' || sign == '>')
         sprintf(postfix, "%c%c= ", sign, sign), postfix += 4;
      else
         sprintf(postfix, "%c= ", sign), postfix += 3;
   }
   else {
      infix = save_infix;
      postfix = save_postfix;
      conditional_expression();
   }
}
void conditional_expression() {
   logical_or_expression();
   if (*infix == '?') {
      infix++;
      strcpy(postfix, "( ");
      postfix += 2;
      expression();
      strcpy(postfix, ") ( ");
      postfix += 4;
      if (*infix++ == ':')
         conditional_expression();
      else
         throw 3;
      strcpy(postfix, ") @ifelse ");
      postfix += 10;
   }
}
void logical_or_expression() {
   logical_and_expression();
   while (!strncmp(infix, "||", 2)) {
      infix += 2;
      strcpy(postfix, "( ");
      postfix += 2;
      logical_and_expression();
      strcpy(postfix, ") @if_or ");
      postfix += 9;
   }   
}
void logical_and_expression() {
   inclusive_or_expression();
   while (!strncmp(infix, "&&", 2)) {
      infix += 2;
      strcpy(postfix, "( ");
      postfix += 2;
      inclusive_or_expression();
      strcpy(postfix, ") @if_and ");
      postfix += 10;
   }   
}
void inclusive_or_expression() {
   exclusive_or_expression();
   while (*infix == '|' && infix[1] != '|') {
      infix++;
      exclusive_or_expression();
      *postfix++ = '|';
      *postfix++ = ' ';
   }   
}
void exclusive_or_expression() {
   and_expression();
   while (*infix == '^') {
      infix++;
      and_expression();
      *postfix++ = '^';
      *postfix++ = ' ';
   }   
}
void and_expression() {
   equality_expression();
   while (*infix == '&' && infix[1] != '&') {
      infix++;
      equality_expression();
      *postfix++ = '&';
      *postfix++ = ' ';
   }   
}
void equality_expression() {
   relational_expression();
   while (!strncmp(infix, "!=", 2) || !strncmp(infix, "==", 2)) {
      char sign = *infix;
      infix += 2;
      relational_expression();
      sprintf(postfix, "%c= ", sign);
      postfix += 3;
   }   
}
void relational_expression() {
   char sign, len;
   shift_expression();
   while ((sign = *infix) == '<' || sign == '>') {
      if (infix[1] == '=')
         infix += len = 3;
      else
         infix++, len = 2;
      shift_expression();
      if (len == 3)
         sprintf(postfix, "%c= ", sign);
      else
         sprintf(postfix, "%c ", sign);
      postfix += len;
   }   
}
void shift_expression() {
   additive_expression();
   while (!strncmp(infix, "<<", 2) || !strncmp(infix, ">>", 2)) {
      char sign = *infix;
      infix += 2;
      additive_expression();
      sprintf(postfix, "%c%c ", sign, sign);
      postfix += 3;
   }   
}
void additive_expression() {
   char sign;
   multiplicative_expression();
   while ((sign = *infix) == '+' || sign == '-') {
      infix++;
      multiplicative_expression();
      *postfix++ = sign;
      *postfix++ = ' ';
   }   
}
void multiplicative_expression() {
   char sign;
   cast_expression();
   while ((sign = *infix) && strchr("*/%", sign)) {
      infix++;
      cast_expression();
      *postfix++ = sign;
      *postfix++ = ' ';
   }   
}
void cast_expression() {
#if 0
   char *save_infix = infix, *save_postfix = postfix;
   try {
      if (*infix == '(') {
         infix++;
         type_name();
         if (*infix++ != ')')
            throw 4;
         cast_expression();
         return;
      }
   }
   catch (...) {
      infix = save_infix;
      postfix = save_postfix;
   }
#endif
   unary_expression();
}
void unary_expression() {
   char sign = *infix;
   if (!strncmp(infix, "+@", 2) || !strncmp(infix, "-@", 2)) {
      infix += 2;
      unary_expression();
      sprintf(postfix, "@prefix%c%c ", sign, sign);
      postfix += 10;
   }
   else if (sign && strchr("+-*&!~", sign)) {
      infix++;
      cast_expression();
      sprintf(postfix, "@unary%c ", sign);
      postfix += 8;
   }
   else if (!strncmp(infix, "sizeof(", 7)) {
      infix += 7;
      type_name();
      if (*infix++ != ')')
         throw 5;
      strcpy(postfix, "@sizeof ");
      postfix += 8;
   }
   else if (!strncmp(infix, "sizeof", 6)) {
      infix += 6;
      unary_expression();
      strcpy(postfix, "@sizeof ");
      postfix += 8;
   }
   else
      postfix_expression();
}
void postfix_expression() {
   primary_expression();
   suffix_expression();
}
void suffix_expression() {
   char sign = *infix;
   if (!strncmp(infix, "()", 2)) {
      infix += 2;
      strcpy(postfix, "@0 ");
      postfix += 3;     
   }
   else if (*infix == '(') {
      infix++;
      int n = argument_expression_list();
      *postfix++ = '@'; //number of arguments
      char b[20];
      sprintf(b, "%d ", n);
      strcpy(postfix, b);
      postfix += strlen(b);
      if (*infix++ != ')')
         throw 6;
   }
   else if (*infix == '[') {
      infix++;
      expression();
      if (*infix++ != ']')
         throw 7;
      strcpy(postfix, "[] ");
      postfix += 3;
   }
   else if (!strncmp(infix, "->", 2)) {
      infix += 2;
      identifier();
      strcpy(postfix, "-> ");
      postfix += 3;
   }
   else if (!strncmp(infix, "+@", 2) || !strncmp(infix, "-@", 2)) {
      infix += 2;
      sprintf(postfix, "@postfix%c%c ", sign, sign);
      postfix += 11;
   }
   else if (sign == '.') {
      infix++;
      identifier();
      *postfix++ = '.';
      *postfix++ = ' ';
   }
   else
      return;
   suffix_expression();
}
int argument_expression_list() {
   int n = 0;
   assignment_expression();
   n++;
   while (*infix == ',') {
      infix++;
      assignment_expression();
      n++;
   }
   return n;
}
void primary_expression() {
   char sign = *infix;
   if (sign == '(') {
      infix++;
      expression();
      if (*infix++ != ')')
         throw 7;
   }
   else if (sign == '"') { //simplified string literal
      infix++;
      char *p = strchr(infix, '"'), *q;
      for (q = infix - 1; q != p + 1; q++)
         *postfix++ = *q;
      *postfix++ = ' ';
      infix = q;
   }
   else if (sign <= '9' && sign >= '0') {
      int n = strspn(infix, "0123456789");
      for (int i = 0; i < n; i++)
         *postfix++ = *infix++; //number
      *postfix++ = ' ';
   }
   else
      identifier();
}
void identifier() {
   char sign = *infix;
   if (sign <= 'Z' && sign >= 'A' || sign <= 'z' && sign >= 'a' || sign == '_') {
      int n = strspn(infix, "0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm_");
      for (int i = 0; i < n; i++)
         *postfix++ = *infix++; //id
      *postfix++ = ' ';
   }
   else
      throw 9;
}
void type_name() {
   identifier(); //simplification
}
int main() {
   cout << "Enter a C-formula in infix notation: ";
   cin.getline(s, MAXSIZE);
   for (int i = 0; s[i]; i++) //replaces ++ and -- with +@ and -@
      if (s[i] == '+' && s[i + 1] == '+') {
         s[i] = '+';
         s[++i] = '@';
      }
      else if (s[i] == '-' && s[i + 1] == '-') {
         s[i] = '-';
         s[++i] = '@';
      }
   for (int i = 0; s[i]; i++) //removes spaces
      if (strchr(" \t", s[i])) {
         s[i] = 0;
         strcat(s, s + i + 1);
      }
   infix = s;
   postfix = r;
   expression();
   *postfix = 0;
   cout << "The equivalent of formula in the postfix form: " << r << endl;
   return 0;
}

