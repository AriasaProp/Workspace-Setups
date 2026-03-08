#include "date_parser.h"
#include "string.h"

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
  Text gtxt = {0};
  int i;
  printf("Version 0.0.4\n");
  {
    greg g = dp_getGreg();
    dp_getGreg_txtWeek(&gtxt, g);
    text_append(&gtxt, ", ");
    dp_getGreg_txtMonth(&gtxt, g);
    printf("Hello in %s %hu, %u \n", gtxt.cstr, dp_getGreg_date(g), g.year);
    text_clean(&gtxt);
    dp_getZodiac(&gtxt, g);
    printf("Zodiac: %s\n", gtxt.cstr);
  }
  text_clean(&gtxt);
  printf("Pawukon\n");
  {
    pawukon p = dp_getPawukon_since(0);
    printf("1970\n");
    dp_getPawukon_txtWuku(&gtxt, p);
    printf("Wuku: %s || Tolu\n", gtxt.cstr);
    text_clean(&gtxt);
    for (i = 0; i < 10; ++i) {
      dp_getPawukon_txtWewaran(&gtxt, i, p);
      text_append(&gtxt,",");
    }
    printf(
      "Wewaran:%s\n"
      "Wewaran:-,Menga,Kajeng,Sri,Wage,Urukung,Wrespati,Sri,Gigis,Suka,\n",
      gtxt.cstr
    );
    text_clean(&gtxt);
    printf("2000\n");
    p = dp_getPawukon_since(10957);
    dp_getPawukon_txtWuku(&gtxt, p);
    printf("Wuku: %s || Bala\n", gtxt.cstr);
    text_clean(&gtxt);
    for (i = 0; i < 10; ++i) {
      dp_getPawukon_txtWewaran(&gtxt, i, p);
      text_append(&gtxt,",");
    }
    printf(
      "Wewaran:%s\n"
      "Wewaran:-,Menga,Pasah,Laba,Umanis,Paniron,Saniacara,Brahma,Nohan,Sri\n",
      gtxt.cstr
    );
    text_clean(&gtxt);
  }
  text_clean(&gtxt);
  printf("\n");
  {
    caka c = dp_getCaka();
    printf("Caka\n");
    dp_getCaka_txtSasih(&gtxt, c);
    text_append(&gtxt, ": ");
    dp_getCaka_txtPenanggal(&gtxt, c);
    text_append(&gtxt, " %hu", c.year);
    printf("Now -> %s\n", gtxt.cstr);
    text_clean(&gtxt);
    
    c = dp_getCaka_since(0);
    dp_getCaka_txtSasih(&gtxt, c);
    text_append(&gtxt, ": ");
    dp_getCaka_txtPenanggal(&gtxt, c);
    text_append(&gtxt, " %hu", c.year);
    printf("1970 -> %s\n", gtxt.cstr);
    text_clean(&gtxt);
    
    c = dp_getCaka_since(10957);
    dp_getCaka_txtSasih(&gtxt, c);
    text_append(&gtxt, ": ");
    dp_getCaka_txtPenanggal(&gtxt, c);
    text_append(&gtxt, " %hu", c.year);
    printf("2000 -> %s\n", gtxt.cstr);
    text_clean(&gtxt);
    
  }
  text_free(&gtxt);
  return 0;
}
