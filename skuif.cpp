#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include "skuif.h"
#include "konstantes.h"
#include "stukke.h"
#include "bord.h"

struct Koordinaat
{
    uint_t gelid    { 8u };
    uint_t ry       { 0u };
};

namespace Skuif
{
    static Blokkie *g_eindBlokkie       { nullptr };
    static Koordinaat g_eindPos;
    static Blokkie *g_beginBlokkie      { nullptr };
    static Koordinaat g_beginPos;
#if 0
    static Koordinaat g_witKoningPos    { 0u, 4u };
    static Koordinaat g_swartKoningPos  { 7u, 4u };
#endif // 0
}

enum class Karakter
{
    BEGIN,
    STUK,
    RY,
    GELID,
    BUIT,
    KOPPELTEKEN,
    PROMOVEER,
    SKAAK,
    SPASIE
};

enum class Fout
{
    ONBEKEND,
    TE_KORT,
    TE_LANK,
    BEGIN,
    STUK,
    RY,
    GELID,
    BUIT,
    KOPPELTEKEN,
    SKAAK,
    SPASIE,
    PROMOVEER_NIE_NA_STUK,
    PROMOVEER_NIE_OP_LAASTE_GELID,
    EN_PASSANT_NIE_NA_6DE_GELID,
    EN_PASSANT_NIE_BUITSKUIF,
    EN_PASSANT_NIE_ONMIDDELLIK,
    TE_VEEL_KOORDINATE,
    TE_VEEL_BUIT,
    TE_VEEL_KOPPELTEKEN,
    ROKEER_KONING_REEDS_GESKUIF,
    ROKEER_TORING_REEDS_GESKUIF,
    ROKEER_STUKKE_TUSSENIN,
    ROKEER_SKUIF_UIT_SKAAK,
    ROKEER_SKUIF_OOR_BEDREIGDE_BLOKKIE,
    ROKEER_KONING_IN_SKAAK,
    DUBBELSINNIG,
    GEEN_MOONTLIKE_SKUIF,
    SKUIF_IN_SKAAK,
    BUIT_EIE_STUK,
    ONWETTIGE_TORINGBEWEGING,
    ONWETTIGE_RUITERBEWEGING,
    ONWETTIGE_LOPERBEWEGING,
    ONWETTIGE_DAMEBEWEGING,
    ONWETTIGE_KONINGBEWEGING,
    ONWETTIGE_PIONBEWEGING
};

bool isOorgee(const std::string &skuif)
{
    return (skuif == "Gee oor.");
}

bool isRokeer(const std::string &skuif)
{
    return (skuif == "0-0") || (skuif == "0-0-0");
}

bool isStuk(char stuk)
{
    return (stuk == 'K') || (stuk == 'D') || (stuk == 'T') || (stuk == 'R') || (stuk == 'L');
}

bool isRy(char ry)
{
    return ry >= 'a' && ry <= 'h';
}

bool isGelid(char gelid)
{
    return gelid >= '1' && gelid <= '8';
}

bool isBuit(char buit)
{
    return buit == 'X';
}

bool isKoppelteken(char koppelteken)
{
    return koppelteken == '-';
}

bool isSpasie(char spasie)
{
    return spasie == ' ';
}

bool isPromoveer(char isGelykAan)
{
    return isGelykAan == '=';
}

bool isEnPassant(const std::string &enPassant)
{
    return enPassant == "e.p.";
}

bool isSkaak(char skaak)
{
    return (skaak == '+');
}

bool isMat(const std::string &mat)
{
    return (mat == "mat");
}

bool isPat(const std::string &pat)
{
    return (pat == "pat");
}

bool isKorrekteGelid(bool isWit, bool isEnPassant, char gelid)
{
    if (isEnPassant)
        return gelid == isWit ? '6' : '3';
    else
        return gelid == isWit ? '8' : '1';
}

bool isWettigeToringBeweging(Koordinaat beginpos, Koordinaat eindpos)
{
    return  (eindpos.ry != beginpos.ry && eindpos.gelid == beginpos.gelid) ||
            (eindpos.gelid != beginpos.gelid && eindpos.ry == beginpos.ry);
}

bool isWettigeRuiterBeweging(Koordinaat beginpos, Koordinaat eindpos)
{
    return ((std::abs(eindpos.ry - beginpos.ry == 1) &&
             std::abs(eindpos.gelid - beginpos.gelid == 2)) ||
            (std::abs(eindpos.ry - beginpos.ry == 2) &&
             std::abs(eindpos.gelid - beginpos.gelid == 1)));
}

