#ifndef BORD_H
#define BORD_H

#include "konstantes.h"
#include "stukke.h"
#include <array>

struct Blokkie
{
    Stuk *stuk;
    char agtergrond;
};

void inisialiseerBord(bordArray_t &bord, stukArray_t &stukke);

void tekenBord(bool isWit, bordArray_t &bord);

#endif // BORD_H
