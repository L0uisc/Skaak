#include "stukke.h"

Soort bepaalSoort(uint_t i, uint_t gelid)
{
    uint_t ry = i % g_aantalRye;
    switch (gelid < 2u ? gelid : 3u - gelid)
    {
    case 0u:
        switch (ry < 5u ? ry : (g_aantalRye - ry) - 1u)
        {
        case 0u:    return Soort::TORING;
        case 1u:    return Soort::RUITER;
        case 2u:    return Soort::LOPER;
        case 3u:    return Soort::DAME;
        case 4u:    return Soort::KONING;
        default:    return Soort::ONBEKEND;
        }
    default:        return Soort::PION;
    }
}

Soort bepaalSoort(char letter)
{
    switch (letter)
    {
        case 'T':   return Soort::TORING;
        case 'R':   return Soort::RUITER;
        case 'L':   return Soort::LOPER;
        case 'D':   return Soort::DAME;
        case 'K':   return Soort::KONING;
        default:    return Soort::PION;
    }
}

char bepaalLetter (const Stuk *stuk)
{
    switch (stuk->soort)
    {
        case Soort::TORING: return stuk->isWit ? 'T' : 't';
        case Soort::RUITER: return stuk->isWit ? 'R' : 'r';
        case Soort::LOPER:  return stuk->isWit ? 'L' : 'l';
        case Soort::DAME:   return stuk->isWit ? 'D' : 'd';
        case Soort::KONING: return stuk->isWit ? 'K' : 'k';
        case Soort::PION:   return stuk->isWit ? 'P' : 'p';
        default:            return 0;
    }
}

void inisialiseerStukke(std::array<Stuk, g_aantalStukke> &stukke)
{
    for (uint_t i { 0u }; i < g_aantalStukke; ++i)
    {
        uint_t gelid { i / g_aantalRye };

        stukke.at(i).soort = bepaalSoort(i, gelid);
        stukke.at(i).isWit = (gelid < 2u);
    }
}
