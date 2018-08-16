#include "stukke.h"

enum class Stukke
{
    TORING,
    RUITER,
    LOPER,
    DAME,
    KONING
};

Stukke bepaalStuk(int i)
{
    int ry = i % 8;
    switch (ry < (8 - ry) ? ry : 7 - ry)
    {
        case 0: return Stukke::TORING;
        case 1: return Stukke::RUITER;
        case 2: return Stukke::LOPER;
        case 3: return Stukke::DAME;
        case 4: return Stukke::KONING;
    }
}

char bepaalSoort(int i)
{
    switch (i / 8)
    {
    case 0:
        switch (bepaalStuk(i))
        {
        case Stukke::TORING:    return 'T';
        case Stukke::RUITER:    return 'R';
        case Stukke::LOPER:     return 'L';
        case Stukke::DAME:      return 'D';
        case Stukke::KONING:    return 'K';
        }
    case 1:     return 'P';
    case 2:     return 'p';
    case 3:
        switch (bepaalStuk(i))
        {
        case Stukke::TORING:    return 't';
        case Stukke::RUITER:    return 'r';
        case Stukke::LOPER:     return 'l';
        case Stukke::DAME:      return 'd';
        case Stukke::KONING:    return 'k';
        }
    }
}

void inisialiseerStukke(Stuk *stukke, Posisie *posisies)
{
    for (int i { 0 }; i < aantalStukke; ++i)
    {
        stukke[i].soort = bepaalSoort(i);
        stukke[i].posisie = &posisies[i];
        posisies[i].ry = (i + 1) % 8;
        posisies[i].gelid = (((i / 8) < 2) ? ((i / 8) + 1) : (i / 8) + 5);
        posisies[i].stuk = &stukke[i];
    }
}
