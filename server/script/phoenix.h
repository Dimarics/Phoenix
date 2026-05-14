#include <iostream>

void arm() {
    std::cout << "arm";
    std::cin.get();
}

void disarm() {
    std::cout << "disarm";
    std::cin.get();
}

void takeoff(float height) {
    std::cout << "takeoff " << height;
    std::cin.get();
}

void land() {
    std::cout << "land";
    std::cin.get();
}

void setPoint(float x, float y, float z) {
    std::cout << "setPoint " << x << " " << y << " " << z;
    std::cin.get();
}

void setYaw(float value) {
    std::cout << "setYaw " << value;
    std::cin.get();
}

void align() {
    std::cout << "align";
    std::cin.get();
}

void delay(float value) {
    std::cout << "delay " << value;
    std::cin.get();
}
