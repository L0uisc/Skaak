#include <iostream>
#include "bord.h"

constexpr int sylengte { 10 };      // 'n Skaakbord is 8x8 met twee ekstra rye vir
                                    // koordinaatletters en -syfers.

constexpr char bord[][sylengte]     // 'n " " stel 'n swart blokkie voor en 'n "W" 'n wit blokkie.
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
        default:            return '0';
    }
}

void tekenBlokkie(bool isWit, int gelid, int ry, Posisie **posisies)
{
    static int nRy { 1 };
    static int i { isWit ? (aantalStukke - aantalRye) : aantalRye - 1 };
    static int teller { 0 };

    if (nRy == 1)
        std::cout << "\t";

    if ((posisies[i]->koordinaat.gelid == gelid) && (posisies[i]->koordinaat.ry == ry))
    {
        std::cout << bepaalLetter(posisies[i]->stuk);
        if (isWit)
        {
            ++i;
            if (i % aantalRye == 0 && teller < sylengte * 8)
                i -= aantalRye * 2;
        }
        else
        {
            if (i % aantalRye == 0 && teller < sylengte * 8)
                i += aantalRye * 2;
            --i;
        }
    }
    else
    {
        std::cout << bord[gelid][ry];
    }

    std::cout << (nRy == sylengte ? "\n\n" : "  ");
    nRy == sylengte ? nRy = 1 : ++nRy;
    ++teller;
    if (teller == sylengte * sylengte)
    {
        i = isWit ? (aantalRye - 1) : (aantalStukke - aantalRye);
        teller = 0;
    }
}

void tekenGebuiteStukke(bool isWit, Posisie **posisies)
{
    int i { isWit ? 0 : aantalStukke - 1 };
    while (isWit ? posisies[i]->koordinaat.gelid == 0 : posisies[i]->koordinaat.gelid == 9)
    {
        std::cout << bepaalLetter(posisies[i]->stuk) << " ";
        isWit ? ++i : --i;
    }
}

void tekenBord(bool isWit, Posisie **posisies)
{
    tekenGebuiteStukke(isWit, posisies);

    if (isWit)  // As dit Wit se beurt is om te speel, teken ons die bord met blokkie a1 links onder
                // sodat die aansig ooreenstem met wat 'n mens gewoonlik van Wit se kant af sien.
    {
        for (int gelid { sylengte - 1 }; gelid >= 0; --gelid)
        {
            for (int ry { 0 }; ry < sylengte; ++ry)
                tekenBlokkie(isWit, gelid, ry, posisies);
        }
    }
    else        //Anders (as dit Swart se beurt is) teken ons die bord andersom ;)
    {
        for (int gelid { 0 }; gelid < sylengte; ++gelid)
        {
            for (int ry { sylengte - 1 }; ry >= 0; --ry)
                tekenBlokkie(isWit, gelid, ry, posisies);
        }
    }
    tekenGebuiteStukke(!isWit, posisies);
}
