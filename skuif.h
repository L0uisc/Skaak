#ifndef SKUIF_H
#define SKUIF_H
#include "konstantes.h"
#include "stukke.h"

void doenSkuif(const std::string &skuif, bool isWit, Stuk *stukke);
void skryfSkuif(bool isSwart, const std::string &skuif);

std::string krySkuif(bool isWit, const Stuk *stukke);

bool isWettig(const std::string &skuif, const Stuk *stukke);
bool isGeldig(const std::string &skuif, bool isWit);

bool isRokeer(const std::string &skuif);
bool isOorgee(const std::string &skuif);
bool isStuk(char skuif);
bool isKoordinaat(const std::string &koordinaat);
bool isBuit(char buit);
bool isKoppelteken(char koppelteken);
bool isSpasie(char spasie);
bool isPromoveer(char isGelykAan);
bool isLaasteGelid(char gelid, bool isWit);
bool isSkaak(char skaak);
bool isEnPassant(const std::string &enPassant);
bool isMat(const std::string &mat);
bool isPat(const std::string &pat);

#endif // SKUIF_H
