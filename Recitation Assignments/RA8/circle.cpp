#include "circle.h"
#include <iostream>

using namespace std;

Circle::Circle(double radius){
    this->radius = radius;
}
void Circle::setRadius( double radius){
    this->radius = radius;
}  
double Circle::getRadius() const{
    return this->radius;
} 
double Circle::getDiameter() const{
    return (this->radius*2);
} 
double Circle::getCircumference() const{
    return (2*PI*radius);
} 
double Circle::getArea() const{   
    return (PI*(radius*radius));
}   
string Circle::getName() const{
    return "Circle";
}
void Circle::print() const{
    cout << "Radius is " << radius << " Area is " << getArea(); 
    cout << " and circumference is " << getCircumference() << endl;
} 