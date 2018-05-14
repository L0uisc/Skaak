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

bool isKoordinaat(const std::string &koordinaat, bool &isRy)
{
    const char ry { koordinaat[0]};
    const char gelid { koordinaat[1]};

    isRy        =   (ry == 'a') || (ry == 'b') || (ry == 'c') || (ry == 'd') ||
                    (ry == 'e') || (ry == 'f') || (ry == 'g') || (ry == 'h');
    bool isGelid {  (gelid == '1') || (gelid == '2') || (gelid == '3') || (gelid == '4') ||
                    (gelid == '5') || (gelid == '6') || (gelid == '7') || (gelid == '8') };

    return ((isRy) && (isGelid));
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

bool isPromoveer(const char &isGelykAan)
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
    int koordinaatTal { 0 };
    int buitOfKoppeltekenTal { 0 };

    for (int i { 0 }; i < skuifLengte; ++i)
    {
        bool isRy { false };

        switch (vorige)
        {
            case Karakter::BEGIN:   //  Voer uit as huidige karakter (skuif[i]) die eerste karakter van die string is.
                isGeldig =  isRokeer(skuif) || isOorgee(skuif) || isStuk(skuif[i]) ||
                            (isKoordinaat(skuif.substr(i, 2), isRy) && !(koordinaatTal > 2));
                break;

            case Karakter::STUK:    // Voer uit as huidige karakter (skuif[i]) op 'n stuk volg.
                isGeldig = (isKoordinaat(skuif.substr(i, 2), isRy) && koordinaatTal < 2) || isBuit(skuif[i]);
                break;

            case Karakter::KOORDINAAT:  // Voer uit as huidige karakter (skuif[i]) op 'n koordinaat volg.
                isGeldig = ((isBuit(skuif[i]) || isKoppelteken(skuif[i])) && buitOfKoppeltekenTal < 1)
                || isSpasie(skuif[i]) || isPromoveer(skuif[i]) || isSkaak(skuif[i]);
                break;

            case Karakter::BUIT:    // Voer uit as huidige karakter (skuif[i]) op 'n 'X' volg.
                isGeldig = isKoordinaat(skuif.substr(i, 2), isRy) && koordinaatTal < 2;
                break;

            case Karakter::KOPPELTEKEN: // Voer uit as huidige karakter (skuif[i]) op 'n koppelteken volg.
                isGeldig = isKoordinaat(skuif.substr(i, 2), isRy) && koordinaatTal < 2;
                break;

            case Karakter::ISGELYKAAN:  // Voer uit as huidige karakter (skuif[i]) op 'n '=' volg.
                isGeldig = isLaasteGelid(skuif[i - 2], isWit) && isStuk(skuif[i]) && ((skuifLengte - i) == 1);
                if (!isLaasteGelid(skuif[i - 2], isWit))
                    i -= 2;
                break;

            case Karakter::SKAAK:   // Voer uit as huidige karakter (skuif[i]) op 'n '+' volg.
                isGeldig = isSkaak(skuif[i]) && ((skuifLengte - i) == 1);
                break;

            case Karakter::SPASIE:  // Voer uit as huidige karakter (skuif[i]) op 'n spasie volg.
                isGeldig = (isEnPassant(skuif.substr(i, 4)) && (skuifLengte - i) == 4) || (isMat(skuif.substr(i, 3))
                            && (skuifLengte - i) == 3) || (isPat(skuif.substr(i, 3)) && (skuifLengte - i) == 3);
                break;
        }
        if (isGeldig)
        {
            if (isRokeer(skuif) || isOorgee(skuif) || isEnPassant(skuif.substr(i, 4))
                || isMat(skuif.substr(i, 3)) || isPat(skuif.substr(i, 3)))
                break;

            //  Bepaal watter soort karakter pas beskou is.
            if (isKoordinaat(skuif.substr(i, 2), isRy))
            {
                vorige = Karakter::KOORDINAAT;
                ++koordinaatTal;
                ++i;    //  Skuif die posisie in die string 'n ekstra plek aan omdat die volgende karakter reeds
                        //  getoets is.
            }
            else if (isBuit(skuif[i]))
            {
                vorige = Karakter::BUIT;
                ++buitOfKoppeltekenTal;
            }
            else if (isKoppelteken(skuif[i]))
            {
                vorige = Karakter::KOPPELTEKEN;
                ++buitOfKoppeltekenTal;
            }
            else if (isSpasie(skuif[i]))
                vorige = Karakter::SPASIE;
            else if (isPromoveer(skuif[i]))
                vorige = Karakter::ISGELYKAAN;
            else if (isSkaak(skuif[i]))
                vorige = Karakter::SKAAK;
            else if (isStuk(skuif[i]))
                vorige = Karakter::STUK;
        }
        else
        {
            if (isRy && koordinaatTal < 2)
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
    bool isWettigEnGeldig { false };       //  Dui aan wanneer die skuif wettig EN geldig is.

    do
    {
        // Bepaal watter kleur volgende moet skuif om die spelers te herinner.
        const std::string kleur { (isWit) ? "WIT" : "SWART" };
        std::cout << kleur << " skuif: ";
        std::getline(std::cin, skuif);
        std::cout << "\n";

        if (isGeldig(skuif, isWit))
            if (isWettig(skuif, stukke))
                {
                    doenSkuif(isWit, stukke);
                    isWettigEnGeldig = true;
                    return skuif;
                }
    }
    while (!isWettigEnGeldig);
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

    const std::string formattering { (isSwart) ? ("...\t") : "" };

    std::cout << "\t" << skuiwe << ".\t" << formattering << skuif << "\n\n";

    if (isSwart)
        ++skuiwe;
}
