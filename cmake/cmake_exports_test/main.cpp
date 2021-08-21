// A minimal program to check that we can link a simple program
// against the installed maeparser

#include <iostream>
#include <memory>
#include <sstream>

#include "maeparser/Reader.hpp"


int main(int argc, char const *argv[])
{
    auto empty_stream = std::make_shared<std::istringstream>();

    schrodinger::mae::Reader r(empty_stream);

    std::cout << "Check ok" << std::endl;

    return 0;
}