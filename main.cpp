#include <iostream>
#include <string>
#include "skuif.h"

int main()
{
    char stukke[][sylengte] // 0 stel 'n lee blokkie voor.
    {
    { leeg },
    { leeg, 'T', 'R', 'L', 'D', 'K', 'L', 'R', 'T' },
    { leeg, 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P' },
    { leeg },
    { leeg },
    { leeg },
    { leeg },
    { leeg, 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p' },
    { leeg, 't', 'r', 'l', 'd', 'k', 'l', 'r', 't' },
    { leeg }
    };

    std::cout << "Welkom by my skaakspel! Gee jou skuiwe in standaardnotasie (bv. Txg4)\n\n\n";

    bool isWit { true };
    tekenBord(isWit, stukke);

    do
    {
        std::string skuif { krySkuif(isWit, stukke) };

        //Draai die bord om sodat die opponent nou die skuif kan sien en daaroor dink.
        isWit = !isWit;

        tekenBord(isWit, stukke);
        skryfSkuif(isWit, skuif);

    }
    while (!(isSpelVerby(isWit, stukke)));

    return 0;
}
