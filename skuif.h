#ifndef SKUIF_H
#define SKUIF_H
#include "bord.h"

void doenSkuif(const bool &isWit, char stukke[sylengte][sylengte]);
void skryfSkuif(const bool &isWit, const std::string &skuif);

std::string krySkuif(const bool &isWit, char stukke[sylengte][sylengte]);

bool isSpelVerby(bool &isWit, const char stukke[sylengte][sylengte]);

bool isWettig(const std::string &skuif, const char stukke[sylengte][sylengte]);
bool isGeldig(const std::string &skuif, const bool &isWit);

bool isRokeer(const std::string &skuif);
bool isOorgee(const std::string &skuif);
bool isStuk(const char &skuif);
bool isKoordinaat(const std::string &koordinaat);
bool isBuit(const char &buit);
bool isKoppelteken(const char &koppelteken);
bool isSpasie(const char &spasie);
bool isIsGelykAan(const char &isGelykAan);
bool isLaasteGelid(const char &gelid, const bool &isWit);
bool isSkaak(const char &skaak);
bool isEnPassant(const std::string &enPassant);
bool isMat(const std::string &mat);
bool isPat(const std::string &pat);

#endif // SKUIF_H
