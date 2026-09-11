#include "Header.h"
#include <iostream>
#include <locale>
#include <cstdlib>  
#include <ctime>    

int main()
{
    std::setlocale(LC_ALL, "Russian");

    Book b1("À", { "à", "á", "â", "ã", "ä" }, 25, 100);
    Book b2("S", { "à", "ì", "í", "î", "ï" }, 50, 100);

    Book result = b1 += b2;
    std::cout << "b1 += b2 = " << std::endl;
    result.print();

    return 0;
}