bool isWettigeLoperBeweging(Koordinaat beginpos, Koordinaat eindpos)
{
    return (std::abs(eindpos.ry - beginpos.ry) == std::abs(eindpos.gelid - beginpos.gelid));
}

bool isWettigeDameBeweging(Koordinaat beginpos, Koordinaat eindpos)
{
    return isWettigeToringBeweging(beginpos, eindpos) && isWettigeLoperBeweging(beginpos, eindpos);
}

bool isWettigeKoningBeweging(Koordinaat beginpos, Koordinaat eindpos)
{
    return (isWettigeDameBeweging(beginpos, eindpos) && std::abs(eindpos.ry - beginpos.ry) <= 1 &&
            std::abs(eindpos.gelid - beginpos.gelid) <= 1);
}

bool isWettigePionBeweging(bool isWit, bool isBuitskuif,
                           Koordinaat beginpos, Koordinaat  eindpos)
{
    if (isBuitskuif)
        return (isWit ? eindpos.gelid == beginpos.gelid + 1u :
                eindpos.gelid == beginpos.gelid - 1u &&
                std::abs(eindpos.ry - beginpos.ry) == 1u);

    else if (eindpos.gelid - beginpos.gelid == isWit ? 2 : -2)
        return (beginpos.gelid == isWit ? 1u : 6u &&
                eindpos.ry - beginpos.ry == 0u);
    else
        return (static_cast<int>(eindpos.gelid) -
                    static_cast<int>(beginpos.gelid) ==
                    (isWit ? 1 : -1) &&
                    eindpos.ry - beginpos.ry == 0u);
}

bool isWettigeBeweging(bool isWit, Soort soort, bool isBuitskuif,
                       Koordinaat beginpos, Koordinaat eindpos)
{
    switch (soort)
    {
        case Soort::TORING:
            return isWettigeToringBeweging(beginpos, eindpos);
        case Soort::RUITER:
            return isWettigeRuiterBeweging(beginpos, eindpos);
        case Soort::LOPER:
            return isWettigeLoperBeweging(beginpos, eindpos);
        case Soort::DAME:
            return isWettigeDameBeweging(beginpos, eindpos);
        case Soort::KONING:
            return isWettigeKoningBeweging(beginpos, eindpos);
        case Soort::PION:
            return isWettigePionBeweging(isWit, isBuitskuif, beginpos, eindpos);
        default:
            return false;
    }
}

Koordinaat mapStringNaKoordinaat(const std::string &koordinaat)
{
    Koordinaat resultaat {};
    // Map 'a' na 0, 'b' na 1, ens.
    resultaat.ry = static_cast<uint_t>(koordinaat.at(0u) - ('a'));
    // Map '1' na 0, '2' na 1, ens.
    resultaat.gelid = static_cast<uint_t>(koordinaat.at(1u) - ('1'));
    return resultaat;
}

Blokkie* mapStringNaBlokkie(const std::string &koordinaat, bordArray_t &bord)
{
    // Map 'a' na 0, 'b' na 1, ens.
    uint_t ry = static_cast<uint_t>(koordinaat.at(0u) - ('a'));
    // Map '1' na 0, '2' na 1, ens.
    uint_t gelid = static_cast<uint_t>(koordinaat.at(1u) - ('1'));
    return &bord.at(gelid).at(ry);
}

