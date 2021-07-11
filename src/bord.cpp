#include "konstantes.h"
#include "bord.h"
#include "stukke.h"
#include <iostream>
#include <array>

void inisialiseerBord(bordArray_t &bord, stukArray_t &stukke)
{
    for (int gelid { 0u }; gelid < g_sylengte; ++gelid)
    {
        for (int ry { 0u }; ry < g_sylengte; ++ry)
        {
            Blokkie *blokkie { &bord.at(gelid).at(ry) };

            blokkie->agtergrond = (gelid + ry) % 2u ? 'W' : ' ';

            if (gelid < 2u || gelid >= 6u)
            {
                int stukIndeks { g_sylengte * (gelid < 2u ?  gelid : gelid- 4u) +
                                      ry % g_sylengte };
                blokkie->stuk = &stukke.at(stukIndeks);
            }
            else
            {
                blokkie->stuk = nullptr;
            }
        }
    }
}

void tekenBlokkie(int gelid, int ry, bordArray_t &bord)
{
    Blokkie* blokkie { &bord.at(gelid).at(ry) };
    std::cout << ((blokkie->stuk) ? (bepaalLetter(blokkie->stuk)) : (blokkie->agtergrond));
}

void skryfRye(bool isWit)
{
    static const char *rye { "abcdefgh" };

    std::cout << "\t  |";
    for (int ry { isWit ? 0 : static_cast<int>(g_sylengte - 1u) };
            isWit ? ry < static_cast<int>(g_sylengte) : ry >= 0;
            isWit ? ++ry : --ry)
    {
        std::cout << rye[ry] << " |";
    }
    std::cout << "\n";
}

void tekenBord(bool isWit, bordArray_t &bord)
{
    static const char *geledere { "12345678" };

    skryfRye(isWit);
    // As dit Wit se beurt is, teken die bord met blokkie a1 links onder.
    // As dit Swart se beurt is, teken ons blokkie a1 regs bo.
    for (int gelid { isWit ? static_cast<int>(g_sylengte - 1u) : 0 };
            isWit ? gelid >= 0 : gelid < static_cast<int>(g_sylengte);
            isWit ? --gelid : ++gelid)
    {
        if (isWit ? gelid >= 0 : gelid < static_cast<int>(g_sylengte))
            std::cout << "\t--|--|--|--|--|--|--|--|--|--\n";

        std::cout << "\t";
        std::cout << geledere[gelid] << " |";
        for (int ry { isWit ? 0 : static_cast<int>(g_sylengte - 1u) };
                isWit ? ry < static_cast<int>(g_sylengte) : ry >= 0;
                isWit ? ++ry : --ry)
        {
// TODO (User#1#): Pass Blokkie* i.p.v. ry, gelid en bord.
            tekenBlokkie(static_cast<int>(gelid), static_cast<int>(ry), bord);
            std::cout << " |";
        }

        std::cout << geledere[gelid] << "\n";
    }

    std::cout << "\t--|--|--|--|--|--|--|--|--|--\n";
    skryfRye(isWit);
    std::cout << "\n";
}
