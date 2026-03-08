#include "date_parser.h"
#include "common.h"
#include "text.h"
#include <time.h>

// zero counted since Jan 1, 1969
// cause leap year become last each 4 years
#define WEEK_COUNT 7
#define MONTH_COUNT 12
#define ZODIAC_COUNT 12
#define DAYS_IN_YEAR 365
#define DAYS_IN_LEAPYEAR 366
#define DAYS_IN_4YEAR 1461

#define WUKU_COUNT 30
#define SASIH_COUNT 12

static const char *GregWeekNames[WEEK_COUNT] = {
  "Wednesday", "Thurstday", "Friday", "Saturday", "Sunday", "Monday", "Tuesday", 
};
static const char *GregMonthNames[MONTH_COUNT] = {
  "January", "February" , "March","April",
  "May","June","July", "August",
  "September", "October", "November", "December"
};
static const char *ZodiacNames[ZODIAC_COUNT] = {
  "Capricon", "Aquarius", "Pisces", "Aries", "Taurus", "Gemini", "Cancer", "Leo", "Virgo", "Libra", "Scorpio", "Sagitarius",
};
static const char **PawukonWewaranNames[10] = {
  (const char *[2]) {"-", "Luang"},
  (const char *[2]) {"Menga", "Pepet"},
  (const char *[3]) {"Pasah", "Beteng", "Kajeng"},
  (const char *[4]) {"Sri","Laba","Jaya","Menala",},
  (const char *[5]) {"Paing", "Pon", "Wage", "Keliwon", "Umanis"},
  (const char *[6]) {"Tungleh","Aryang", "Urukung", "Paniron", "Was", "Maulu"},
  (const char *[7]) {"Redite", "Coma", "Anggara", "Buda", "Wrespati", "Sukra", "Saniscara"},
  (const char *[8]) {"Sri","Indra","Guru","Yama","Ludra","Brahma","Kala","Uma"},
  (const char *[9]) {"Dangu","Jangur","Gigis","Nohan","Ogan","Erangan","Urungan","Tulus","Dadi"},
  (const char *[10]){"Raksasa","Pandita","Pati","Suka","Duka","Sri","Manuh","Manusa","Raja","Dewa"},
};
static const uint8_t *PawukonWewaranUrip[10] = {
  NULL,
  NULL,
  NULL,
  NULL,
  (const uint8_t[5]) {9,7,4,8,5},
  NULL,
  (const uint8_t[7]) {5,4,3,7,8,6,9},
  NULL,
  NULL,
  NULL,
};
static const char *PawukonWukuNames[WUKU_COUNT] = {
  "Sinta", "Landep", "Wukir", "Kurantil",
  "Tolu", "Gumbreg", "Warigalit", "Wariagung",
  "Julungwangi", "Sungsang", "Galungan", "Kuningan",
  "Langkir", "Mandasiya", "Julungpujut", "Pahang",
  "Kuruwelut", "Marakeh", "Tambir", "Medangkungan",
  "Maktal", "Wuye", "Manahil", "Prangbakat",
  "Bala", "Wugu","Wayang", "KulawuAWU","Dukut","Watugunung"
};
static const unsigned int PawukonWukuUrip[WUKU_COUNT] = {
  7,1,4,6,5,8,9,3,7,1,4,6,5,8,9,3,7,1,4,6,5,8,9,3,7,1,4,6,5,8
};
// used moon cycle that ~29 days a month
static const char *CakaSasihNames[SASIH_COUNT] = {
  "Kadasa", "Jyestha", "Sadha", "Kasa", "Karo", "Katiga", "Kapat", "Kalima", "Kanem", "Kapitu", "Kawalu", "Kasanga", 
};

static inline time_t dp__gettime() {
  static struct timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts)) {
    // TODO: ERROR handle
    return 0;
  }
  return ts.tv_sec;
}
static inline time_t dp__getday() {
  return dp__gettime() / 86400;
}


