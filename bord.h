#ifndef BORD_H
#define BORD_H

#include <array>
#include "konstantes.h"
#include "stukke.h"

struct Blokkie
{
    char agtergrond;
    Stuk *stuk;
};

void inisialiseerBord(std::array<std::array<Blokkie, g_sylengte>,g_sylengte> &bord,
                      std::array<Stuk, g_aantalStukke> &stukke);

void tekenBord(bool isWit, std::array<std::array<Blokkie, g_sylengte>,
               g_sylengte> &bord);

#endif // BORD_H