void geeFoutboodskap(Fout fout, bool geldigRoep=false, int pos=0)
{
    std::string foutboodskap;
    switch (fout)
    {
        case Fout::TE_KORT: foutboodskap = "skuif is te kort."; break;
        case Fout::TE_LANK: foutboodskap = "skuif is te lank."; break;
        case Fout::BEGIN:
            foutboodskap = "ongeldige beginkarakter.\n"
                "Die skuif moet met 'n stuk ('T', 'R', ens.), ry ('a' - 'h'),\n"
                "rokade (0-0 of 0-0-0) of oorgee (Gee oor.) begin.";
            break;
        case Fout::STUK:
            foutboodskap = "ongeldige karakter na stuk.\n"
                "'n Stuk moet deur 'n ry ('a' - 'h'), buitsimbool ('X')\n"
                "of koppelteken ('-') gevolg word.";
            break;
        case Fout::RY:
            foutboodskap = "ongeldige karakter na ry.\n"
                "'n Ry kan net deur 'n gelid ('1' - '8') gevolg word.";
            break;
        case Fout::GELID:
            foutboodskap = "ongeldige karakter na gelid.\n"
                "'n Gelid moet deur 'n buitsimbool ('X'), koppelteken ('-'),\n"
                "spasie (' '), promoveerskuif ('=') of skaak ('+') gevolg word.";
            break;
        case Fout::BUIT:
            foutboodskap = "ongeldige karakter na buitsimbool.\n"
                "'n Buitsimbool moet deur 'n ry ('a' - 'h') gevolg word.";
            break;
        case Fout::KOPPELTEKEN:
            foutboodskap = "ongeldige karakter na koppelteken.\n"
                "'n Koppelteken moet deur 'n ry ('a' - 'h') gevolg word.";
            break;
        case Fout::PROMOVEER_NIE_NA_STUK:
            foutboodskap = "ongeldige karakter na promoveerskuif.\n"
                "'n Promoveerskuif moet deur 'n stuk ('T', 'R', ens.) gevolg word.";
            break;
        case Fout::PROMOVEER_NIE_OP_LAASTE_GELID:
            foutboodskap = "kan nie op die middel van die bord promoveer nie.\n"
                "'n Pion kan slegs op die laaste gelid (8 vir Wit en 1 vir Swart) promoveer.";
            break;
        case Fout::SKAAK:
            foutboodskap = "ongeldige karakter na skaak.\n"
                "'n Skaak kan slegs deur nog 'n skaak ('+') gevolg word.";
            break;
        case Fout::SPASIE:
            foutboodskap = "ongeldige string na spasie.\n"
                "'n Spasie kan slegs deur \"mat\", \"pat\"\n of \"e.p.\" gevolg word.";
            break;
        case Fout::EN_PASSANT_NIE_NA_6DE_GELID:
            foutboodskap = "ongeldige en passant-buit.\n"
                "'n Pion kan slegs en passant buit vanaf sy vyfde gelid na sy sesde gelid.";
            break;
        case Fout::TE_VEEL_KOORDINATE:
            foutboodskap = "te veel koördinate in die skuif.\n"
                "'n Skuif kan 'n maksimum van twee koördinate bevat: een oorsprong en een bestemming.";
            break;
        case Fout::TE_VEEL_BUIT:
            foutboodskap = "te veel buitsimbole ('X') in die skuif.\n"
                "'n Skuif kan 'n maksimum van een buitsimbool bevat.";
            break;
        case Fout::TE_VEEL_KOPPELTEKEN:
            foutboodskap = "te veel koppeltekens ('-') in die skuif.\n"
                "'n Skuif kan 'n maksimum van een koppelteken bevat";
            break;
        //case Fout::
        case Fout::ONBEKEND:    //Val deur.
        default:
            foutboodskap = "Onbekende fout.";
    }

    foutboodskap = " " + std::string { geldigRoep ? "Ongeldige" : "Onwettige" } +
        " skuif: " + foutboodskap;

    if (pos != 0)
        foutboodskap += " " + std::to_string(pos) + "e karakter foutief.";

    std::cout << foutboodskap;
}

Fout bepaalOnwettigeBeweging(Soort soort)
{
    switch (soort)
    {
        case Soort::TORING: return  Fout::ONWETTIGE_TORINGBEWEGING;
        case Soort::RUITER: return  Fout::ONWETTIGE_RUITERBEWEGING;
        case Soort::LOPER:  return  Fout::ONWETTIGE_LOPERBEWEGING;
        case Soort::DAME:   return  Fout::ONWETTIGE_DAMEBEWEGING;
        case Soort::KONING: return  Fout::ONWETTIGE_KONINGBEWEGING;
        case Soort::PION:   return  Fout::ONWETTIGE_PIONBEWEGING;
        default:            return  Fout::ONBEKEND;
    }
}

