#ifndef _DATE_PARSER_INCLUDED_
#define _DATE_PARSER_INCLUDED_

#include "text.h"
#include <unistd.h>

// start from Jan 1, 1969
typedef struct {
  uint8_t date, week, month;
  uint16_t year;
} greg;
// start either Jan 1, 1970 or 2000
// store full year 360 < days
typedef struct {
  uint8_t wewaran[10], wuku;
  uint16_t year;
} pawukon;
typedef struct {
  struct {
    uint8_t dayIndex, ngunaratri;
  } date;
  uint8_t sasih;
  uint16_t year;
} caka;
typedef struct {
  uint8_t hour, minute, second;
} tclock; // a day in seconds is % (86400)

#ifdef __cplusplus
extern "C" {
#endif

tclock dp_getClock();

greg dp_getGreg();
uint8_t dp_getGreg_date(const greg);
void dp_getGreg_txtWeek   (Text*,const greg);
void dp_getGreg_txtMonth  (Text*,const greg);
void dp_getZodiac         (Text*,const greg);

pawukon dp_getPawukon();
pawukon dp_getPawukon_since(unsigned int);
void    dp_getPawukon_txtWewaran(Text*,uint8_t, const pawukon);
void    dp_getPawukon_txtWuku   (Text*,const pawukon);
uint8_t dp_getPawukon_uripWuku  (const pawukon);
uint8_t dp_getPawukon_ekaJalaRsi(const pawukon);

caka dp_getCaka();
caka dp_getCaka_since(unsigned int);
void dp_getCaka_txtSasih    (Text*,const caka);
void dp_getCaka_txtPenanggal(Text*,const caka);



#ifdef __cplusplus
}
#endif

#endif // _DATE_PARSER_INCLUDED_
