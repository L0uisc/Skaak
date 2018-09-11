#ifndef KONSTANTES_H
#define KONSTANTES_H

enum class Soort
{
    TORING,
    RUITER,
    LOPER,
    DAME,
    KONING,
    PION
};

struct Koordinaat
{
    int ry;
    int gelid;
};

constexpr int aantalRye { 8 };                  // Die aantal rye op 'n skaakbord.
constexpr int aantalStukke { aantalRye * 4 };   // Die totale aantal stukke op die bord aan die begin van elke spel.

#endif // KONSTANTES_H
