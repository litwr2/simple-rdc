#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<ctime>
using namespace std;

#define WIDENESS 3
#define MAXSIZE 10000

string s;
void Term(), Element(), Component(), Item();

void error() {
   cerr << "Error!\n";
   exit(1);
}
void Formula() {
   unsigned rndv = rand()%WIDENESS + 1;
   Term();
   for (int i = 0; i < rndv; i++) {
      if (rand()%2) s += '+'; else s += '-';
      Term();
   }
}
void Term() {
   unsigned rndv = rand()%WIDENESS + 1;
   Component();
   for (int i = 0; i < rndv; i++) {
      if (rand()%2) s += '*'; else s += '/';
      Component();
   }
}
void Component() {
   unsigned rndv = rand()/2 + 1;
   if (rndv%5 == 4) {
      s += '+';
      Component();
   }
   else if (rndv%3 > 1) {
      s += '-';
      Component();
   }
   else
      Item();
}
void Item() {
   Element();
   if (rand()%2) {
      s += '^';
      Component();
   }
}
void Element() {
  if (rand()%2 && s.length() < MAXSIZE) {
      s += '(';
      Formula();
      s += ')';
  }
  else
      s += to_string(rand()%8);
}
int main() {
   srand(time(0));
   Formula();
   cout << s << endl;
   return 0;
}

