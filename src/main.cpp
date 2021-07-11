#include "bord.h"
#include "skuif.h"
#include "stukke.h"
#include "konstantes.h"
#include "InputValidationRoutines.h"
#include <iostream>
#include <string>
#include <array>
#include <vector>

enum class Wenner
{
    GELYKOP,
    WIT,
    SWART
};

Wenner bepaalWenner(bool isWit, const std::string &skuif,
                    const std::string &stert)
{
    if (isPat(stert))
        return Wenner::GELYKOP;
    else if (isOorgee(skuif))
        // As die vorige skuif 'n oorgee was, wen die speler wat volgende skuif.
        return isWit ? Wenner::WIT : Wenner::SWART;
    else
        // As dit volgende Wit se beurt sou wees, het Swart gewen, en andersom.
        return isWit ? Wenner::SWART : Wenner::WIT;
}

bool isSpelVerby(/*bool isWit, vir herhaling v. skuiwe, 50 skuiwe, ens*/
                 const std::string &skuif, const std::string &stert)
{
    return isPat(stert) || isMat(stert) || isOorgee(skuif) ||
        isSkaak(stert.at(1u));
}

bool speelWeer()
{
    char antwoord {};
    do
    {
        std::cout << "Wil jy weer speel? <j>a of <n>ee:\n";
        antwoord = getValidChar();
    } while (!(antwoord == 'j' || antwoord == 'n'));

    return antwoord == 'j';
}


void tekenGebuiteStukke(const buitVector_t &gebuiteStukke)
{
    std::cout << "\t";
    int i { 1u };
    for (auto stuk : gebuiteStukke)
    {
        std::cout << bepaalLetter(stuk);
        std::cout << (i == gebuiteStukke.size() ? "\n" : " ");

        if (i >= gebuiteStukke.size())
            break;
        ++i;
    }
    std::cout << "\n";
}

int main()
{
    do
    {
        std::cout << "Welkom by my skaakspel! Gee jou skuiwe in standaardnotasie, bv.:\n\n"
            "e4\t Dh5\t TXg4\t e4Xd5\t f8=D\t 0-0\t e5Xd5 e.p.\n"
            "Th1-e1\t Th4+\t Kf4++\t c7 mat\t d2 pat\t Gee oor.\n\n\n";

        stukArray_t stukke;
        bordArray_t bord;
        inisialiseerStukke(stukke);
        inisialiseerBord(bord, stukke);

        bool isWit { true };
        tekenBord(isWit, bord);

        std::string skuif {};
        std::string stert {};

        buitVector_t witGebuiteStukke {};
        buitVector_t swartGebuiteStukke {};

        do
        {
            skuif = krySkuif(isWit, bord/*, stukke*/);

            if (!isOorgee(skuif))
                doenSkuif(isWit, skuif, bord, (isWit ? swartGebuiteStukke :
                          witGebuiteStukke));

            //Draai die bord sodat die opponent die skuif sien en daaroor dink.
            isWit = !isWit;

            tekenGebuiteStukke(isWit ? witGebuiteStukke : swartGebuiteStukke);
            tekenBord(isWit,bord);
            tekenGebuiteStukke(isWit ? swartGebuiteStukke : witGebuiteStukke);

            skryfSkuif(isWit, skuif);

            stert = skuif.length() > 3u ? skuif.substr(skuif.length() - 3u, 3u) :
                skuif;
        }
        while (!isSpelVerby(/*isWit, */skuif, stert));

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
        default:
            std::cout << "Fout! Kan nie die wenner bepaal nie.\n\n";
        }

    } while (speelWeer());

    return 0;
}
