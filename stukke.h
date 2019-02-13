#ifndef STUKKE_H
#define STUKKE_H

#include <array>
#include "konstantes.h"

enum class Soort
{
    TORING,
    RUITER,
    LOPER,
    DAME,
    KONING,
    PION,
    ONBEKEND
};

struct Stuk
{
    Soort soort;
    bool isWit;
};

struct Blokkie;

using stukArray_t = std::array<Stuk, g_aantalStukke>;
using bordArray_t = std::array<std::array<Blokkie, g_sylengte>, g_sylengte>;

void inisialiseerStukke(stukArray_t &stukke);
char bepaalLetter(const Stuk* const stuk);
Soort bepaalSoort(char letter);

#endif // STUKKE_H
