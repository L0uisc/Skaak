#include <iostream>
#include <string>
#include "skuif.h"

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

bool isRokeer(const std::string &skuif)
{
    return ((skuif == "0-0") || (skuif == "0-0-0"));
}

bool isOorgee(const std::string &skuif)
{
    return (skuif == "Gee oor.");
}

bool isStuk(char skuif)
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

bool isBuit(char buit)
{
    return (buit == 'X');
}

bool isKoppelteken(char koppelteken)
{
    return (koppelteken == '-');
}

bool isSpasie(char spasie)
{
    return (spasie == ' ');
}

bool isPromoveer(char isGelykAan)
{
    return (isGelykAan == '=');
}

bool isSkaak(char skaak)
{
    return (skaak == '+');
}

bool isLaasteGelid(char gelid, bool isWit)
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

bool isWettig(const std::string &skuif, const Stuk *stukke)
{
    return true;
}

bool isGeldig(const std::string &skuif, bool isWit)
{
    const int skuifLengte { skuif.length() };
    std::string fout { "" };
    bool isGeldig { !((skuifLengte < 2) || (skuifLengte > 14)) };
    if (!isGeldig)
        fout = ((skuifLengte < 2) ? " te kort" : " te lank");

    //  Enumerator hou tred met watter soort karakter die vorige karakter in die skuif was.
    Karakter vorige { Karakter::BEGIN };
    int koordinaatTal { 0 };
    int buitOfKoppeltekenTal { 0 };

    for (int i { 0 }; i < skuifLengte; ++i)
    {
        bool isRy { false };

        switch (vorige)
        {
        case Karakter::BEGIN:   //  Voer uit as huidige karakter (skuif[i]) die eerste karakter van die string is.
            isGeldig = isRokeer(skuif) || isOorgee(skuif) || isStuk(skuif[i]) ||
                isKoordinaat(skuif.substr(i, 2), isRy);
            break;

        case Karakter::STUK:    // Voer uit as huidige karakter (skuif[i]) op 'n stuk volg.
            isGeldig = (isKoordinaat(skuif.substr(i, 2), isRy) && koordinaatTal < 2) || isBuit(skuif[i]);
            break;

        case Karakter::KOORDINAAT:  // Voer uit as huidige karakter (skuif[i]) op 'n koördinaat volg.
            isGeldig = ((isBuit(skuif[i]) || isKoppelteken(skuif[i])) && buitOfKoppeltekenTal < 1) ||
                isSpasie(skuif[i]) || (isPromoveer(skuif[i]) && (koordinaatTal == 1)) || isSkaak(skuif[i]);
            break;

        case Karakter::BUIT:    // Voer uit as huidige karakter (skuif[i]) op 'n 'X' volg.
            isGeldig = isKoordinaat(skuif.substr(i, 2), isRy) && (koordinaatTal == 1);
            break;

        case Karakter::KOPPELTEKEN: // Voer uit as huidige karakter (skuif[i]) op 'n koppelteken volg.
            isGeldig = isKoordinaat(skuif.substr(i, 2), isRy) && (koordinaatTal == 1);
            break;

        case Karakter::ISGELYKAAN:  // Voer uit as huidige karakter (skuif[i]) op 'n '=' volg.
            // 'n Pion kan net promoveer op die laaste gelid. As die koördinaat 'n ander gelid gee, lê die fout
            // reeds by daardie karakter in die skuifstring.
            if (!isLaasteGelid(skuif[i - 2], isWit))
                i -= 2;
            isGeldig = isLaasteGelid(skuif[i - 2], isWit) && isStuk(skuif[i]) && (skuifLengte - i) == 1;
            break;

        case Karakter::SKAAK:   // Voer uit as huidige karakter (skuif[i]) op 'n '+' volg.
            isGeldig = isSkaak(skuif[i]) && ((skuifLengte - i) == 1);
            break;

        case Karakter::SPASIE:  // Voer uit as huidige karakter (skuif[i]) op 'n spasie volg.
            isGeldig = (isEnPassant(skuif.substr(i, 4)) && (i == (skuifLengte - 4))) ||
                             (isMat(skuif.substr(i, 3)) && (i == (skuifLengte - 3))) ||
                             (isPat(skuif.substr(i, 3)) && (i == (skuifLengte - 3)));
            break;

        default:
            isGeldig = false;
        }
        if (isGeldig)
        {
            if (isRokeer(skuif) || isOorgee(skuif) || isEnPassant(skuif.substr(i, 4)) ||
                isMat(skuif.substr(i, 3)) || isPat(skuif.substr(i, 3)))
                break;

            //  Wat die huidige karakter is, bepaal wat die volgende karakter mag wees.
            if (isKoordinaat(skuif.substr(i, 2), isRy))
            {
                ++koordinaatTal;
                ++i;    //  Die volgende karakter is reeds getoets.
                vorige = Karakter::KOORDINAAT;
            }
            else if (isBuit(skuif[i]))
            {
                ++buitOfKoppeltekenTal;
                vorige = Karakter::BUIT;
            }
            else if (isKoppelteken(skuif[i]))
            {
                ++buitOfKoppeltekenTal;
                vorige = Karakter::KOPPELTEKEN;
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
            std::cout << "Ongeldige toevoer: " << fout << ". Probeer weer!\n\n";
            break;
        }
    }
    return isGeldig;
}

std::string krySkuif(bool isWit, const Stuk *stukke)
{
    while (true)
    {
        std::string skuif { "" };

        // Bepaal watter kleur volgende moet skuif om die spelers te herinner.
        const std::string kleur { (isWit) ? "WIT" : "SWART" };
        std::cout << kleur << " skuif: ";
        std::getline(std::cin, skuif);
        std::cout << "\n";

        if (isGeldig(skuif, isWit) && isWettig(skuif, stukke))
            return skuif;
    }
}

void doenSkuif(const std::string &skuif, bool isWit, Stuk *stukke)
{

}

void skryfSkuif(bool isSwart, const std::string &skuif)
{
    static int skuiwe { 1 };

    std::cout << "\t" << skuiwe << ".\t" << (isSwart ? "...\t" : "") << skuif << "\n\n";

    if (isSwart)
        ++skuiwe;
}
