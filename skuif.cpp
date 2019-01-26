#include <iostream>
#include <string>
#include <cmath>
#include <utility>
#include "skuif.h"

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

bool isWettigeToringBeweging(const Koordinaat* const beginpos,
                             const Koordinaat* const eindpos)
{
    return  (eindpos->ry != beginpos->ry && eindpos->gelid == beginpos->gelid) ||
            (eindpos->gelid != beginpos->gelid && eindpos->ry == beginpos->ry);
}

bool isWettigeRuiterBeweging(const Koordinaat* const beginpos,
                             const Koordinaat* const eindpos)
{
    return ((std::abs(eindpos->ry - beginpos->ry == 1) &&
             std::abs(eindpos->gelid - beginpos->gelid == 2)) ||
            (std::abs(eindpos->ry - beginpos->ry == 2) &&
             std::abs(eindpos->gelid - beginpos->gelid == 1)));
}

bool isWettigeLoperBeweging(const Koordinaat* const beginpos,
                            const Koordinaat* const eindpos)
{
    return (std::abs(eindpos->ry - beginpos->ry) == std::abs(eindpos->gelid - beginpos->gelid));
}

bool isWettigeDameBeweging(const Koordinaat* const beginpos,
                           const Koordinaat* const eindpos)
{
    return isWettigeToringBeweging(beginpos, eindpos) && isWettigeLoperBeweging(beginpos, eindpos);
}

bool isWettigeKoningBeweging(const Koordinaat* const beginpos,
                             const Koordinaat* const eindpos)
{
    return (isWettigeDameBeweging(beginpos, eindpos) && std::abs(eindpos->ry - beginpos->ry) <= 1 &&
            std::abs(eindpos->gelid - beginpos->gelid) <= 1);
}

bool isWettigePionBeweging(bool isWit, bool isBuitskuif,
                           const Koordinaat* const beginpos,
                           const Koordinaat* const eindpos)
{
    if (isBuitskuif)
        return (isWit ? eindpos->gelid == beginpos->gelid + 1 :
                eindpos->gelid == beginpos->gelid - 1 &&
                std::abs(eindpos->ry - beginpos->ry) == 1);

    else if (eindpos->gelid - beginpos->gelid == isWit ? 2 : -2)
        return (beginpos->gelid == isWit ? 2 : 7 &&
                eindpos->ry - beginpos->ry == 0);
    else
        return (static_cast<int>(eindpos->gelid) -
                    static_cast<int>(beginpos->gelid) ==
                    (isWit ? 1 : -1) &&
                    eindpos->ry - beginpos->ry == 0u);
}

bool isWettigeBeweging(bool isWit, Soort soort, bool isBuitskuif,
                       const Koordinaat* const beginpos,
                       const Koordinaat* const eindpos)
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

Koordinaat* mapStringNaKoordinaat(const std::string &koordinaat)
{
    Koordinaat *resultaat { new Koordinaat };
    // Map 'a' na 1, 'b' na 2, ens.
    resultaat->ry = static_cast<uint_t>(koordinaat.at(0u) - ('a' - 1));
    // Map '1' na 1, '2' na 2, ens.
    resultaat->gelid = static_cast<uint_t>(koordinaat.at(1u) - ('1' - 1));
    return resultaat;
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
            i = static_cast<int>(k);
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
                        const Koordinaat* const beginpos
                            { mapStringNaKoordinaat(skuif.substr(k - 4, 2)) };
                        const Koordinaat* const eindpos
                            { mapStringNaKoordinaat(skuif.substr(k - 1, 2)) };
                        geldig = isWettigeBeweging(isWit, bepaalSoort(skuif.at(0u)),
                                                   buitTal > 0, beginpos, eindpos);

                        delete beginpos;
                        delete eindpos;
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
        }
    }
    if (!geldig)
        geeFoutboodskap(fout, true, i + 1);

    return geldig;
}

// Hierdie funksie kan slegs vir nie-rokeerskuiwe geroep word. Dit bepaal die bestemming waarheen 'n stuk
// geskuif moet word uit die gegewe (geldige en wettige) skuif. Elke geldige skuif (behalwe rokades) moet
// een of twee koördinate hê. As daar net een koördinaat is, is dit die bestemming se koördinaat. As daar
// twee koördinate is, is die laaste een die bestemming se koördinaat. Daarom stap ek deur die string van
// die einde af en beskou die eerste koördinaat wat ek teëkom, as die bestemming.
Koordinaat* bepaalBestemming(const std::string &skuif)
{
    for (int i { static_cast<int>(skuif.length()) - 1 }; i >= 0; --i)
    {
        if (isGelid(skuif.at(static_cast<uint_t>(i))))
            return mapStringNaKoordinaat(skuif.substr(static_cast<uint_t>(i) - 1, 2));
    }

    return nullptr;
}

