#include <stdio.h>
#define _PARAM_PARSER_IMPLEMENTATION_
#include "param_parser.h"

static const char *help_msg =
"Usage: sched <cmd>\n"
"COMMAND   Description\n"
"help      show this message.\n"
"adde      add event to schedule list.\n"
"rmve      remove event from schedule list.\n"
"show      show all schedule listed.\n"
;
static char temp[sizeof(int)==4?256:-1];

int main (int argc, char **argv) {
  (void)temp;
#define CS(A,B,C,D) (int)(A|B<<8|C<<16|D<<24)
  int cmd = CS('s','h','o','w');
  if (argc >= 2) {
    param t = {.c=argc, .p=argv};
    // executable name
    param_shift(&t);
    cmd = *(int*)param_c_shift(&t);
  }
  switch (cmd) {
    default:
      memcpy(temp, &cmd, sizeof(int));
      temp[sizeof(int)] = 0;
      printf("Unknown %s command!\n", temp);
    case CS('h','e','l','p'):
      printf("%s", help_msg);
      break;
    case CS('a','d','d','e'):
      printf("Doing added schedule\n");
      break;
    case CS('r','m','v','e'):
      printf("Doing remove schedule\n");
      break;
    case CS('s','h','o','w'):
      printf("Doing show schedule\n");
      break;
  }
#undef CS
  return EXIT_SUCCESS;
}