bool isGeldig(bool isWit, const std::string &skuif)
{
    Skuif::g_beginPos.gelid = 8u;
    Skuif::g_eindPos.gelid  = 8u;
    const uint_t skuifLengte { skuif.length() };
    Fout fout;
    bool geldig { (skuifLengte >= 2u) || (skuifLengte <= 14u) };
    if (!geldig)
        fout = ((skuifLengte < 2u) ? Fout::TE_KORT : Fout::TE_LANK);

    int i { -1 };

    if (geldig)
    {
        int koordinaatTal { 0 };
        int buitTal { 0 };
        int koppeltekenTal { 0 };

        //  Enumerator Karakter hou tred met watter soort karakter die vorige karakter in die skuif was.
        Karakter vorige { Karakter::BEGIN };
        bool promoveerOpLaasteGelid { false };
        for (uint_t k { 0u }; k < skuifLengte; ++k)
        {
            if (!geldig)
                break;

            std::string stert { "" };

            switch (vorige)
            {
            case Karakter::BEGIN:   //  Voer uit as huidige karakter (skuif.at(k)) die eerste karakter van die string is.
                geldig = isRokeer(skuif) || isOorgee(skuif) || isStuk(skuif.at(k)) ||
                    isRy(skuif.at(k));
                break;

            case Karakter::STUK:    // Voer uit as huidige karakter (skuif.at(k)) op 'n stuk volg.
                geldig = (isRy(skuif.at(k)) && koordinaatTal < 2) || isBuit(skuif.at(k)) ||
                    isSpasie(skuif.at(k));
                break;

            case Karakter::RY:
                geldig = isGelid(skuif.at(k));
                break;

            case Karakter::GELID:  // Voer uit as huidige karakter (skuif.at(k)) op 'n koördinaat volg.
                geldig = ((isBuit(skuif.at(k)) || isKoppelteken(skuif.at(k))) &&
                          (buitTal + koppeltekenTal) == 0) || isSpasie(skuif.at(k)) ||
                          (isPromoveer(skuif.at(k)) && koordinaatTal <= 2) || isSkaak(skuif.at(k));
                break;

            case Karakter::BUIT:    // Voer uit as huidige karakter (skuif.at(k) op 'n 'X' volg.
                geldig = isRy(skuif.at(k)) && (koordinaatTal < 2);
                break;

            case Karakter::KOPPELTEKEN: // Voer uit as huidige karakter (skuif.at(k) op 'n koppelteken volg.
                geldig = isRy(skuif.at(k)) && (koordinaatTal == 1);
                break;

            case Karakter::PROMOVEER:  // Voer uit as huidige karakter (skuif.at(k) op 'n '=' volg.
                // 'n Pion kan net promoveer op die laaste gelid. As die koördinaat 'n ander gelid gee,
                // lê die fout reeds by daardie karakter in die skuifstring.
                promoveerOpLaasteGelid = isKorrekteGelid(isWit, false, skuif.at(k - 2u));
                if (!promoveerOpLaasteGelid)
                    k -= 2u;
                geldig = promoveerOpLaasteGelid && isStuk(skuif.at(k)) &&
                    (k = skuifLengte - 1 || isSpasie(skuif.at(k + 1u)) || isSkaak(skuif.at(k + 1u)));
                break;

            case Karakter::SKAAK:   // Voer uit as huidige karakter (skuif.at(k) op 'n '+' volg.
                geldig = isSkaak(skuif.at(k)) && ((skuifLengte - k) == 1u);
                break;

            case Karakter::SPASIE:  // Voer uit as huidige karakter (skuif.at(k) op 'n spasie volg.
                stert = skuif.substr(k, skuifLengte - k);
                geldig =  (isEnPassant(stert) && isKorrekteGelid(isWit, true, skuif.at(k - 2u)) &&
                           buitTal == 1) || isMat(stert) || isPat(stert);
                break;

            default:
                geldig = false;
            }
            if (geldig)
            {
                // As enige van die volgende waar is, het ek reeds alles tot by die einde
                // van die string getoets, al is die lusteller (k) nog nie gelyk aan
                // skuifLengte nie, so ek moet dus eksplisiet uit die lus uitbreek.
                if (isRokeer(skuif) || isOorgee(skuif) || isEnPassant(stert) ||
                    isMat(stert) || isPat(stert))
                    break;

                //  Wat die huidige karakter is, bepaal wat die volgende karakter mag wees.
                if (isRy(skuif.at(k)))
                {
                    ++koordinaatTal;
                    vorige = Karakter::RY;
                    fout = Fout::RY;
                }
                else if (isGelid(skuif.at(k)))
                {
                    if ((buitTal + koppeltekenTal) == 1 && koordinaatTal == 2)
                    {
                        Koordinaat beginpos
                            { mapStringNaKoordinaat(skuif.substr(k - 4u, 2)) };
                        Koordinaat eindpos
                            { mapStringNaKoordinaat(skuif.substr(k - 1u, 2)) };
                        geldig = isWettigeBeweging(isWit, bepaalSoort(skuif.at(0u)),
                                                   buitTal > 0, beginpos, eindpos);
                        if (geldig)
                        {
                            using namespace Skuif;
                            g_beginPos = beginpos;
                            g_eindPos  = eindpos;
                        }
                    }
                    vorige = Karakter::GELID;
                    fout = geldig ? Fout::GELID :
                        bepaalOnwettigeBeweging(bepaalSoort(skuif.at(0u)));
                }
                else if (isStuk(skuif.at(k)))
                {
                    vorige = Karakter::STUK;
                    fout = Fout::STUK;
                }
                else if (isBuit(skuif.at(k)))
                {
                    ++buitTal;
                    vorige = Karakter::BUIT;
                    fout = Fout::BUIT;
                }
                else if (isKoppelteken(skuif.at(k)))
                {
                    ++koppeltekenTal;
                    vorige = Karakter::KOPPELTEKEN;
                    fout = Fout::KOPPELTEKEN;
                }
                else if (isSpasie(skuif.at(k)))
                {
                    vorige = Karakter::SPASIE;
                    fout = Fout::SPASIE;
                }
                else if (isPromoveer(skuif.at(k)))
                {
                    vorige = Karakter::PROMOVEER;
                    fout = promoveerOpLaasteGelid ? Fout::PROMOVEER_NIE_NA_STUK :
                        Fout::PROMOVEER_NIE_OP_LAASTE_GELID;
                }
                else if (isSkaak(skuif.at(k)))
                {
                    vorige = Karakter::SKAAK;
                    fout = Fout::SKAAK;
                }
            }
            else
            {
                if (k == 0u)
                    fout = Fout::BEGIN;
                if (koordinaatTal == 2 && isRy(skuif.at(k)))
                    fout = Fout::TE_VEEL_KOORDINATE;
                else if ((buitTal + koppeltekenTal) == 1 &&
                         (isBuit(skuif.at(k) || isKoppelteken(skuif.at(k)))))
                    fout = isBuit(skuif.at(k)) ? Fout::TE_VEEL_BUIT :
                        Fout::TE_VEEL_KOPPELTEKEN;
                else if (isEnPassant(stert))
                    fout = buitTal != 1 ? Fout::EN_PASSANT_NIE_BUITSKUIF :
                        Fout::EN_PASSANT_NIE_NA_6DE_GELID;
                break;
            }

            i = static_cast<int>(k);
        }
    }
    if (!geldig)
    {
        geeFoutboodskap(fout, true, i + 1);
    }
    else if (Skuif::g_eindPos.gelid == 8u)
    {
        // Nodig om die eindposisie se koördinaat nog te stel.
        for (uint_t k { 0u }; k < skuifLengte - 1u; ++k)
        {
            if (isRy(skuif.at(k)))
            {
                Skuif::g_eindPos = mapStringNaKoordinaat(skuif.substr(k, 2u));
                break;
            }
        }
    }

    return geldig;
}

