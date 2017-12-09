#include<iostream>
#include<cstdlib>
#include<cstring>
#include<stack>
#define EVALUATE 1
#define MAXSIZE 10000000
using namespace std;

stack<int> z;
char *infix, *postfix;
char s[MAXSIZE], r[MAXSIZE];
void Term(), Element(), Component(), Item();

void error(unsigned n) {
   cerr << "Error!\n";
   exit(n);
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
#if EVALUATE
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
#endif
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
#if EVALUATE
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
#endif
   }
}
void Component() {
   int sign;
   if ((sign = *infix) == '+' || *infix == '-') {
      sign = *infix++;
      Component();
      if (sign == '-') {
#if EVALUATE
         z.top() = -z.top();
#endif
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
      *postfix++ = '^';
      *postfix++ = ' ';
#if EVALUATE
      sum = z.top();
      z.pop();
      int t = z.top();
      for (int i = 1; i <= sum; i++)
         m *= t;
#endif
   }
#if EVALUATE
   else
      m = z.top();
   z.top() = m;
#endif
}
void Element() {
  if (*infix == '(') {
     infix++;
     Formula();
     if (*infix++ != ')') error(1);
  }
  else {
     char *p = infix;
     int sum = 0;
     if (*infix < '0' || *infix > '9') error(2);
     while (*infix >= '0' && *infix <= '9') {
        sum = sum*10 + *infix - '0';
        *postfix++ = *infix++;
     }
     *postfix++ = ' ';
#if EVALUATE
     z.push(sum);
#endif
  }
}
int main() {
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
#if EVALUATE
   cout << "The result of the formula evaluation: " << z.top() << endl;
#endif
   return 0;
}

