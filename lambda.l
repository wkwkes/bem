%{
#include <stdio.h>
#include "y.tab.h"

int yywrap() {
  return 1;
}
%}

%%

"lambda" return LAMBDA;
"\\"     return LAMBDA;
[a-z]+   return ID;

%%
