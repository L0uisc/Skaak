#ifndef KONSTANTES_H
#define KONSTANTES_H

using uint_t = unsigned int;

// 'n Skaakbord is 8x8 met twee ekstra rye vir koordinaatletters en -syfers.
constexpr uint_t g_sylengte { 10 };

// Die aantal rye op 'n skaakbord.
constexpr uint_t g_aantalRye { 8 };

// Die totale aantal stukke op die bord aan die begin van elke spel.
constexpr uint_t g_aantalStukke { g_aantalRye * 4 };

#endif // KONSTANTES_H