Koordinaat soekStuk(Koordinaat rigting, Koordinaat beginpos, bordArray_t &bord,
                  int afstand=0)
{
    int teller { 1 };
    Koordinaat soekPos { beginpos.gelid + rigting.gelid, beginpos.ry + rigting.ry };

    // Sorg dat die indekse nie buite die omvang van die array gaan nie.
    while (soekPos.gelid < g_sylengte && soekPos.ry < g_sylengte)
    {
        // As daar 'n stuk is op die posisie, return die koördinaat.
        if (bord.at(soekPos.gelid).at(soekPos.ry).stuk)
            return soekPos;

        // Soek slegs vir 'n beperkte aantal kere as so gespesifiseer is.
        if (afstand != 0 && teller == afstand)
                break;

        // Skuif die soekposisie aan.
        if (rigting.gelid)
            soekPos.gelid += rigting.gelid;
        if (rigting.ry)
            soekPos.ry += rigting.ry;

        ++teller;
    }
    return { 8u };
}

bool soekBeginPos(bool isWit, bool resultaat, Soort soort, Koordinaat rigting
                  , bordArray_t &bord, const std::string &skuif)
{
    using namespace Skuif;

    // Hoe ver moet gesoek word. 'n Koning kan net 1 blokkie skuif. 0 beteken onbeperk.
    int afstand { soort == Soort::KONING ? 1 : 0 };

    // 'n Ruiter kan ook net 1x sy sprong doen.
    if (soort == Soort::RUITER)
    {
        afstand = 1;
    }

    // 'n Pion kan soms 2 blokkies skuif, maar meestal net 1.
    else if (soort == Soort::PION)
    {
        // As die skuif nie van ry verander nie, is dit nie 'n buitskuif nie.
        // As dit na die 4e gelid vir Wit en 5e gelid vir Swart skuif, slegs dan,
        // kan dit wettig 2 blokkies ver skuif.
        if (rigting.ry == 0u && skuif.at(1u) == isWit ? '3' : '4')
            afstand = 2;
        else
            // Anders mag net 1 blokkie ver skuif.
            afstand = 1;
    }

    // Tydelike veranderlike.
    Koordinaat tydl { soekStuk(rigting, g_eindPos, bord, afstand) };

    if (tydl.gelid != 8u)
    {
        Stuk *stuk { bord.at(tydl.gelid).at(tydl.ry).stuk };

        if (stuk->isWit == isWit && stuk->soort == soort)
        {
            if (g_beginPos.gelid != 8u)
            {
                g_beginPos = tydl;
                resultaat = true;
            }
            else
            {
                geeFoutboodskap(Fout::DUBBELSINNIG);
                return false;
            }
        }
    }

    if (!resultaat)
        geeFoutboodskap(Fout::GEEN_MOONTLIKE_SKUIF);

    return resultaat;
}

