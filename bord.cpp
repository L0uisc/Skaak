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

void tekenBlokkie(const int ry, const int kolom, const char stukke[sylengte][sylengte])
{
    static int nKolom { 1 };

    if (nKolom == 1)
        std::cout << "\t";

    // Toets om te bepaal of daar 'n stuk op die blokkie is of nie.
    // As daar is, druk die stuk, anders druk die bord.
    if (stukke[ry][kolom] == leeg)
        std::cout << bord[ry][kolom];
    else
        std::cout << stukke[ry][kolom];

    // Toets of dit die einde van die ry is
    if (nKolom < sylengte)
    {
        std::cout << "  ";      // As dit nie is nie, las spasies aan vir formattering.
        ++nKolom;
    }
    else
    {
        std::cout << "\n\n";    // As dit is, laat 'n lyntjie oop.
        nKolom = 1;
    }
}

void tekenBord(const bool isWit, const char stukke[sylengte][sylengte])
{
    if (isWit)  // As dit Wit se beurt is om te speel, teken ons die bord met blokkie a1 links onder
                // sodat die aansig ooreenstem met wat 'n mens gewoonlik van Wit se kant af sien.
    {
        for (int ry { sylengte - 1 }; ry >= 0; --ry)
        {
            for (int kolom { 0 }; kolom < sylengte; ++kolom)
                tekenBlokkie(ry, kolom, stukke);
        }
    }
    else        //Anders (as dit Swart se beurt is) teken ons die bord andersom ;)
    {
        for (int ry { 0 }; ry < sylengte; ++ry)
        {
            for (int kolom { sylengte - 1 }; kolom >= 0; --kolom)
                tekenBlokkie(ry, kolom, stukke);
        }
    }
}
