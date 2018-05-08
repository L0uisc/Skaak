#include <iostream>
#include <string>
#include "skuif.h"

bool isWettig(const std::string skuif, const char stukke[sylengte][sylengte])
{
    return true;
}

bool isGeldig(const std::string skuif)
{
    bool isGeldigeLengte { ((skuif.length() >= 2)&&(skuif.length() <= 8)) };
    if (isGeldigeLengte)
    {
        //if ()
        return true;
    }
    else
    {
        std::cout << "Ongeldige toevoer: " + skuif + ". Probeer weer!\n\n";
        return false;
    }
}

std::string krySkuif(const bool isWit, char stukke[sylengte][sylengte])
{
    std::string skuif { "" };

    do
    {
        // Bepaal watter kleur volgende moet skuif om die spelers te herinner.
        std::string kleur { (isWit) ? "WIT" : "SWART" };

        std::cout << kleur << " skuif: ";
        std::cin >> skuif;
        std::cout << "\n";

        if (isGeldig(skuif))
        {
            if (isWettig(skuif, stukke))
                {
                    doenSkuif(isWit, stukke);
                    return skuif;
                }
        }
    }
    while (!(isWettig(skuif, stukke)&&(isGeldig(skuif))));
}

void doenSkuif(const bool isWit, char stukke[sylengte][sylengte])
{

}

bool isSpelVerby(bool isWit, const char stukke[sylengte][sylengte])
{
    static int i { 0 };         //Toetskode!!! Doen niks nuttig nie.
    if (i < 4)
    {
        ++i;
        return false;
    }
    else
        return true;
}

void skryfSkuif(const bool isSwart, const std::string skuif)
{
    static int skuiwe { 1 };

    std::string formattering { (isSwart) ? ("...\t") : "" };

    std::cout << "\t" << skuiwe << ".\t" << formattering << skuif << "\n\n";

    if (isSwart)
        ++skuiwe;
}
