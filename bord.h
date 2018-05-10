#ifndef BORD_H
#define BORD_H

extern const char leeg;

const int sylengte { 10 };    // 'n Skaakbord is 8x8 met twee ekstra rye vir
                            // koordinaatletters en -syfers.

void tekenBord(const bool &isWit, const char stukke[sylengte][sylengte]);
void tekenBlokkie(const int &gelid, const int &ry, const char stukke[sylengte][sylengte]);

#endif // BORD_H
