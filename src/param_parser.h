#ifndef _PARAM_PARSER_INCLUDED_
#define _PARAM_PARSER_INCLUDED_

typedef struct {
  int c;
  char **p;
} param;

#ifdef __cplusplus
extern "C" {
#endif
// reduce counter
int param_shift(param*);
// get parameter char* and reduce counter, return NULL when counter passed
char *param_c_shift(param*);
// get parameter int and reduce counter, return 0 when counter passed or value invalid
int  param_i_shift(param*);


#ifdef __cplusplus
} 
#endif

#endif // _PARAM_PARSER_INCLUDED_
#ifdef _PARAM_PARSER_IMPLEMENTATION_
#include <stdio.h>
#include <stdlib.h>


int param_shift(param *p) {
  if (p->c > 0) {
    p->p++;
    return (p->c)--;
  }
  return 0;
}
char *param_c_shift(param *p) {
  if (p->c > 0) {
    (p->c)--;
    return *(p->p++);
  }
  return NULL;
}
int  param_i_shift(param *p) {
  if (!p->c) return 0;
  (p->c)--;
  return atoi(*(p->p++));
}
#endif // _PARAM_PARSER_IMPLEMENTATION_
