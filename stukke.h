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

void inisialiseerStukke(std::array<Stuk, g_aantalStukke> &stukke);
char bepaalLetter(const Stuk *stuk);
Soort bepaalSoort(char letter);

#endif // STUKKE_H
