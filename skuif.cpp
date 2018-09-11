#include <iostream>
#include <string>
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
    PROMOVEER_NIE_NA_STUK,
    PROMOVEER_NIE_OP_LAASTE_GELID,
    SKAAK,
    SPASIE,
    EN_PASSANT_NIE_NA_6DE_GELID,
    EN_PASSANT_NIE_BUITSKUIF,
    TE_VEEL_KOORDINATE,
    TE_VEEL_BUIT,
    TE_VEEL_KOPPELTEKEN,
    ROKEER_KONING_REEDS_GESKUIF,
    ROKEER_TORING_REEDS_GESKUIF,
    ROKEER_STUKKE_TUSSENIN,
    ROKEER_SKUIF_UIT_SKAAK,
    ROKEER_SKUIF_OOR_BEDREIGDE_BLOKKIE,
    SKUIF_IN_SKAAK,
    BUIT_EIE_STUK,
    ONWETTIGE_TORINGBEWEGING,
    ONWETTIGE_RUITERBEWEGING,
    ONWETTIGE_LOPERBEWEGING,
    ONWETTIGE_DAMEBEWEGING,
    ONWETTIGE_KONINGBEWEGING,
    ONWETTIGE_PIONBEWEGING
};

bool isRokeer(const std::string &skuif)
{
    return ((skuif == "0-0") || (skuif == "0-0-0"));
}

bool isOorgee(const std::string &skuif)
{
    return (skuif == "Gee oor.");
}

bool isStuk(char stuk)
{
    return ((stuk == 'K') || (stuk == 'D') || (stuk == 'T') || (stuk == 'R') || (stuk == 'L'));
}

bool isRy(char ry)
{
    return (ry >= 'a' && ry <= 'h');
}

