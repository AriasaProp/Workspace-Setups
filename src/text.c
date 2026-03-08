#include "text.h"
#include "common.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

void text_reserve(Text *txt, unsigned int need) {
  if (txt->cap >= need) return;
  while (txt->cap < need) {
    txt->cap <<= 4;
    txt->cap += 15;
  }
  txt->cstr = CAST(char*)realloc(txt->cstr, txt->cap);
  assert(txt->cstr != NULL && "Fail to allocate heap");
}

int  text_append(Text *txt, const char *fmt, ...) {
  char temp[1024];
  va_list args;
  va_start(args, fmt);
  int result = vsnprintf(temp, 1024, fmt, args);
  va_end(args);
  if (LIKELY(result >= 0)) {
    size_t ex_size = strlen(temp);
    text_reserve(txt, txt->count + ex_size + 1);
    memcpy(txt->cstr + txt->count, temp, ex_size);
    txt->count += strlen(temp);
    txt->cstr[txt->count] = 0;
  }
  return result;
}

void text_clean(Text *txt) {
  txt->count = 0;
  txt->cstr[0] = 0;
}
void text_free (Text *txt) {
  free(txt->cstr);
  memset(txt, 0, sizeof(Text));
}