bool bepaalBeginPos(bool isWit, const std::string &skuif, bordArray_t &bord)
{
    bool resultaat { false };
    Soort soort { bepaalSoort(skuif.at(0u)) };
    using namespace Skuif;

    // Daar is reeds 'n tentatiewe beginpos gevind deur isGeldig()
    if (g_beginPos.gelid != 8u)
    {
        Stuk *stuk  { bord.at(g_beginPos.gelid).at(g_beginPos.ry).stuk };
        resultaat = stuk->soort == soort && stuk->isWit == isWit;

        // Kyk of die pad oop is tussen die voorgenome begin- en eindpos.
        // 'n Ruiter kan spring, so nie nodig om vir ruiters the toets nie.
        if (soort != Soort::RUITER)
        {
            int gelid   { static_cast<int>(g_beginPos.gelid - g_eindPos.gelid) };
            int ry      { static_cast<int>(g_beginPos.ry - g_eindPos.ry) };
            Koordinaat rigting { static_cast<uint_t>(gelid / abs(gelid))
                                ,static_cast<uint_t>(ry / abs(ry)) };

            Koordinaat res { soekStuk(rigting, g_beginPos, bord) };
            resultaat = resultaat && res.gelid == g_eindPos.gelid
                && res.ry == g_eindPos.ry;
        }
    }
    else
    {
        if (soort == Soort::TORING || soort == Soort::DAME || soort == Soort::KONING)
        {
            for (int gelid { -1 }; gelid <= 1; ++gelid)
            {
                for (int ry { (gelid - 1) % 2 }; ry <= 1; ry +=2)
                {
                    resultaat = soekBeginPos(isWit, resultaat, soort
                                             , { static_cast<uint_t>(gelid)
                                             , static_cast<uint_t>(ry) }
                                             , bord, skuif);
                }
            }
        }

        if (soort == Soort::LOPER || soort == Soort::DAME || soort == Soort::KONING)
        {
            for (int gelid { -1 }; gelid <= 1; gelid += 2)
            {
                for (int ry { -1 }; ry <= 1; ry +=2)
                {
                    resultaat = soekBeginPos(isWit, resultaat, soort
                                             , { static_cast<uint_t>(gelid)
                                             , static_cast<uint_t>(ry) }
                                             , bord, skuif);
                }
            }
        }
        else if (soort == Soort::RUITER)
        {
            for (int gelid { -2 }; gelid <= 2; ++ gelid)
            {
                if (gelid == 0)
                    continue;

                int vermenigvuldiger { gelid % 2 ? 1 : 2 };

                for (int ry { -1 }; ry <= 1; ry += 2)
                {
                    resultaat = soekBeginPos(isWit, resultaat, soort
                                             , { static_cast<uint_t>(gelid)
                                             ,   static_cast<uint_t>(ry
                                                    * vermenigvuldiger) }
                                             , bord, skuif);
                }
            }
        }
        else
        {
            int gelid { isWit ? -1 : 1 };
            uint_t skuifLengte { skuif.length() };

            for (int ry { skuifLengte > 2u && isBuit(skuif.at(2u)) ? -1 : 0 }
                 ; ry <= 1; ry += 2)
            {
                resultaat = soekBeginPos(isWit, resultaat, soort
                                         , { static_cast<uint_t>(gelid)
                                         , static_cast<uint_t>(ry) }
                                         , bord, skuif);
            }
        }
    }
    return resultaat;
}