bool isGelid(char gelid)
{
    return (gelid >= '1' && gelid <= '8');
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

bool isKorrekteGelid(bool isWit, bool isEnPassant, char gelid)
{
    if (isEnPassant)
        return gelid == (isWit ? '6' : '3');
    else
        return gelid == (isWit ? '8' : '1');
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

Soort bepaalSoort(char stuk)
{
    switch (stuk)
    {
        case 'T':   return Soort::TORING;
        case 'R':   return Soort::RUITER;
        case 'L':   return Soort::LOPER;
        case 'D':   return Soort::DAME;
        case 'K':   return Soort::KONING;
        default:    return Soort::PION;
    }
}

bool isWettigeToringBeweging(const Koordinaat *beginpos, const Koordinaat *eindpos)
{
    return  (eindpos->ry != beginpos->ry && eindpos->gelid == beginpos->gelid) ||
            (eindpos->gelid != beginpos->gelid && eindpos->ry == beginpos->ry);
}

bool isWettigeRuiterBeweging(const Koordinaat *beginpos, const Koordinaat *eindpos)
{
    return ((abs(eindpos->ry - beginpos->ry == 1) &&
             abs(eindpos->gelid - beginpos->gelid == 2)) ||
            (abs(eindpos->ry - beginpos->ry == 2) &&
             abs(eindpos->gelid - beginpos->gelid == 1)));
}

bool isWettigeLoperBeweging(const Koordinaat *beginpos, const Koordinaat *eindpos)
{
    return (abs(eindpos->ry - beginpos->ry) == abs(eindpos->gelid - beginpos->gelid));
}

bool isWettigeDameBeweging(const Koordinaat *beginpos, const Koordinaat *eindpos)
{
    return isWettigeToringBeweging(beginpos, eindpos) && isWettigeLoperBeweging(beginpos, eindpos);
}

bool isWettigeKoningBeweging(const Koordinaat *beginpos, const Koordinaat *eindpos)
{
    return (isWettigeDameBeweging(beginpos, eindpos) && abs(eindpos->ry - beginpos->ry) <= 1 &&
            abs(eindpos->gelid - beginpos->gelid) <= 1);
}

bool isWettigePionBeweging(bool isWit, bool isBuitskuif, const Koordinaat *beginpos, const Koordinaat *eindpos)
{
    if (isBuitskuif)
        return (isWit ? (eindpos->gelid == beginpos->gelid + 1) : eindpos->gelid == beginpos->gelid - 1 &&
                abs(eindpos->ry - beginpos->ry) == 1);

    else if (eindpos->gelid - beginpos->gelid == isWit ? 2 : -2)
        return (beginpos->gelid == isWit ? 2 : 7 && eindpos->ry - beginpos->ry == 0);

    else
        return (eindpos->gelid - beginpos->gelid == (isWit ? 1 : -1) && eindpos->ry - beginpos->ry == 0);
}

bool isWettigeBeweging(bool isWit, Soort soort, const Koordinaat *beginpos, const Koordinaat *eindpos)
{
    bool resultaat { false };
    switch (soort)
    {
        case Soort::TORING: resultaat =  isWettigeToringBeweging(beginpos, eindpos);            break;
        case Soort::RUITER: resultaat =  isWettigeRuiterBeweging(beginpos, eindpos);            break;
        case Soort::LOPER:  resultaat =  isWettigeLoperBeweging(beginpos, eindpos);             break;
        case Soort::DAME:   resultaat =  isWettigeDameBeweging(beginpos, eindpos);              break;
        case Soort::KONING: resultaat =  isWettigeKoningBeweging(beginpos, eindpos);            break;
        case Soort::PION:   resultaat =  isWettigePionBeweging(isWit, true, beginpos, eindpos); break;
        default:            resultaat =  false;
    }
    delete beginpos;
    delete eindpos;

    return resultaat;
}

Koordinaat* mapStringNaKoordinaat(const std::string &koordinaat)
{
    Koordinaat *resultaat { new Koordinaat };
    resultaat->ry = koordinaat[0] - ('a' - 1);   // Map 'a' na 1, 'b' na 2, ens.
    resultaat->gelid = koordinaat[1] - '0';      // Map '1' na 1, '2' na 2, ens.
    return resultaat;
}

void geeFoutboodskap(Fout fout, int pos=0)
{
    std::string foutboodskap { "" };
    switch (fout)
    {
        case Fout::TE_KORT: foutboodskap = "skuif is te kort."; break;
        case Fout::TE_LANK: foutboodskap = "skuif is te lank."; break;
        case Fout::BEGIN:
            foutboodskap =  "ongeldige beginkarakter. Die skuif moet met 'n stuk ('T', 'R', ens.),\n"
                            "ry ('a' - 'h'), rokade (0-0 of 0-0-0) of oorgee (Gee oor.) begin.";
            break;
        case Fout::STUK:
            foutboodskap =  "ongeldige karakter na stuk. 'n Stuk moet deur 'n ry ('a' - 'h')\n"
                            "buitsimbool ('X') of koppelteken ('-') gevolg word.";
            break;
        case Fout::RY:
            foutboodskap =  "ongeldige karakter na ry. 'n Ry kan net deur 'n gelid ('1' - '8')\ngevolg word.";
            break;
        case Fout::GELID:
            foutboodskap =  "ongeldige karakter na gelid. 'n Gelid moet deur 'n buitsimbool ('X'),\n"
                            "koppelteken ('-'), spasie (' '), promoveerskuif ('=') of skaak ('+') gevolg word.";
            break;
        case Fout::BUIT:
            foutboodskap =  "ongeldige karakter na buitsimbool. 'n Buitsimbool moet deur 'n ry\n"
                            "('a' - 'h') gevolg word.";
            break;
        case Fout::KOPPELTEKEN:
            foutboodskap =  "ongeldige karakter na koppelteken. 'n Koppelteken moet deur 'n ry\n"
                            "('a' - 'h') gevolg word.";
            break;
        case Fout::PROMOVEER_NIE_NA_STUK:
            foutboodskap =  "ongeldige karakter na promoveerskuif. 'n Promoveerskuif moet deur 'n\n"
                            "stuk ('T', 'R', ens.) gevolg word.";
            break;
        case Fout::PROMOVEER_NIE_OP_LAASTE_GELID:
            foutboodskap =  "kan nie op die middel van die bord promoveer nie. 'n Pion kan slegs\n"
                            "op die laaste gelid (gelid 8 vir Wit en 1 vir Swart) promoveer.";
            break;
        case Fout::SKAAK:
            foutboodskap =  "ongeldige karakter na skaak. 'n Skaak kan slegs deur nog 'n skaak ('+')\n"
                            "vir skaakmat gevolg word.";
            break;
        case Fout::SPASIE:
            foutboodskap =  "ongeldige string na spasie. Spasie kan slegs deur \"mat\", \"pat\"\n"
                            "of \"e.p.\" gevolg word.";
            break;
        case Fout::EN_PASSANT_NIE_NA_6DE_GELID:
            foutboodskap =  "";
    }
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
    const int skuifLengte { static_cast<int>(skuif.length()) };
    Fout fout;
    bool isGeldigeSkuif { !((skuifLengte < 2) || (skuifLengte > 14)) };
    if (!isGeldigeSkuif)
        fout = ((skuifLengte < 2) ? Fout::TE_KORT : Fout::TE_LANK);

    int i { -1 };
    int koordinaatTal { 0 };
    int buitTal { 0 };
    int koppeltekenTal { 0 };

    if (isGeldigeSkuif)
    {
        //  Enumerator Karakter hou tred met watter soort karakter die vorige karakter in die skuif was.
        Karakter vorige { Karakter::BEGIN };
        bool promoveerOpLaasteGelid { false };

        for (i = 0; i < skuifLengte; ++i)
        {
            if (!isGeldigeSkuif)
                break;

            std::string stert { "" };

            switch (vorige)
            {
            case Karakter::BEGIN:   //  Voer uit as huidige karakter (skuif[i]) die eerste karakter van die string is.
                isGeldigeSkuif = isRokeer(skuif) || isOorgee(skuif) || isStuk(skuif[i]) ||
                    isRy(skuif[i]);
                break;

            case Karakter::STUK:    // Voer uit as huidige karakter (skuif[i]) op 'n stuk volg.
                isGeldigeSkuif = (isRy(skuif[i]) && koordinaatTal < 2) || isBuit(skuif[i]) || isSpasie(skuif[i]);
                break;

            case Karakter::RY:
                isGeldigeSkuif = isGelid(skuif[i]);
                break;

            case Karakter::GELID:  // Voer uit as huidige karakter (skuif[i]) op 'n koördinaat volg.
                isGeldigeSkuif = ((isBuit(skuif[i]) || isKoppelteken(skuif[i])) && (buitTal + koppeltekenTal) == 0) ||
                    isSpasie(skuif[i]) || (isPromoveer(skuif[i]) && koordinaatTal <= 2) || isSkaak(skuif[i]);
                break;

            case Karakter::BUIT:    // Voer uit as huidige karakter (skuif[i]) op 'n 'X' volg.
                isGeldigeSkuif = isRy(skuif[i]) && (koordinaatTal < 2);
                break;

            case Karakter::KOPPELTEKEN: // Voer uit as huidige karakter (skuif[i]) op 'n koppelteken volg.
                isGeldigeSkuif = isRy(skuif[i]) && (koordinaatTal == 1);
                break;

            case Karakter::PROMOVEER:  // Voer uit as huidige karakter (skuif[i]) op 'n '=' volg.
                // 'n Pion kan net promoveer op die laaste gelid. As die koördinaat 'n ander gelid gee, lê die fout
                // reeds by daardie karakter in die skuifstring.
                promoveerOpLaasteGelid = isKorrekteGelid(isWit, false, skuif[i-2]);
                if (!promoveerOpLaasteGelid)
                    i -= 2;
                isGeldigeSkuif = promoveerOpLaasteGelid && isStuk(skuif[i]) && (i = skuifLengte - 1 ||
                                                                                isSpasie(skuif[i+1]) ||
                                                                                isSkaak(skuif[i+1]));
                break;

            case Karakter::SKAAK:   // Voer uit as huidige karakter (skuif[i]) op 'n '+' volg.
                isGeldigeSkuif = isSkaak(skuif[i]) && ((skuifLengte - i) == 1);
                break;

            case Karakter::SPASIE:  // Voer uit as huidige karakter (skuif[i]) op 'n spasie volg.
                stert = skuif.substr(i, skuifLengte - i);
                isGeldigeSkuif =  (isEnPassant(stert) && isKorrekteGelid(isWit, true, skuif[i-2]) &&
                                   buitTal == 1) || isMat(stert) || isPat(stert);
                break;

            default:
                isGeldigeSkuif = false;
            }
            if (isGeldigeSkuif)
            {
                // As enige van die volgende waar is, het ek reeds alles tot by die einde van die string getoets,
                // al is die loop counter (i) nog nie gelyk aan skuifLengte nie, so ek moet dus eksplisiet uit die
                // loop uitbreek.
                if (isRokeer(skuif) || isOorgee(skuif) || isEnPassant(stert) ||
                    isMat(stert) || isPat(stert))
                    break;

                //  Wat die huidige karakter is, bepaal wat die volgende karakter mag wees.
                if (isRy(skuif[i]))
                {
                    ++koordinaatTal;
                    vorige = Karakter::RY;
                    fout = Fout::RY;
                }
                else if (isGelid(skuif[i]))
                {
                    if ((buitTal + koppeltekenTal) == 1 && koordinaatTal == 2)
                    isGeldigeSkuif = isWettigeBeweging(isWit, bepaalSoort(skuif[0]),
                                                 mapStringNaKoordinaat(skuif.substr(i - 4, 2)),
                                                 mapStringNaKoordinaat(skuif.substr(i - 1, 2)));
                    vorige = Karakter::GELID;
                    fout = isGeldigeSkuif ? Fout::GELID : bepaalOnwettigeBeweging(bepaalSoort(skuif[0]));
                }
                else if (isStuk(skuif[i]))
                {
                    vorige = Karakter::STUK;
                    fout = Fout::STUK;
                }
                else if (isBuit(skuif[i]))
                {
                    ++buitTal;
                    vorige = Karakter::BUIT;
                    fout = Fout::BUIT;
                }
                else if (isKoppelteken(skuif[i]))
                {
                    ++koppeltekenTal;
                    vorige = Karakter::KOPPELTEKEN;
                    fout = Fout::KOPPELTEKEN;
                }
                else if (isSpasie(skuif[i]))
                {
                    vorige = Karakter::SPASIE;
                    fout = Fout::SPASIE;
                }
                else if (isPromoveer(skuif[i]))
                {
                    vorige = Karakter::PROMOVEER;
                    fout = promoveerOpLaasteGelid ? Fout::PROMOVEER_NIE_NA_STUK :
                        Fout::PROMOVEER_NIE_OP_LAASTE_GELID;
                }
                else if (isSkaak(skuif[i]))
                {
                    vorige = Karakter::SKAAK;
                    fout = Fout::SKAAK;
                }
            }
            else
            {
                if (koordinaatTal == 2 && isRy(skuif[i]))
                    fout = Fout::TE_VEEL_KOORDINATE;
                else if ((buitTal + koppeltekenTal) == 1 && (isBuit(skuif[i] || isKoppelteken(skuif[i]))))
                    fout = isBuit(skuif[i]) ? Fout::TE_VEEL_BUIT : Fout::TE_VEEL_KOPPELTEKEN;
                else if (isEnPassant(stert))
                    fout = buitTal != 1 ? Fout::EN_PASSANT_NIE_BUITSKUIF : Fout::EN_PASSANT_NIE_NA_6DE_GELID;
                break;
            }
        }
    }
    if (!isGeldigeSkuif)
        geeFoutboodskap(fout, i + 1);

    return isGeldigeSkuif;
}

// Hierdie funksie kan slegs vir nie-rokeerskuiwe geroep word. Dit bepaal die bestemming waarheen 'n stuk
// geskuif moet word uit die gegewe (geldige en wettige) skuif. Elke geldige skuif (behalwe rokades) moet
// een of twee koördinate hê. As daar net een koördinaat is, is dit die bestemming se koördinaat. As daar
// twee koördinate is, is die laaste een die bestemming se koördinaat. Daarom loop ek deur die string van
// die einde af en beskou die eerste koördinaat wat ek teëkom, as die bestemming.
Koordinaat* bepaalBestemming(const std::string &skuif)
{
    for (int i { static_cast<int>(skuif.length()) - 1 }; i >= 0; --i)
    {
        if (isGelid(skuif[i]))
            return mapStringNaKoordinaat(skuif.substr(i - 1, 2));
    }

    return nullptr;
}

Stuk* soekStuk(const Koordinaat *koordinaat, Posisie **posisies)
{
    int boonsteGrens { aantalStukke - 1};
    int ondersteGrens { 0 };

    do
    {
        int indeks { ondersteGrens + ((boonsteGrens - ondersteGrens) / 2) };
        if (posisies[indeks]->koordinaat.gelid > koordinaat->gelid)
        {
            ondersteGrens = indeks + 1;
        }
        else if (posisies[indeks]->koordinaat.gelid < koordinaat->gelid)
        {
            boonsteGrens = indeks - 1;
        }
        else if (posisies[indeks]->koordinaat.gelid == koordinaat->gelid)
        {
            if (posisies[indeks]->koordinaat.ry > koordinaat->ry)
                ondersteGrens = indeks + 1;
            else if (posisies[indeks]->koordinaat.ry < koordinaat->ry)
                boonsteGrens = indeks - 1;
            else if (posisies[indeks]->koordinaat.ry == koordinaat->ry)
                return posisies[indeks]->stuk;
        }
    } while (!(boonsteGrens < ondersteGrens));

    return nullptr;
}

Koordinaat* bepaalOorsprong(bool isWit, const std::string &skuif, Posisie **posisies, Koordinaat *bestemming)
{
    const int skuifLengte { static_cast<int>(skuif.length()) };

    if (skuifLengte > 5)
    {
        bool koordinaatGevind { false };
        std::string koordinaat { "" };

        for (int i { 0 }; i < (skuifLengte - 1); ++i)
        {
            if (isRy(skuif[i]))
            {
                if (koordinaatGevind)
                {
                    return mapStringNaKoordinaat(koordinaat);
                }
                else
                {
                    koordinaatGevind = true;
                    koordinaat = skuif.substr(i, 2);
                }
            }
        }
    }

}

bool isBlokkieBedreig(bool isWit, const Koordinaat &koordinaat)
{
    for (auto el : )
}

bool isWettig(bool isWit, const std::string &skuif, Stuk* stukke, Posisie **posisies)
{
    // Hierdie ses static bools dui aan of elk van die 4 torings en die 2 konings al geskuif het.
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
        bool isKortRokade { skuif == "0-0" };

        // Inisialiseer met die resultaat van die toets of al die blokkies tussen die Koning en Toring oop is.
        bool isRokadeWettig { (stukke[isWit ? 4 : 28].posisie + (isKortRokade ? 1 : -1))->
            stuk->soort == Soort::TORING };

        if (isRokadeWettig)     // As die eerste toets slaag...
        {
            // doen die tweede toets ;-) (of die betrokke koning of toring al geskuif het.)
            isRokadeWettig = isWit ? witKoning : swartKoning;

            if (isRokadeWettig && isKortRokade)
                isRokadeWettig = isWit ? witKortToring : swartKortToring;
            else
                isRokadeWettig = isWit ? witLangToring : swartLangToring;

            if (isRokadeWettig) // As die tweede toets slaag...
            {
                // Toets of die Koning in skaak is
                isRokadeWettig = isBlokkieBedreig(isWit, stukke[isWit ? 4 : 28].posisie->koordinaat);
            }
            else    // As die tweede toets dop:
            {
                // Verduidelik die gebruiker hoekom.
                if  (isWit ? witKoning : swartKoning)
                    geeFoutboodskap(Fout::ROKEER_KONING_REEDS_GESKUIF);
                else
                    geeFoutboodskap(Fout::ROKEER_TORING_REEDS_GESKUIF);
            }
        }
        else    // As die eerste toets dop:
        {
            geeFoutboodskap(Fout::ROKEER_STUKKE_TUSSENIN);
        }

        return isRokadeWettig;
    }
    else
    {
        Koordinaat *bestemming { bepaalBestemming(skuif) };
        Stuk *stukByBestemming { soekStuk(bestemming, posisies) };
        if (stukByBestemming && stukByBestemming->isWit == isWit)
        {
            geeFoutboodskap(Fout::BUIT_EIE_STUK);
            return false;
        }
        // return bepaalOorsprong(isWit, skuif, posisies, bestemming).ry == 0 ? false : true;
        return true;
    }
}

const std::string valideerEnDoenSkuif(bool isWit, Stuk *stukke, Posisie **posisies)
{
    // Bepaal watter kleur volgende moet skuif om die spelers te herinner.
    const std::string kleur { (isWit) ? "WIT" : "SWART" };
    std::string skuif { "" };
    Koordinaat *bestemming { nullptr };
    Koordinaat *oorsprong { nullptr };
    bool isGeldigeSkuif { false };

    do
    {
        std::cout << kleur << " skuif: ";
        std::getline(std::cin, skuif);
        std::cout << "\n";

        isGeldigeSkuif = isGeldig(isWit, skuif);

        if (isGeldigeSkuif)
        {
            bestemming = bepaalBestemming(skuif);
            oorsprong = bepaalOorsprong(isWit, skuif, posisies, bestemming);
        }

    } while (!(isGeldigeSkuif && isWettig(isWit, skuif, stukke, posisies)));

    doenSkuif(isWit, skuif, stukke, posisies);

    delete bestemming;
    delete oorsprong;

    return skuif;
}

void doenSkuif(bool isWit, const std::string &skuif, Stuk *stukke, Posisie **posisies)
{
    if (isOorgee(skuif))
    {
        return;
    }
    else if (isRokeer(skuif))
    {
        if (skuif == "0-0") // Kortrokade
        {
            stukke[isWit ? 4 : 28].posisie->koordinaat.ry = 7;
            stukke[isWit ? (aantalRye - 1) : (aantalStukke - 1)].posisie->koordinaat.ry = 6;
        }
        else                // Langrokade
        {
            stukke[isWit ? 4 : 28].posisie->koordinaat.ry = 3;
            stukke[isWit ? 0 : 24].posisie->koordinaat.ry = 4;
        }
    }
    #if 0
    else
    {

        const Stuk *eindPosisieStuk { soekStuk(bestemming, posisies) };
        if (eindPosisieStuk)
            eindPosisieStuk->posisie->koordinaat.gelid = isWit ?  0 : 9;

        const Stuk *beginPosisieStuk { soekStuk (oorsprong) };
        beginPosisieStuk->posisie->koordinaat.ry = bestemming->ry;
        beginPosisieStuk->posisie->koordinaat.gelid = bestemming->gelid;
    }

    sorteerPosisies(posisies);
    #endif // 0
}

void skryfSkuif(bool isSwart, const std::string &skuif)
{
    static int skuiwe { 1 };

    std::cout << "\t" << skuiwe << ".\t" << (isSwart ? "...\t" : "") << skuif << "\n\n";

    if (isSwart)
        ++skuiwe;
}
