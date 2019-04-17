#ifndef SKUIF_H
#define SKUIF_H

#include "konstantes.h"
#include "stukke.h"
#include "bord.h"
#include <vector>

using buitVector_t = std::vector<const Stuk*>;

const std::string& krySkuif(bool isWit, bordArray_t &bord/*,
                            const stukArray_t &stukke*/);
void doenSkuif(bool isWit, const std::string &skuif, bordArray_t &bord,
               buitVector_t &gebuiteStukke);
void skryfSkuif(bool isSwart, const std::string &skuif);
bool isOorgee(const std::string &skuif);
bool isSkaak(char skaak);
bool isMat(const std::string &mat);
bool isPat(const std::string &pat);

#endif // SKUIF_H