bool isRuitersBedreiging(bool isWit, Koordinaat koord, bordArray_t &bord)
{
    for (int ryVerpl { -2 }; ryVerpl <= 2; ++ryVerpl)
    {
        if (ryVerpl == 0)
            continue;

        int vermenigvuldiger { ryVerpl % 2 ? 1 : 2 };

        for (int gelidVerpl { -1 }; gelidVerpl <= 1; gelidVerpl += 2)
        {
            Koordinaat pos { soekStuk({ static_cast<uint_t>(gelidVerpl
                * vermenigvuldiger), static_cast<uint_t>(ryVerpl) }
                ,koord, bord, 1) };

            Stuk *stuk { bord.at(pos.gelid).at(pos.ry).stuk };

            if (stuk->isWit == !isWit && stuk->soort == Soort::RUITER)
            {
                return true;
            }
        }
    }

    return false;
}

bool isDiagonaleBedreiging(bool isWit, Koordinaat koordinaat, bordArray_t &bord)
{
    for (int gelid { -1 }; gelid <= 1; gelid += 2)
    {
        for (int ry { -1 }; ry <= 1; ry += 2)
        {
            Koordinaat pos { soekStuk({static_cast<uint_t>(gelid)
                                      ,static_cast<uint_t>(ry)}
                                      ,koordinaat, bord) };

            Stuk *stuk { bord.at(pos.gelid).at(pos.ry).stuk };

            if (stuk->isWit != isWit)
            {
                if (stuk->soort == Soort::DAME || stuk->soort == Soort::LOPER)
                {
                    return true;
                }
                else if (stuk->soort == Soort::KONING)
                {
                    return isWettigeKoningBeweging(pos, koordinaat);
                }
                else if (stuk->soort == Soort::PION)
                {
                    return isWettigePionBeweging(isWit, true, pos, koordinaat);
                }
            }
        }
    }

    return false;
}

bool isParallelleBedreiging(bool isWit, Koordinaat koordinaat, bordArray_t &bord)
{
    for (int gelid { -1 }; gelid <= 1; ++gelid)
    {
        for (int ry { (gelid - 1) % 2 }; ry <= -ry; ry += 2)
        {
            Koordinaat pos { soekStuk({static_cast<uint_t>(gelid)
                                      ,static_cast<uint_t>(ry)}
                                      ,koordinaat, bord) };

            Stuk *stuk { bord.at(pos.gelid).at(pos.ry).stuk };

            if (stuk->isWit != isWit)
            {
                if (stuk->soort == Soort::DAME || stuk->soort == Soort::TORING)
                {
                    return true;
                }
                else if (stuk->soort == Soort::KONING)
                {
                    return isWettigeKoningBeweging(pos, koordinaat);
                }
            }
        }
    }

    return false;
}

bool isBlokkieBedreig(bool isWit, Koordinaat koordinaat, bordArray_t &bord)
{
    return isRuitersBedreiging(isWit, koordinaat, bord)
        || isDiagonaleBedreiging(isWit, koordinaat, bord)
        || isParallelleBedreiging(isWit, koordinaat, bord);
}

