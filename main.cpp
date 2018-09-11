#include <iostream>
#include <string>

#include "bord.h"
#include "skuif.h"
#include "stukke.h"
#include "konstantes.h"
#include "InputValidationRoutines.h"

enum class Wenner
{
    GELYKOP,
    WIT,
    SWART
};

Wenner bepaalWenner(bool isWit, const std::string &skuif, const std::string &stert)
{
    if (isPat(stert))
        return Wenner::GELYKOP;
    return isWit ? Wenner::SWART : Wenner::WIT; // As dit volgende Wit se beurt sou wees, het Swart gewen, en andersom.
}

bool isSpelVerby(bool isWit, const std::string &skuif, const std::string &stert)
{
    return isPat(stert) || isMat(stert) || isOorgee(skuif) || isSkaak(stert[1]);
}

bool speelWeer()
{
    char antwoord;
    do
    {
        std::cout << "Wil jy weer speel? <j>a of <n>ee:\n";
        antwoord = getValidChar();
    } while (!(antwoord == 'j' || antwoord == 'n'));

    return antwoord == 'j' ? true : false;
}

int main()
{
    do
    {
        std::cout << "Welkom by my skaakspel! Gee jou skuiwe in standaardnotasie, bv.:\n\n"
            "e4\t Dh5\t TXg4\t e4Xd5\t f8=D\t 0-0\t e5Xd5 e.p.\n"
            "Th1-e1\t Th4+\t Kf4++\t c7 mat\t d2 pat\t Gee oor.\n\n\n";

        bool isWit { true };

        Stuk stukke[aantalStukke];
        Posisie *posisies[aantalStukke];
        inisialiseerStukke(stukke, posisies);

        tekenBord(isWit, posisies);

        std::string skuif { "" };
        std::string stert { "" };

        do
        {
            skuif = valideerEnDoenSkuif(isWit, stukke, posisies);

            //Draai die bord om sodat die opponent nou die skuif kan sien en daaroor dink.
            isWit = !isWit;

            tekenBord(isWit, posisies);
            skryfSkuif(isWit, skuif);

            stert = skuif.length() > 3 ? skuif.substr(skuif.length() - 3, 3) : skuif;
        }
        while (!isSpelVerby(isWit, skuif, stert));

        ruimStukkeOp(posisies);

        switch (bepaalWenner(isWit, skuif, stert))
        {
        case Wenner::GELYKOP:
            std::cout << "Gelykop!\n\n";
            break;
        case Wenner::WIT:
            std::cout << "Wit wen!\n\n";
            break;
        case Wenner::SWART:
            std::cout << "Swart wen!\n\n";
            break;
        }

    } while (speelWeer());

    return 0;
}
