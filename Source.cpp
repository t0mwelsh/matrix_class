// Assignment 6 PHYS30762
// Creates a system of classes to output volumes and areas of shapes. Demonstrates the use of abstract base classes

#include<iostream>
#include<string>
#include<cmath>
#include<vector>
#include<iomanip>

class shape
{
public:
	virtual ~shape() { std::cout << "Shape destructor called" << std::endl; } // Need this!
	virtual double area() const = 0; // pure virtual function 
	virtual double volume() const = 0; // pure virtual function
};

class two_shape : public shape
{
public:
	~two_shape() { std::cout << "2D_shape destructor called" << std::endl; } // I think virtual-ness is inherited
};

class three_shape : public shape
{
public:
	~three_shape() { std::cout << "3D_shape destructor called" << std::endl; } // I think virtual-ness is inherited
};

class square : public two_shape
{
private:
	double length;
public:
	square() : length{} {} //default
	square(double l) : length{ l } {} //parameterised
	~square() { std::cout << "Square destructor called" << std::endl; }
	double area() const { return length*length; }
	double volume() const { return 0; }
};
class rectangle : public two_shape
{
private:
	double length;
	double width;
public:
	rectangle() : length{}, width{} {} //default
	rectangle(double l, double w) : length{ l }, width{ w } {} //parameterised
	~rectangle() { std::cout << "Rectangle destructor called" << std::endl; }
	double area() const { return length * width; }
	double volume() const { return 0; }
};
class circle : public two_shape
{
private:
	double radius;
public:
	circle() : radius{} {} //default
	circle(double r) : radius{ r } {} //parameterised
	~circle() { std::cout << "Circle destructor called" << std::endl; }
	double area() const { return atan(1) * 4 * radius * radius; }
	double volume() const { return 0; }
};
class ellipse : public two_shape
{
private:
	double a;
	double b;
public:
	ellipse() : a{}, b{} {} //default
	ellipse(double a_, double b_) : a{ a_ }, b{ b_ } {} //parameterised
	~ellipse() { std::cout << "Ellipse destructor called" << std::endl; }
	double area() const { return atan(1) * 4 * a * b; }
	double volume() const { return 0; }
};

class cube : public three_shape
{
private:
	double length;
public:
	cube() : length{} {} //default
	cube(double l) : length{ l } {} //parameterised
	~cube() { std::cout << "Cube destructor called" << std::endl; }
	double area() const { return 6*length*length; }
	double volume() const { return length * length * length; }
};
class cuboid : public three_shape
{
private:
	double length;
	double width;
	double height;
public:
	cuboid() : length{}, height{}, width{} {} //default
	cuboid(double l, double h, double w) : length{ l }, height{h}, width{w} {} //parameterised
	~cuboid() { std::cout << "Cuboid destructor called" << std::endl; }
	double area() const { return 2*(length*height + length*width + width*height); }
	double volume() const { return length * width * height; }
};
class sphere : public three_shape
{
private:
	double radius;
public:
	sphere() : radius{} {} //default
	sphere(double r) : radius{ r } {} //parameterised
	~sphere() { std::cout << "Sphere destructor called" << std::endl; }
	double area() const { return 4 * atan(1) * 4 * pow(radius, 2); }
	double volume() const { return 4./3. * atan(1) * 4 * pow(radius,3); }
};
class ellipsoid : public three_shape
{
private:
	double a;
	double b;
	double c;
public:
	ellipsoid() : a{}, b{}, c{} {} //default
	ellipsoid(double a_, double b_, double c_) : a{ a_ }, b{ b_ }, c{ c_ } {} //parameterised
	~ellipsoid() { std::cout << "Ellipsoid destructor called" << std::endl; }
	double area() const { return 16 * atan(1) * pow((pow(a*b,1.6) + pow(a*c,1.6) + pow(b*c,1.6))/3., 1./1.6); } //approximate
	double volume() const { return 4. / 3. * atan(1) * 4 * a*b*c; }
};
class prism : public three_shape
{
private:
	double length;
	two_shape* cross_section;
public:
	prism() : length {}, cross_section{} {} //default
	prism(double l, two_shape* cross_shape) : length{ l }, cross_section{ cross_shape } {} //parameterised
	~prism() { std::cout << "Prism destructor called" << std::endl; }
	double area() const { return 0; } //surface area not required here so set to zero
	double volume() const { return cross_section->area() * length; }
};

int main()
{
	two_shape* cross_shape{ new circle(2) };
	
	std::vector<shape*> shapes;
	shapes.push_back(new square(3));
	shapes.push_back(new rectangle(2,3));
	shapes.push_back(new circle(3));
	shapes.push_back(new ellipse(2,3));
	shapes.push_back(new cube(2));
	shapes.push_back(new cuboid(2,3,4));
	shapes.push_back(new sphere(2));
	shapes.push_back(new ellipsoid(2,3,4));
	shapes.push_back(new prism(2, cross_shape));

	std::vector<shape*>::iterator iter;
	std::cout << std::setprecision(4);
	for (iter = shapes.begin(); iter != shapes.end(); iter++) {
		if ((*iter)->area() != 0) { std::cout << "area=" << (*iter)->area() << std::endl; }
		if ((*iter)->volume() != 0) { std::cout << "volume=" << (*iter)->volume() << std::endl; }
		delete (*iter);
	}//vector deletes itself when out of scope
	delete cross_shape;
	return 0;
}