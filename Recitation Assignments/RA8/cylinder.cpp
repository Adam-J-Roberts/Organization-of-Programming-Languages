#include "cylinder.h"
#include <iostream>

using namespace std;

Cylinder::Cylinder(double radius, double height): Circle(radius){
    this->height = height;
    //setRadius(radius);
}    
void Cylinder::setHeight(double height){
    this->height = height;  
}
double Cylinder::getHeight() const{
    return height;
}

double Cylinder::getArea() const{
    return ((2*PI*getRadius()*height) + (2*PI* (getRadius()*getRadius())) );
}
double Cylinder::getVolume() const{
    return (PI * (getRadius()*getRadius()) * height );
}
string Cylinder::getName() const{
    return "Cylinder";
}
void Cylinder::print() const{
    cout << "Radius is " << getRadius() << " and height is " << height;
    cout << " Area is " << getArea() << " and volume is " << getVolume() << endl;
}
