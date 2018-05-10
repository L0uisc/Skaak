#include <iostream>
#include <string>
#include "skuif.h"

bool isRokeer(const std::string &skuif)
{
    return ((skuif == "0-0") || (skuif == "0-0-0"));
}

bool isOorgee(const std::string &skuif)
{
    return (skuif == "Gee oor.");
}

bool isStuk(const char &skuif)
{
    return ((skuif == 'K') || (skuif == 'D') || (skuif == 'T') || (skuif == 'R') || (skuif == 'L'));
}

bool isKoordinaat(const std::string &koordinaat)
{
    const char ry { koordinaat[0]};
    const char gelid { koordinaat[1]};
    return ((ry == 'a') || (ry == 'b') || (ry == 'c') || (ry == 'd') ||
            (ry == 'e') || (ry == 'f') || (ry == 'g') || (ry == 'h')) &&
           ((gelid == '1') || (gelid == '2') || (gelid == '3') || (gelid == '4') ||
            (gelid == '5') || (gelid == '6') || (gelid == '7') || (gelid == '8'));
}

bool isBuit(const char &buit)
{
    return (buit == 'X');
}

bool isKoppelteken(const char &koppelteken)
{
    return (koppelteken == '-');
}

bool isSpasie(const char &spasie)
{
    return (spasie == ' ');
}

bool isIsGelykAan(const char &isGelykAan)
{
    return (isGelykAan == '=');
}

bool isSkaak(const char &skaak)
{
    return (skaak == '+');
}

bool isLaasteGelid(const char &gelid, const bool &isWit)
{
    return (isWit) ? (gelid == '8') : (gelid == '1');
}

bool isEnPassant(const std::string &enPassant)
{
    return (enPassant == "e.p.");
}

bool isMat(const std::string &mat)
{
    return (mat == "mat");
}

bool isPat(const std::string &pat)
{
    return (pat == "pat");
}

bool isWettig(const std::string &skuif, const char stukke[sylengte][sylengte])
{
    return true;
}

