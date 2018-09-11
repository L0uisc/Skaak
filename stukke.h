#ifndef STUKKE_H_INCLUDED
#define STUKKE_H_INCLUDED
#include "konstantes.h"

struct Posisie;

struct Stuk
{
    Soort soort;
    bool isWit;
    Posisie *posisie;
};

struct Posisie
{
    Koordinaat koordinaat;
    Stuk *stuk;
};

void inisialiseerStukke(Stuk *stukke, Posisie **posisies);
void ruimStukkeOp(Posisie **posisies);

#endif // STUKKE_H_INCLUDED
