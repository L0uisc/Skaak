#ifndef SKUIF_H
#define SKUIF_H
#include "konstantes.h"
#include "stukke.h"

void doenSkuif(bool isWit, const std::string &skuif, Stuk *stukke, Posisie **posisies);
void skryfSkuif(bool isSwart, const std::string &skuif);

const std::string valideerEnDoenSkuif(bool isWit, Stuk *stukke, Posisie **posisies);

bool isOorgee(const std::string &skuif);
bool isSkaak(char skaak);
bool isMat(const std::string &mat);
bool isPat(const std::string &pat);

#endif // SKUIF_H