bool isGeldig(const std::string &skuif, const bool &isWit)
{
    enum class Karakter
    {
        BEGIN,
        STUK,
        KOORDINAAT,
        BUIT,
        KOPPELTEKEN,
        ISGELYKAAN,
        SKAAK,
        SPASIE
    };

    const int skuifLengte { skuif.length() };
    bool isGeldig { !((skuifLengte < 2) && (skuifLengte > 14)) };
    std::string fout { (skuifLengte < 2) ? (" te kort") : " te lank" };

    Karakter vorige { Karakter::BEGIN };        //  Enumerator hou tred met watter soort karakter die vorige karakter
                                                //  in die skuif was.
    for (int i { 0 }; i < skuifLengte; ++i)
    {
        switch (vorige)
        {
            case Karakter::BEGIN:   //  Voer uit as huidige karakter (skuif[i]) die eerste karakter van die string is.
            {
                isGeldig = isRokeer(skuif) || isOorgee(skuif) || isStuk(skuif[i]) || isKoordinaat(skuif.substr(i, 2));
                break;
            }
            case Karakter::STUK:    // Voer uit as huidige karakter (skuif[i]) op 'n stuk volg.
            {
                isGeldig = isKoordinaat(skuif.substr(i, 2)) || isBuit(skuif[i]);
                break;
            }
            case Karakter::KOORDINAAT:  // Voer uit as huidige karakter (skuif[i]) op 'n stuk volg.
            {
                isGeldig = isBuit(skuif[i]) || isKoppelteken(skuif[i]) || isSpasie(skuif[i]) ||
                           isIsGelykAan(skuif[i]) || isSkaak(skuif[i]);
                break;
            }
            case Karakter::BUIT:    // Voer uit as huidige karakter (skuif[i]) op 'n stuk volg.
            {
                isGeldig = isKoordinaat(skuif.substr(i, 2));
                break;
            }
            case Karakter::KOPPELTEKEN: // Voer uit as huidige karakter (skuif[i]) op 'n stuk volg.
            {
                isGeldig = isKoordinaat(skuif.substr(i, 2));
                break;
            }
            case Karakter::ISGELYKAAN:  // Voer uit as huidige karakter (skuif[i]) op 'n stuk volg.
            {
                isGeldig = isLaasteGelid(skuif[i - 2], isWit) && isStuk(skuif[i]);
                if (!isGeldig)
                    i -= 2;
                break;
            }
            case Karakter::SKAAK:   // Voer uit as huidige karakter (skuif[i]) op 'n stuk volg.
            {
                isGeldig = isSkaak(skuif[i]) && (i == skuifLengte - 1);
                break;
            }
            case Karakter::SPASIE:  // Voer uit as huidige karakter (skuif[i]) op 'n stuk volg.
            {
                isGeldig = (isEnPassant(skuif.substr(i, 4)) && (skuifLengte - i) == 4) || (isMat(skuif.substr(i, 3))
                            && (skuifLengte - i) == 3) || (isPat(skuif.substr(i, 3)) && (skuifLengte - i) == 3);
                break;
            }
        }
        if (isRokeer(skuif) || isOorgee(skuif) || isEnPassant(skuif.substr(i, 4)) || isMat(skuif.substr(i, 3)) ||
            isPat(skuif.substr(i, 3)))
            break;

        if (isGeldig)
        {
            //  Bepaal watter soort karakter pas beskou is.
            if (isStuk(skuif[i]))
                vorige = Karakter::STUK;

            else if (isKoordinaat(skuif.substr(i, 2)))
            {
                vorige = Karakter::KOORDINAAT;
                ++i;    //  Skuif die posisie in die string 'n ekstra plek aan omdat die volgende karakter reeds
                        //  getoets is.
            }
            else if (isBuit(skuif[i]))
                vorige = Karakter::BUIT;

            else if (isKoppelteken(skuif[i]))
                vorige = Karakter::KOPPELTEKEN;

            else if (isSpasie(skuif[i]))
                vorige = Karakter::SPASIE;

            else if (isIsGelykAan(skuif[i]))
                vorige = Karakter::ISGELYKAAN;

            else if (isSkaak(skuif[i]))
                vorige = Karakter::SKAAK;
        }
        else
        {
            if (skuif[i] == 'a' || skuif[i] == 'b' || skuif[i] == 'c' || skuif[i] == 'd' ||
                skuif[i] == 'e' || skuif[i] == 'f' || skuif[i] == 'g' || skuif[i] == 'h')
                ++i;
            fout = std::to_string(i + 1) + " e karakter foutief";
            std::cout << "Ongeldige toevoer: " + fout + ". Probeer weer!\n\n";
            break;
        }
    }
    return isGeldig;
}

std::string krySkuif(const bool &isWit, char stukke[sylengte][sylengte])
{
    std::string skuif { "" };
    bool isReg { false };       //  Dui aan wanneer die skuif wettig EN geldig is.

    do
    {
        // Bepaal watter kleur volgende moet skuif om die spelers te herinner.
        std::string kleur { (isWit) ? "WIT" : "SWART" };
        std::cout << kleur << " skuif: ";
        std::getline(std::cin, skuif);
        std::cout << "\n";

        if (isGeldig(skuif, isWit))
            if (isWettig(skuif, stukke))
                {
                    doenSkuif(isWit, stukke);
                    isReg = true;
                    return skuif;
                }
    }
    while (!isReg);
}

void doenSkuif(const bool &isWit, char stukke[sylengte][sylengte])
{

}

bool isSpelVerby(bool &isWit, const char stukke[sylengte][sylengte])
{
    static int i { 0 };         //Toetskode!!! Doen niks nuttig nie.
    if (i < 20)
    {
        ++i;
        return false;
    }
    else
        return true;
}

void skryfSkuif(const bool &isSwart, const std::string &skuif)
{
    static int skuiwe { 1 };

    std::string formattering { (isSwart) ? ("...\t") : "" };

    std::cout << "\t" << skuiwe << ".\t" << formattering << skuif << "\n\n";

    if (isSwart)
        ++skuiwe;
}
