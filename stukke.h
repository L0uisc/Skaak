#ifndef STUKKE_H_INCLUDED
#define STUKKE_H_INCLUDED
#include "konstantes.h"

struct Posisie;

struct Stuk
{
    char soort;
    Posisie *posisie;
};

struct Posisie
{
    int ry;
    int gelid;
    Stuk *stuk;
};

void inisialiseerStukke(Stuk *stukke, Posisie *posisies);

#endif // STUKKE_H_INCLUDED