tclock dp_getClock() {
  tclock ret;
  unsigned int cl = dp__gettime() % 86400;
  ret.second = cl % 60;
  ret.minute = cl % 60;
  ret.hour = cl;
  return ret;
}

greg dp_getGreg() {
  greg ret = {0};
  unsigned int ts = dp__getday();
  if (ts) {
    // system using since Jan 1, 1970
    // shift 1970 into 1969 to match last leap year
    ts += 365;
    // modulo it to 4 year (1 in 4 was 366 days a year)
    // modulo 4 year
    ret.week = ts % WEEK_COUNT;
    div_t ld = div(ts, DAYS_IN_4YEAR);
    ret.year = 1969 + ld.quot * 4;
    ts = ld.rem;
    int leap = 0;
    if (ts >= (DAYS_IN_YEAR * 3)) {
      leap = 1;
      ts -= DAYS_IN_YEAR * 3;
      ret.year += 3;
    } else {
      ts %= DAYS_IN_YEAR;
      ret.year += ts / DAYS_IN_YEAR;
    }
    uint8_t monthL[12] = {31, 28 + leap, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    while (ret.month < 12) {
      if (monthL[ret.month] > ts) break;
      ts -= monthL[ret.month++];
    }
    ret.date = ts;
  }
  return ret;
}
uint8_t dp_getGreg_date(const greg g) {
  return g.date + 1;
}
void dp_getGreg_txtWeek(Text *txt, const greg g) {
  text_append(txt, "%s", GregWeekNames[g.week]);
}
void dp_getGreg_txtMonth(Text *txt, const greg g) {
  text_append(txt, "%s", GregMonthNames[g.month]);
}
void dp_getZodiac(Text *txt, const greg g) {
  static const int lr[12] = {20, 19, 20, 19, 20, 20, 21, 21, 21, 21, 20, 20};
  text_append(txt, "%s", ZodiacNames[(g.month + (g.date > lr[g.month])) % 12]);
}

/* 
 * pawukon is 210 days cycled
 */
pawukon dp_getPawukon() {
  return dp_getPawukon_since(dp__getday());
}
pawukon dp_getPawukon_since(unsigned int days) {
  pawukon p = {0};
  //if (!days)  return p;
  // wewaran since Nov 30, 1969
  div_t ld = div(days + 32, 210);
  p.year = ld.quot;
  int wn = ld.rem; // cycle of 210 days
  ld = div(wn, 7);
  p.wuku = ld.quot;
  
  p.wewaran[2] = wn % 3;
  p.wewaran[3] = (wn < 70 ? wn : (wn > 72 ? wn + 6 : 2)) % 4;
  p.wewaran[4] = wn % 5;
  p.wewaran[5] = wn % 6;
  p.wewaran[6] = ld.rem;
  p.wewaran[1] = p.wewaran[0] = (PawukonWewaranUrip[4][p.wewaran[4]] + PawukonWewaranUrip[6][p.wewaran[6]]) % 2;
  p.wewaran[7] = (wn < 70 ? wn : (wn > 73 ? wn + 6 : 6)) % 8;
	p.wewaran[8] = (wn > 4) * ((wn + 6) % 9);
  p.wewaran[9] = (PawukonWewaranUrip[4][p.wewaran[4]] + PawukonWewaranUrip[6][p.wewaran[6]] + 1) % 10;
  
  return p;
}
void dp_getPawukon_txtWewaran(Text *txt, uint8_t type, const pawukon c) {
  text_append(txt, "%s", PawukonWewaranNames[type][c.wewaran[type]]);
}
void dp_getPawukon_txtWuku(Text *txt, const pawukon c) {
  text_append(txt, "%s", PawukonWukuNames[c.wuku]);
}
uint8_t dp_getPawukon_uripWuku  (const pawukon p) {
  return PawukonWukuUrip[p.wuku];
}
uint8_t dp_getPawukon_ekaJalaRsi(const pawukon p) {
  switch(p.wuku) {
    case 0: switch(p.wewaran[6]) {
      default: return 0;
      case 0: case 4: case 5: return 24;
      case 2: case 6: return 18;
      case 1: case 3: return 8;
    }
    case 1: switch(p.wewaran[6]) {
      default: return 0;
      case 0 : return 10;
      case 1 : return 8;
      case 2 : return 14;
      case 3 : return 27;
      case 4 : return 25;
      case 5 : return 24;
      case 6 : return 21;
    }
    case 2: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 14;
      case 1 : return 8;
      case 2 : return 14;
      case 3 : return 26;
      case 4 : return 20;
      case 5 : return 6;
      case 6 : return 3;
    }
    case 3: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 15;
      case 1 : return 27;
      case 2 : return 18;
      case 3 : return 21;
      case 4 : return 26;
      case 5 : return 23;
      case 6 : return 1;
    }
    case 4: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 11;
      case 1 : return 6;
      case 2 : return 16;
      case 3 : return 24;
      case 4 : return 8;
      case 5 : return 18;
      case 6 : return 24;
    }
    case 5: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 18;
      case 1 : return 26;
      case 2 : return 5;
      case 3 : return 24;
      case 4 : return 3;
      case 5 : return 3;
      case 6 : return 3;
    }
    case 6: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 13;
      case 1 : return 13;
      case 2 : return 5;
      case 3 : return 15;
      case 4 : return 13;
      case 5 : return 27;
      case 6 : return 27;
    }
    case 7: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 2;
      case 1 : return 24;
      case 2 : return 24;
      case 3 : return 16;
      case 4 : return 26;
      case 5 : return 16;
      case 6 : return 6;
    }
    case 8: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 10;
      case 1 : return 26;
      case 2 : return 19;
      case 3 : return 26;
      case 4 : return 12;
      case 5 : return 16;
      case 6 : return 22;
    }
    case 9: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 26;
      case 1 : return 26;
      case 2 : return 13;
      case 3 : return 1;
      case 4 : return 18;
      case 5 : return 14;
      case 6 : return 1;
    }
    case 10: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 16;
      case 1 : return 24;
      case 2 : return 13;
      case 3 : return 8;
      case 4 : return 17;
      case 5 : return 26;
      case 6 : return 19;
    }
    case 11: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 25;
      case 1 : return 13;
      case 2 : return 13;
      case 3 : return 6;
      case 4 : return 8;
      case 5 : return 6;
      case 6 : return 27;
    }
    case 12: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 8;
      case 1 : return 6;
      case 2 : return 13;
      case 3 : return 8;
      case 4 : return 26;
      case 5 : return 3;
      case 6 : return 13;
    }
    case 13: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 26;
      case 1 : return 26;
      case 2 : return 15;
      case 3 : return 16;
      case 4 : return 27;
      case 5 : return 8;
      case 6 : return 13;
    }
    case 14: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 21;
      case 1 : return 8;
      case 2 : return 6;
      case 3 : return 26;
      case 4 : return 26;
      case 5 : return 6;
      case 6 : return 14;
    }
    case 15: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 26;
      case 1 : return 18;
      case 2 : return 14;
      case 3 : return 24;
      case 4 : return 6;
      case 5 : return 27;
      case 6 : return 21;
    }
    case 16: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 26;
      case 1 : return 26;
      case 2 : return 24;
      case 3 : return 8;
      case 4 : return 19;
      case 5 : return 19;
      case 6 : return 18;
    }
    case 17: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 8;
      case 1 : return 18;
      case 2 : return 8;
      case 3 : return 5;
      case 4 : return 27;
      case 5 : return 18;
      case 6 : return 6;
    }
    case 18: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 10;
      case 1 : return 13;
      case 2 : return 13;
      case 3 : return 14;
      case 4 : return 26;
      case 5 : return 19;
      case 6 : return 19;
    }
    case 19: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 6;
      case 1 : return 3;
      case 2 : return 26;
      case 3 : return 26;
      case 4 : return 3;
      case 5 : return 26;
      case 6 : return 18;
    }
    case 20: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 21;
      case 1 : return 15;
      case 2 : return 28;
      case 3 : return 24;
      case 4 : return 18;
      case 5 : return 9;
      case 6 : return 26;
    }
    case 21: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 18;
      case 1 : return 6;
      case 2 : return 18;
      case 3 : return 8;
      case 4 : return 7;
      case 5 : return 16;
      case 6 : return 19;
    }
    case 22: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 26;
      case 1 : return 3;
      case 2 : return 8;
      case 3 : return 14;
      case 4 : return 26;
      case 5 : return 21;
      case 6 : return 8;
    }
    case 23: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 16;
      case 1 : return 16;
      case 2 : return 24;
      case 3 : return 8;
      case 4 : return 9;
      case 5 : return 25;
      case 6 : return 3;
    }
    case 24: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 13;
      case 1 : return 10;
      case 2 : return 25;
      case 3 : return 25;
      case 4 : return 18;
      case 5 : return 25;
      case 6 : return 21;
    }
    case 25: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 8;
      case 1 : return 13;
      case 2 : return 13;
      case 3 : return 15;
      case 4 : return 19;
      case 5 : return 26;
      case 6 : return 21;
    }
    case 26: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 5;
      case 1 : return 19;
      case 2 : return 5;
      case 3 : return 21;
      case 4 : return 27;
      case 5 : return 13;
      case 6 : return 24;
    }
    case 27: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 19;
      case 1 : return 18;
      case 2 : return 18;
      case 3 : return 26;
      case 4 : return 16;
      case 5 : return 3;
      case 6 : return 25;
    }
    case 28: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 4;
      case 1 : return 3;
      case 2 : return 24;
      case 3 : return 26;
      case 4 : return 19;
      case 5 : return 26;
      case 6 : return 21;
    }
    case 29: switch (p.wewaran[6]) {
      default: return 0;
      case 0 : return 15;
      case 1 : return 4;
      case 2 : return 3;
      case 3 : return 26;
      case 4 : return 8;
      case 5 : return 26;
      case 6 : return 18;
    }
    default: return 0;
  }
  return 0;
}

