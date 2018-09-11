#include <iostream>
#include <limits>

int getValidInt()
{
    int i;
    bool invalid { false };

    do
    {
        invalid = false;
        std::cout << "Enter a valid integer: ";
        std::cin >> i;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (std::cin.fail())
        {
            invalid = true;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Oops! You did not enter a valid integer.\n";
        }

        std::cout << "\n";

    } while (invalid);

    return i;

}

double getValidDouble()
{
    double d;
    bool invalid { false };

    do
    {
        invalid = false;
        std::cout << "Enter a valid double: ";
        std::cin >> d;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (std::cin.fail())
        {
            invalid = true;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Oops! You did not enter a valid double.\n";
        }

        std::cout << "\n";

    } while (invalid);

    return d;

}

char getValidChar()
{
    char c;

    std::cout << "Enter a valid character: ";
    std::cin >> c;
    std::cout << "\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return c;

}
