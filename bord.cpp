#include <iostream>
#include <array>
#include "bord.h"

void inisialiseerBord(std::array<std::array<Blokkie, g_sylengte>,g_sylengte> &bord,
                      std::array<Stuk, g_aantalStukke> &stukke)
{
    for (uint_t gelid { 0 }; gelid < g_sylengte; ++gelid)
    {
        for (uint_t ry { 0 }; ry < g_sylengte; ++ry)
        {
            if (gelid > 0 && gelid < g_sylengte - 1)
            {
                if (ry > 0 && ry < g_sylengte - 1)
                {
                    bord.at(gelid).at(ry).agtergrond = (gelid + ry) % 2 ? 'W' : ' ';

                    if (gelid <= 2 || gelid >= 7)
                    {
                        uint_t stukIndeks { ((gelid <= 2 ? gelid :
                            (gelid - 4)) - 1) * g_aantalRye + ry - 1 };
                        bord.at(gelid).at(ry).stuk = &stukke.at(stukIndeks);
                        continue;
                    }
                }
                else
                    bord.at(gelid).
                        at(ry).agtergrond =
                        gelid + '1' - 1;
            }
            else
            {
                if (ry > 0 && ry < g_sylengte - 1)
                {
                    bord.at(gelid).
                        at(ry).agtergrond =
                        ry + 'a' - 1;
                }
                else
                {
                    bord.at(gelid).
                        at(ry).agtergrond = ' ';
                }
            }

            bord.at(gelid).at(ry).stuk = nullptr;
        }
    }
}

void tekenBlokkie(uint_t gelid, uint_t ry, std::array<std::array<Blokkie, g_sylengte>,
                  g_sylengte> &bord)
{
    std::cout << ((bord.at(gelid).at(ry).stuk) ?
                  (bepaalLetter(bord.at(gelid).at(ry).stuk)) :
                  (bord.at(gelid).at(ry).agtergrond));
}

void tekenBord(bool isWit, std::array<std::array<Blokkie, g_sylengte>,
               g_sylengte> &bord)
{
    // As dit Wit se beurt is, teken die bord met blokkie a1 links onder.
    // As dit Swart se beurt is, teken ons blokkie a1 regs bo.
    for (int gelid { isWit ? static_cast<int>(g_sylengte - 1) : 0 };
            isWit ? gelid >= 0 : gelid < static_cast<int>(g_sylengte);
            isWit ? --gelid : ++gelid)
    {
        std::cout << "\t";
        for (int ry { isWit ? 0 : static_cast<int>(g_sylengte - 1) };
                isWit ? ry < static_cast<int>(g_sylengte) : ry >= 0;
                isWit? ++ry : --ry)
        {
            tekenBlokkie(static_cast<uint_t>(gelid), static_cast<uint_t>(ry), bord);
            if (isWit ? ry < (static_cast<int>(g_sylengte - 1)) : ry > 0)
                std::cout << " |";
            else
                std::cout << "\n";
        }
        if (isWit ? gelid > 0 : gelid < static_cast<int>(g_sylengte - 1))
            std::cout << "\t--|--|--|--|--|--|--|--|--|--\n";
    }

    std::cout << "\n";
}