caka dp_getCaka() {
  return dp_getCaka_since(dp__getday());
}
caka dp_getCaka_since(unsigned int days) {
  // counting sasih
  // 1953-1971 -> eka sungsang kliwon
  // 1971-2000 -> eka sungsang pon
  // 2000-2050 -> eka sungsang pahing
  // 9/11/2117 ->
  // wewaran since Nov 12, 1969
  caka c = {0};
  c.sasih = 6;
#define CAKA_NGUNARATRI_CYCLE 63
#define CAKA_SASIH_CYCLE 30
  if (days < 10957) { // 1 / 1 / 1970
    c.date.dayIndex = 7; // +1
    c.date.ngunaratri = 50;
    c.year = 1891;
  } else { // 1 / 1 / 2000
    days -= 10957;
    c.date.dayIndex = 9; // +1
    c.date.ngunaratri = 45;
    c.year = 1921;
  }
  div_t ld = div(c.date.ngunaratri + days, CAKA_NGUNARATRI_CYCLE);
  c.date.ngunaratri = ld.rem;
  ld = div(c.date.dayIndex + days + ld.quot, CAKA_SASIH_CYCLE);
  c.date.dayIndex = ld.rem;
#undef CAKA_NGUNARATRI_CYCLE
#undef CAKA_SASIH_CYCLE
  return c;
}
void dp_getCaka_txtSasih(Text *txt, const caka c) {
  text_append(txt, "%s", CakaSasihNames[c.sasih]);
}
void dp_getCaka_txtPenanggal(Text *txt, const caka c) {
  uint8_t p = c.date.dayIndex + 1;
  if (p > 15) {
    p -= 15;
    text_append(txt, "°");
  }
  text_append(txt, "%hhu", p);
  if (!c.date.ngunaratri)
    text_append(txt, "/%hhu", p + 1);
}
