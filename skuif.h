#ifndef SKUIF_H
#define SKUIF_H

#include <vector>
#include "konstantes.h"
#include "stukke.h"
#include "bord.h"

struct Koordinaat
{
    uint_t ry;
    uint_t gelid;
};

const std::string krySkuif(bool isWit, std::array<std::array<Blokkie, g_sylengte>,
                           g_sylengte> &blokkies);
void doenSkuif(bool isWit, const std::string &skuif,
               std::array<std::array<Blokkie, g_sylengte>, g_sylengte> &bord,
               std::vector<const Stuk*> &gebuiteStukke);
void skryfSkuif(bool isSwart, const std::string &skuif);
bool isOorgee(const std::string &skuif);
bool isSkaak(char skaak);
bool isMat(const std::string &mat);
bool isPat(const std::string &pat);

#endif // SKUIF_H
