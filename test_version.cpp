#include "lib.h"

#include <iostream>

int main (int, char **) 
{
    if (version() == 100)
        std::cout << "FAIL" << std::endl;
    
    return 0;
}