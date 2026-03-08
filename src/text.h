#ifndef _STRING_INCLUDED_
#define _STRING_INCLUDED_

typedef struct {
  char *cstr;
  unsigned int cap, count;
} Text;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void text_reserve(Text *, unsigned int);
int  text_append (Text *, const char *, ...);
void text_clean  (Text *);
void text_free   (Text *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _STRING_INCLUDED_