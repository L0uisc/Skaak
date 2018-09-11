#include "stukke.h"

Soort bepaalSoort(int i, int gelid)
{
    int ry = i % aantalRye;
    switch (gelid < 2 ? gelid : 3 - gelid)
    {
    case 0:
        switch (ry < 5 ? ry : (aantalRye - ry) - 1)
        {
        case 0:     return Soort::TORING;
        case 1:     return Soort::RUITER;
        case 2:     return Soort::LOPER;
        case 3:     return Soort::DAME;
        case 4:     return Soort::KONING;
        }
    default:        return Soort::PION;
    }
}

void inisialiseerStukke(Stuk *stukke, Posisie **posisies)
{
    for (int i { 0 }; i < aantalStukke; ++i)
    {
        int gelid { i / aantalRye };

        posisies[i] = new Posisie;
        posisies[i]->koordinaat.ry = (i % aantalRye) + 1;
        posisies[i]->koordinaat.gelid = gelid < 2 ? (gelid + 1) : gelid + 5;
        posisies[i]->stuk = &stukke[i];

        stukke[i].soort = bepaalSoort(i, gelid);
        stukke[i].isWit = (gelid < 2);
        stukke[i].posisie = posisies[i];
    }
}

void ruimStukkeOp(Posisie **posisies)
{
    for (int i { 0 }; i < aantalStukke; ++i)
        delete posisies[i];
}
