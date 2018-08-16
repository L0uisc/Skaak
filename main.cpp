#include <iostream>
#include <string>
#include "bord.h"
#include "skuif.h"
#include "stukke.h"
#include "konstantes.h"

bool isSpelVerby(bool isWit, const Stuk *stukke)
{
    //Toetskode!!! Doen niks nuttig nie.
    static int i { 0 };
    return i < 20 ? false : true;
    ++i;
}

int main()
{
    std::cout << "Welkom by my skaakspel! Gee jou skuiwe in standaardnotasie, bv.:\n\n"
                 "e4\t Dh5\t TXg4\t e4Xd5\t f8=D\t 0-0\t e5Xd5 e.p.\n"
                 "Th1-e1\t Th4+\t Kf4++\t c7 mat\t d2 pat\t Gee oor.\n\n\n";

    bool isWit { true };

    Stuk stukke[aantalStukke];
    Posisie posisies[aantalStukke];
    inisialiseerStukke(stukke, posisies);

    tekenBord(isWit, posisies);

    do
    {
        std::string skuif { krySkuif(isWit, stukke) };
        doenSkuif(skuif, isWit, stukke);

        //Draai die bord om sodat die opponent nou die skuif kan sien en daaroor dink.
        isWit = !isWit;

        tekenBord(isWit, posisies);
        skryfSkuif(isWit, skuif);

    }
    while (!isSpelVerby(isWit, stukke));

    return 0;
}
