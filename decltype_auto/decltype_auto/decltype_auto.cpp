// decltype_auto.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/* In this example we are trying to add two numbers or concatenating two strings and return type of function is based on what it adds or 
concatenates */

#include <iostream>
#include <string>
#include <type_traits>

template<typename T, typename U>
decltype (auto) add(const T& t, const U& u)
{
    return (t + u);
}

int main()
{
    std::cout << add(2, 5) << std::endl; // Adds two integers and return an integer
    std::cout << add(2.2, 5.9) << std::endl; // Adds to floating point numbers and returns a floating point result
    std::cout << add(2, 5.2) << std::endl; // Type promotion happened and return type is a float after adding a integer with float

    std::cout << add(std::string("Hey"), std::string(" You")) << std::endl; // Return type is a string concatenated
    std::cout << add(std::to_string(2), std::string(" You")) << std::endl; // Return type is a string 

    return 0;
}


