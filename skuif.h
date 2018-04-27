#ifndef SKUIF_H
#define SKUIF_H
#include "bord.h"

void doenSkuif(const bool isWit, char stukke[sylengte][sylengte]);
void skryfSkuif(const bool isWit, const std::string skuif);

std::string krySkuif(bool isWit, char stukke[sylengte][sylengte]);

bool isSpelVerby(bool isWit, const char stukke[sylengte][sylengte]);

bool isWettig(std::string skuif);
bool isGeldig(std::string skuif);

#endif // SKUIF_H
