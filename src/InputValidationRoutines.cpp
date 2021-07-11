#include "InputValidationRoutines.h"
#include <iostream>
#include <limits>

constexpr std::streamsize g_maxStreamSize { std::numeric_limits<std::streamsize>::max() };

int getValidInt()
{
    int i {};
    bool invalid {};

    do
    {
        invalid = false;
        std::cout << "Enter a valid integer: ";
        std::cin >> i;

        std::cin.ignore(g_maxStreamSize, '\n');

        if (std::cin.fail())
        {
            invalid = true;
            std::cin.clear();
            std::cin.ignore(g_maxStreamSize, '\n');
            std::cerr << "Oops! You did not enter a valid integer.\n";
        }

        std::cout << "\n";

    } while (invalid);

    return i;
}

double getValidDouble()
{
    double d {};
    bool invalid {};

    do
    {
        invalid = false;
        std::cout << "Enter a valid double: ";
        std::cin >> d;

        std::cin.ignore(g_maxStreamSize, '\n');

        if (std::cin.fail())
        {
            invalid = true;
            std::cin.clear();
            std::cin.ignore(g_maxStreamSize, '\n');
            std::cerr << "Oops! You did not enter a valid double.\n";
        }

        std::cout << "\n";

    } while (invalid);

    return d;
}

char getValidChar()
{
    char c {};

    std::cout << "Enter a valid character: ";
    std::cin >> c;
    std::cin.ignore(g_maxStreamSize, '\n');
    std::cout << "\n";

    return c;
}
