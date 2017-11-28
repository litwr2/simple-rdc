#include<iostream>
#include<cstdlib>
#include<cstring>
#include<stack>
using namespace std;

stack<int> z;
char *infix, *postfix;
void Term(), Element(), Component(), Item();

void error() {
   cerr << "Error!\n";
   exit(1);
}
void Formula() {
   char sign;
   int sum;
   Term();
   while ((sign = *infix) == '+' || sign == '-') {
      infix++;
      Term();
      *postfix++ = sign;
      *postfix++ = ' ';
      if (sign == '+') {
         sum = z.top();
         z.pop();
         sum += z.top();
      }
      else {
         sum = z.top();
         z.pop();
         sum = z.top() - sum;
      }
      z.pop();
      z.push(sum);
   }
}
void Term() {
   char sign;
   int sum;
   Component();
   while ((sign = *infix) == '*' || sign == '/') {
      infix++;
      Component();
      *postfix++ = sign;
      *postfix++ = ' ';
      if (sign == '*') {
         sum = z.top();
         z.pop();
         sum *= z.top();
      }
      else {
         sum = z.top();
         z.pop();
         sum = z.top()/sum;
      }
      z.pop();
      z.push(sum);
   }
}
void Component() {
   int sign;
   if ((sign = *infix) == '+' || *infix == '-') {
      sign = *infix++;
      Component();
      if (sign == '-') {
         z.top() = -z.top();
         *postfix++ = '@';   //unary minus
         *postfix++ = ' ';
      }
   }
   else
      Item();
}

void Item() {
   int sum, m = 1;
   Element();
   if (*infix == '^') {
      infix++;
      Component();
      sum = z.top();
      z.pop();
      int t = z.top();
      for (int i = 1; i <= sum; i++)
         m *= t;
      *postfix++ = '^';
      *postfix++ = ' ';
   }
   else
      m = z.top();
   z.top() = m;
}
void Element() {
  if (*infix == '(') {
     infix++;
     Formula();
     if (*infix++ != ')') error();
  }
  else {
     char *p = infix;
     int sum = 0;
     if (*infix < '0' || *infix > '9') error();
     while (*infix >= '0' && *infix <= '9') {
        sum = sum*10 + *infix - '0';
        *postfix++ = *infix++;
     }
     *postfix++ = ' ';
     z.push(sum);
  }
}
#define MAXSIZE 100
main() {
   char s[MAXSIZE], r[MAXSIZE]; 
   cout << "Enter a formula in infix notation: ";
   cin.getline(s, MAXSIZE);
   for (int i = 0; s[i]; i++) //removes spaces
      if (strchr(" \t", s[i])) {
         s[i] = 0;
         strcat(s, s + i + 1);
      }
   infix = s;
   postfix = r;
   Formula();
   *postfix = 0;
   cout << "The equivalent of formula in the postfix form: " << r << endl;
   cout << "The result of the formula evaluation: " << z.top() << endl;
}

