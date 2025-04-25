// person.cpp
#include "her/test.hpp"
#include <iostream>

Person::Person(std::string n, int a) : name(n), age(a) {}  // Constructor implementation

void Person::display() {
    std::cout << "Name: " << name << ", Age: " << age << std::endl;
}