Koordinaat* bepaalOorsprong(bool isWit, const std::string &skuif, Koordinaat *bestemming,
                            std::array<std::array<Blokkie, g_sylengte>,g_sylengte> &bord)
{
    const uint_t skuifLengte { skuif.length() };

    if (skuifLengte > 5u)
    {
        bool koordinaatGevind { false };
        std::string koordinaat { "" };

        for (uint_t i { 0u }; i < (skuifLengte - 1u); ++i)
        {
            if (isRy(skuif.at(i)))
            {
                if (koordinaatGevind)
                {
                    koordinaat = skuif.substr(i, 2);
                    return mapStringNaKoordinaat(koordinaat);
                }
                else
                {
                    koordinaatGevind = true;
                }
            }
        }
    }

    switch (bepaalSoort(skuif.at(0u)))
    {
        case Soort::TORING:
            ;
    }
}

bool toetsBlokkie(bool isWit, uint_t gelid, uint_t ry, Soort soort,
                  std::array<std::array<Blokkie, g_sylengte>, g_sylengte> &bord)
{
    Stuk *stuk { bord.at(gelid).at(ry).stuk };
    return stuk && stuk->soort == soort && stuk->isWit != isWit;
}

bool isRuitersBedreiging(bool isWit, const Koordinaat &koordinaat,
                         std::array<std::array<Blokkie, g_sylengte>, g_sylengte> &bord)
{
    uint_t gelid;

    for (uint_t ry { koordinaat.ry - 2u }; ry <= koordinaat.ry + 2u; ++ry)
    {
        if (ry > 0u && ry < 9u)
        {
            if ((koordinaat.ry - ry) % 2u == 0u)  // As ry == +/- 2
            {
                gelid = koordinaat.gelid - 1u;
                if (toetsBlokkie(isWit, gelid, ry, Soort::RUITER, bord))
                    return true;
                gelid = koordinaat.gelid + 1u;
                if (toetsBlokkie(isWit, gelid, ry, Soort::RUITER, bord))
                    return true;
            }
            else
            {
                gelid = koordinaat.gelid - 2u;
                if (toetsBlokkie(isWit, gelid, ry, Soort::RUITER, bord))
                    return true;
                gelid = koordinaat.gelid + 2u;
                if (toetsBlokkie(isWit, gelid, ry, Soort::RUITER, bord))
                    return true;
            }
        }
    }
    return false;
}

bool isBlokkieBedreig(bool isWit, const Koordinaat &koordinaat,
                      std::array<std::array<Blokkie, g_sylengte>, g_sylengte> &bord)
{
    bool isBedreig { isRuitersBedreiging(isWit, koordinaat, bord) };

    if (!isBedreig)
    {

    }
    return isBedreig;
}

