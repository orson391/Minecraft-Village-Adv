// person.hpp
#ifndef TEST_HPP
#define TEST_HPP
#include <string>

class Person {
public:
    std::string name;  // Data member
    int age;

    Person(std::string n, int a);  // Constructor
    void display();  // Member function
};

#endif