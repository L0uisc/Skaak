#include <iostream>
#include "bord.h"

extern const char leeg { 0 };

const char bord[][sylengte]     // 'n " " stel 'n swart blokkie voor en 'n "W" 'n wit blokkie.
{
{ ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', ' ' },
{ '1', ' ', 'W', ' ', 'W', ' ', 'W', ' ', 'W', '1' },
{ '2', 'W', ' ', 'W', ' ', 'W', ' ', 'W', ' ', '2' },
{ '3', ' ', 'W', ' ', 'W', ' ', 'W', ' ', 'W', '3' },
{ '4', 'W', ' ', 'W', ' ', 'W', ' ', 'W', ' ', '4' },
{ '5', ' ', 'W', ' ', 'W', ' ', 'W', ' ', 'W', '5' },
{ '6', 'W', ' ', 'W', ' ', 'W', ' ', 'W', ' ', '6' },
{ '7', ' ', 'W', ' ', 'W', ' ', 'W', ' ', 'W', '7' },
{ '8', 'W', ' ', 'W', ' ', 'W', ' ', 'W', ' ', '8' },
{ ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', ' ' }
};

void tekenBlokkie(const int &gelid, const int &ry, const char stukke[sylengte][sylengte])
{
    static int nRy { 1 };

    if (nRy == 1)
        std::cout << "\t";

    // Toets om te bepaal of daar 'n stuk op die blokkie is of nie.
    // As daar is, druk die stuk, anders druk die bord.
    if (stukke[gelid][ry] == leeg)
        std::cout << bord[gelid][ry];
    else
        std::cout << stukke[gelid][ry];

    // Toets of dit die einde van die ry is
    if (nRy < sylengte)
    {
        std::cout << "  ";      // As dit nie is nie, las spasies aan vir formattering.
        ++nRy;
    }
    else
    {
        std::cout << "\n\n";    // As dit is, laat 'n lyntjie oop.
        nRy = 1;
    }
}

void tekenBord(const bool &isWit, const char stukke[sylengte][sylengte])
{
    if (isWit)  // As dit Wit se beurt is om te speel, teken ons die bord met blokkie a1 links onder
                // sodat die aansig ooreenstem met wat 'n mens gewoonlik van Wit se kant af sien.
    {
        for (int gelid { sylengte - 1 }; gelid >= 0; --gelid)
        {
            for (int ry { 0 }; ry < sylengte; ++ry)
                tekenBlokkie(gelid, ry, stukke);
        }
    }
    else        //Anders (as dit Swart se beurt is) teken ons die bord andersom ;)
    {
        for (int gelid { 0 }; gelid < sylengte; ++gelid)
        {
            for (int ry { sylengte - 1 }; ry >= 0; --ry)
                tekenBlokkie(gelid, ry, stukke);
        }
    }
}