bool isWettig(bool isWit, const std::string &skuif, bordArray_t &bord)
{
    // Hierdie static bools dui aan of elk van die 4 torings en 2 konings al geskuif het.
    // True beteken 'n rokade is nog wettig (die stuk het nog nie geskuif nie).
    static bool witKoning { true };
    static bool witKortToring { true };
    static bool witLangToring { true };
    static bool swartKoning { true };
    static bool swartKortToring { true };
    static bool swartLangToring { true };

    if (isRokeer(skuif))
    {
        bool kortRokade { skuif == "0-0" };

        // Inisialiseer met die resultaat of die koning al geskuif het.
        bool rokadeWettig { isWit ? witKoning : swartKoning };
        const uint_t gelid { isWit ? 0u : 7u };

        if (rokadeWettig)   // As die eerste toets slaag...
        {
            // doen die tweede toets ;-) (of die betrokke toring al geskuif het.)
            if (kortRokade)
                rokadeWettig = isWit ? witKortToring : swartKortToring;
            else
                rokadeWettig = isWit ? witLangToring : swartLangToring;

            if (rokadeWettig)   // As die tweede toets slaag...
            {
                if (kortRokade)     // Toets of die bord oop is tussen Koning en Toring. (Toets 3)
                    rokadeWettig = !(bord.at(gelid).at(5u).stuk &&
                                     bord.at(gelid).at(6u).stuk);
                else
                    rokadeWettig = !(bord.at(gelid).at(3u).stuk &&
                                     bord.at(gelid).at(2u).stuk &&
                                     bord.at(gelid).at(1u).stuk);

                if (rokadeWettig)   // Toets of die Koning in skaak is. (Toets 4)
                {
                    Koordinaat blokkie { 4u, gelid };
                    rokadeWettig = !isBlokkieBedreig(isWit, blokkie, bord);

                    if (rokadeWettig)   // Toets of die Koning oor 'n bedreigde blokkie spring. (Toets 5)
                    {
                        blokkie.ry = kortRokade ? 5u : 3u;
                        rokadeWettig = !isBlokkieBedreig(isWit, blokkie, bord);

                        if (rokadeWettig)   // Toets of die koning in skaak in skuif. (Toets 6)
                        {
                            blokkie.ry = kortRokade ? 6u : 2u;
                            rokadeWettig = !isBlokkieBedreig(isWit, blokkie, bord);

                            if (!rokadeWettig)  // As die sesde toets dop:
                                geeFoutboodskap(Fout::SKUIF_IN_SKAAK);
                        }
                        else    // As die vyfde toets dop:
                        {
                            geeFoutboodskap(Fout::ROKEER_SKUIF_OOR_BEDREIGDE_BLOKKIE);
                        }
                    }
                    else    // As die vierde toets dop:
                    {
                        geeFoutboodskap(Fout::ROKEER_KONING_IN_SKAAK);
                    }
                }
                else    // As die derde toets dop:
                {
                    geeFoutboodskap(Fout::ROKEER_STUKKE_TUSSENIN);
                }
            }
            else    // As die tweede toets dop:
            {
                geeFoutboodskap(Fout::ROKEER_TORING_REEDS_GESKUIF);
            }
        }
        else    // As die eerste toets dop:
        {
            geeFoutboodskap(Fout::ROKEER_KONING_REEDS_GESKUIF);
        }

        return rokadeWettig;
    }
    else
    {
        using namespace Skuif;

        // Inisialiseer met of 'n wettige beginpos gevind is.
        bool skuifWettig { bepaalBeginPos(isWit, skuif, bord) };

        if (skuifWettig)
        {
            g_beginBlokkie = &bord.at(g_beginPos.gelid).at(g_beginPos.ry);
            g_eindBlokkie  = &bord.at(g_eindPos.gelid).at(g_eindPos.ry);
        }


        return skuifWettig;
    }
}

const std::string& krySkuif(bool isWit, bordArray_t &bord/*,
                            const stukArray_t &stukke*/)
{
    // Bepaal watter kleur volgende moet skuif om die spelers te herinner.
    const std::string kleur { (isWit) ? "WIT" : "SWART" };
    static std::string skuif;

    do
    {
        std::cout << kleur << " skuif: ";
        std::getline(std::cin, skuif);
        std::cout << "\n";

    } while (!(isGeldig(isWit, skuif) && (isOorgee(skuif)
                                          || isWettig(isWit, skuif, bord))));

    return skuif;
}

void doenSkuif(bool isWit, const std::string &skuif, bordArray_t &bord,
               buitVector_t &gebuiteStukke)
{
    if (isRokeer(skuif))
    {
        const uint_t gelid { isWit ? 0u : 7u };
        if (skuif == "0-0") // Kortrokade
        {
            bord.at(gelid).at(6u).stuk = bord.at(gelid).at(4u).stuk;
            bord.at(gelid).at(4u).stuk = nullptr;

            bord.at(gelid).at(5u).stuk = bord.at(gelid).at(7u).stuk;
            bord.at(gelid).at(7u).stuk = nullptr;
        }
        else                // Langrokade
        {
            bord.at(gelid).at(2u).stuk = bord.at(gelid).at(4u).stuk;
            bord.at(gelid).at(4u).stuk = nullptr;

            bord.at(gelid).at(3u).stuk = bord.at(gelid).at(0u).stuk;
            bord.at(gelid).at(0u).stuk = nullptr;
        }

    }
    else
    {
        using namespace Skuif;
        // Daar is 'n stuk by die bestemming wat gebuit word.
        if (g_eindBlokkie->stuk)
            // Las die stuk wat gebuit word aan die vektor van gebuite stukke.
            gebuiteStukke.push_back(g_eindBlokkie->stuk);

        g_eindBlokkie->stuk  = g_beginBlokkie->stuk;
        g_beginBlokkie->stuk = nullptr;

    }
}

void skryfSkuif(bool isSwart, const std::string &skuif)
{
    static int skuiwe { 1 };

    std::cout << "\t" << skuiwe << ".\t" << (isSwart ? "...\t" : "") << skuif << "\n\n";

    if (isSwart)
        ++skuiwe;
}