bool isWettig(bool isWit, const std::string &skuif,
              std::array<std::array<Blokkie, g_sylengte>, g_sylengte> &bord,
              Koordinaat *bestemming, Koordinaat *oorsprong)
{
    // Hierdie static bools dui aan of elk van die 4 torings en 2 konings al geskuif het.
    // True beteken 'n rokade is nog wettig (die stuk het nog nie geskuif nie.
    static bool witKoning { true };
    static bool witKortToring { true };
    static bool witLangToring { true };
    static bool swartKoning { true };
    static bool swartKortToring { true };
    static bool swartLangToring { true };

    if (isOorgee(skuif))
    {
        return true;
    }
    else if (isRokeer(skuif))
    {
        bool kortRokade { skuif == "0-0" };

        // Inisialiseer met die resultaat of die koning al geskuif het.
        bool rokadeWettig { isWit ? witKoning : swartKoning };
        const uint_t gelid { isWit ? 1u : 8u };

        if (rokadeWettig)   // As die eerste toets slaag...
        {
            // doen die tweede toets ;-) (of die betrokke toring al geskuif het.)
            if (kortRokade)
                rokadeWettig = isWit ? witKortToring : swartKortToring;
            else
                rokadeWettig = isWit ? witLangToring : swartLangToring;

            if (rokadeWettig)   // As die tweede toets slaag...
            {
                if (kortRokade)     // Toets of die bord oop is tussen Koning en Toring.
                    rokadeWettig = !(bord.at(gelid).at(6u).stuk &&
                                     bord.at(gelid).at(7u).stuk);
                else
                    rokadeWettig = !(bord.at(gelid).at(4u).stuk &&
                                     bord.at(gelid).at(3u).stuk &&
                                     bord.at(gelid).at(2u).stuk);

                if (rokadeWettig)   // Toets of die Koning in skaak is.
                {

                    Koordinaat blokkie { 5u, gelid };
                    rokadeWettig = !isBlokkieBedreig(isWit, blokkie, bord);
                    if (rokadeWettig)   // Toets of die Koning oor 'n bedreigde blokkie spring.
                    {
                        blokkie.ry = kortRokade ? 6u : 4u;
                        rokadeWettig = !isBlokkieBedreig(isWit, blokkie, bord);
                        if (rokadeWettig)   // Toets of die koning in skaak in skuif.
                        {
                            blokkie.ry = kortRokade ? 7u : 3u;
                            rokadeWettig = !isBlokkieBedreig(isWit, blokkie, bord);
                            if (!rokadeWettig)
                                geeFoutboodskap(Fout::SKUIF_IN_SKAAK);
                        }
                        else
                        {
                            geeFoutboodskap(Fout::ROKEER_SKUIF_OOR_BEDREIGDE_BLOKKIE);
                        }
                    }
                    else
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
    else    // Dis 'n gewone nie-rokeerskuif.
    {
        Stuk *bestemmingStuk { bord.at(bestemming->gelid).at(bestemming->ry).stuk };
        if (bestemmingStuk && bestemmingStuk->isWit == isWit)
        {
            geeFoutboodskap(Fout::BUIT_EIE_STUK);
            return false;
        }
        else
        {
            if
        }
        return true;
    }
}

namespace Skuif
{
    static Koordinaat *bestemming;
    static Koordinaat *oorsprong;
}

void maakSkoon(Koordinaat *koordinaat)
{
    if (koordinaat)
    {
        delete koordinaat;
        koordinaat = nullptr;
    }
}

const std::string krySkuif(bool isWit, std::array<std::array<Blokkie, g_sylengte>,
                           g_sylengte> &bord)
{
    using namespace Skuif;
    // Bepaal watter kleur volgende moet skuif om die spelers te herinner.
    const std::string kleur { (isWit) ? "WIT" : "SWART" };
    std::string skuif;
    bool isGeldigeSkuif;

    do
    {
        std::cout << kleur << " skuif: ";
        std::getline(std::cin, skuif);
        std::cout << "\n";

        isGeldigeSkuif = isGeldig(isWit, skuif);

        if (isGeldigeSkuif && !(isRokeer(skuif) || isOorgee(skuif)))
        {
            maakSkoon(bestemming);
            maakSkoon(oorsprong);

            bestemming = bepaalBestemming(skuif);
            oorsprong = bepaalOorsprong(isWit, skuif, bestemming, bord);
        }

    } while (!(isGeldigeSkuif && isWettig(isWit, skuif, bord, bestemming, oorsprong)));

    return skuif;
}

void doenSkuif(bool isWit, const std::string &skuif,
               std::array<std::array<Blokkie, g_sylengte>, g_sylengte> &bord,
               std::vector<const Stuk*> &gebuiteStukke)
{
    using namespace Skuif;
    if (isOorgee(skuif))
    {
        return;
    }
    else if (isRokeer(skuif))
    {
        const uint_t gelid { isWit ? 1u : 8u };
        if (skuif == "0-0") // Kortrokade
        {
            std::swap(bord.at(gelid).at(7u).stuk, bord.at(gelid).at(5u).stuk);
            std::swap(bord.at(gelid).at(6u).stuk, bord.at(gelid).at(8u).stuk);
        }
        else                // Langrokade
        {
            std::swap(bord.at(gelid).at(3u).stuk, bord.at(gelid).at(5u).stuk);
            std::swap(bord.at(gelid).at(4u).stuk, bord.at(gelid).at(8u).stuk);
        }
    }
    else
    {
        Stuk *bestemmingStuk { bord.at(bestemming->gelid).at(bestemming->ry).stuk };
        if (bestemmingStuk)
        {
            // Las die stuk wat gebuit word aan die vektor van gebuite stukke.
            gebuiteStukke.push_back(bestemmingStuk);
            bestemmingStuk = nullptr;
        }

        std::swap(bord.at(oorsprong->gelid).at(oorsprong->ry).stuk, bestemmingStuk);
    }
    maakSkoon(bestemming);
    maakSkoon(oorsprong);
}

void skryfSkuif(bool isSwart, const std::string &skuif)
{
    static int skuiwe { 1 };

    std::cout << "\t" << skuiwe << ".\t" << (isSwart ? "...\t" : "") << skuif << "\n\n";

    if (isSwart)
        ++skuiwe;
}
