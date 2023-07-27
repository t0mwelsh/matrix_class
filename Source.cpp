// PHYS 30762 Programming in C++
// Assignment 5
// A matrix class
// Provides overloading of <<, >>, +, - and * on matricies as well as calculating determinant
#include<iostream>
#include<ostream>
#include<stdlib.h> // for c style exit
#include<vector>
#include<cmath>

class matrix
{
	// Friends
	friend std::ostream& operator<<(std::ostream& os, matrix& mat);
	friend std::istream& operator>>(std::istream& input, matrix& mat);
private:
	double* matrix_data{ nullptr };
	int rows{ 0 };
	int columns{ 0 };
public:
	// Default constructor
	matrix() = default;
	// Parameterized constructor
	matrix(int m, int n);
	// Copy constructor
	matrix(const matrix&);
	// Move constructor
	matrix(matrix&&) noexcept;
	// Destructor
	~matrix() {
		std::cout << "Destructor called" << std::endl;
		if (matrix_data != nullptr) { delete[] matrix_data; }
	}
	// Access functions
	int get_rows() const { return rows; } // Return number of rows
	int get_columns() const { return columns; } // Return number of columns
	double* get_data() const { return matrix_data; }
	//Other functions
	int index(int m, int n) const // Return position in array of element (m,n)
	{
		if (m > 0 && m <= rows && n > 0 && n <= columns) return (n - 1) + (m - 1) * columns;
		else { std::cout << "Error: out of range" << std::endl; exit(1); }
	}
	double& operator()(int m, int n) { return matrix_data[index(m, n)]; }
	// Addition, subtraction and multiplication
	matrix operator+(const matrix& mat) const;
	matrix operator-(const matrix& mat) const;
	matrix operator*(const matrix& mat) const;
	// Copy Assignment operator
	matrix& operator=(const matrix&);
	// Move Assignment operator
	matrix& operator=(matrix&&) noexcept;
	// minor
	matrix remove_ij(int i, int j);
	// determinant
	double determinant();
};
// Member functions defined outside class
// Parameterized constructor implementation
matrix::matrix(int m, int n)
{
	std::cout << "Parameterized constructor called" << std::endl;
	if (m < 1 || n < 1) // matricies need at least one row and one column
	{
		std::cout << "Error: trying to declare an array with size < 1" << std::endl;
		throw("size not positive");
	}
	rows = m; columns = n; int size = m * n;
	matrix_data = new double[size];
	for (int i{}; i < size; i++) matrix_data[i] = 0; // fill with zeroes and fill in with other functions
}
// Copy constructor for deep copying
matrix::matrix(const matrix& mat)
{
	// Copy size and declare new array
	std::cout << "copy constructor\n";
	matrix_data = nullptr; rows = mat.get_rows(); columns = mat.get_columns();
	int size = rows * columns;
	if (size > 0) {
		matrix_data = new double[size];
		// Copy values into new array
		for (int i{}; i < size; i++) matrix_data[i] = mat.matrix_data[i];
	}
}
// Move constructor
matrix::matrix(matrix&& mat) noexcept
{ // steal the data
	std::cout << "move constructor\n";
	rows = mat.rows;
	columns = mat.columns;
	matrix_data = mat.matrix_data;
	// leave original matrix empty
	mat.rows = 0;
	mat.columns = 0;
	mat.matrix_data = nullptr;
}
// Assignment operator for deep copying
matrix& matrix::operator=(const matrix& mat)
{
	std::cout << "copy assignment\n";
	if (&mat == this) return *this; // no self assignment
	// First delete this object's array
	delete[] matrix_data; matrix_data = nullptr; rows = 0; columns = 0;
	// Now copy size and declare new array
	rows = mat.get_rows(); columns = mat.get_columns();
	int size = rows * columns;
	if (size > 0) {
		matrix_data = new double[size];
		// Copy values into new array
		for (int i{}; i < size; i++) matrix_data[i] = mat.matrix_data[i];
	}
	return *this;
}
// Move assignment operator
matrix& matrix::operator=(matrix&& mat) noexcept
{
	std::cout << "move assignment\n";
	std::swap(rows, mat.rows);
	std::swap(columns, mat.columns);
	std::swap(matrix_data, mat.matrix_data);
	return *this;
}
// Overload input and output stream for matrices
std::ostream& operator<<(std::ostream& os, matrix& mat)
{
	// Code goes here
	int m = mat.get_rows(); int n = mat.get_columns();
	for (int i = 1; i <= m; i++)
		for (int j = 1; j <= n; j++)
			os << mat(i, j) << " \n"[j == n]; // includes condition of new line if at the end of a row
	return os;
}
std::istream& operator>>(std::istream& is, matrix& mat)
{
	int row_input, column_input;
	while (true) {
		std::cout << "Enter two positive integers for the number of rows and columns respectively: \n";
		if (is >> row_input && row_input > 0 && is >> column_input && column_input > 0) { //ensures we have positive integers
			break;
		}
		else {
			std::cout << "Invalid input. Please try again." << std::endl;
			is.clear();
			is.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clears input before we try again
		}
	}
	matrix temp_matrix{ row_input, column_input };
	mat = std::move(temp_matrix); //this method means we get matrix_data constructed for us

	std::cout << "Please type a list of numbers (include spaces or press enter in between) for each element of the matrix. "
		"Type numbers \nin a row by row order from left to right." << std::endl;
	for (int i{}; i < mat.rows * mat.columns; i++) { 
		while (true) {
			try {
				if (is >> mat.matrix_data[i]) { //if statement triggered if 'is' is of correct form
					break;
				}
				else {
					throw std::runtime_error("Invalid input. Please try again.");
				}
			}
			catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
				is.clear();
				is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
	}
	return is;
}
// Addition, subtraction and multiplication
matrix matrix::operator+(const matrix& mat2) const
{
	matrix mat = *this;
	if (mat.rows != mat2.rows || mat.columns != mat2.columns)
	{
		std::cout << "Error: trying to add different sized matricies." << std::endl;
		throw("Out of Bounds error");
	}
	matrix temp{ rows, columns };
	int size = rows * columns;
	if (size > 0) {
		temp.matrix_data = new double[size];
		// Assign values into new array
		for (int i{}; i < size; i++) temp.matrix_data[i] = mat.matrix_data[i] + mat2.matrix_data[i];
	}
	return temp;
}
matrix matrix::operator-(const matrix& mat2) const
{
	matrix mat = *this;
	if (mat.rows != mat2.rows || mat.columns != mat2.columns)
	{
		std::cout << "Error: trying to add different sized matricies." << std::endl;
		throw("Out of Bounds error");
	}
	matrix temp{ rows, columns };
	int size = rows * columns;
	if (size > 0) {
		temp.matrix_data = new double[size];
		// Assign values into new array
		for (int i{}; i < size; i++) temp.matrix_data[i] = mat.matrix_data[i] - mat2.matrix_data[i];
	}
	return temp;
}
matrix matrix::operator*(const matrix& mat2) const
{
	matrix mat = *this;
	if (mat.columns != mat2.rows)
	{
		std::cout << "Error: trying to multiply wrong sized matricies." << std::endl;
		throw("Out of Bounds error");
	}
	matrix result{ mat.rows, mat2.columns };
	int size = result.get_rows() * result.get_columns();
	if (size > 0) { //otherwise just outputs the empty matrix
		for (int i{}; i < size; i++) //goes through and calculates each entry
		{
			double row_number_d = floor((1. * i) / (1. * result.get_columns())) + 1;
			int row_number = (int)std::round(row_number_d); //to avoid loss of data
			int col_number = (i + 1) - (row_number - 1) * result.get_columns();
			double mult{ 0. };
			for (int j = 0; j < mat.columns; j++) // number of things we're adding together
			{
				mult += mat.matrix_data[(row_number - 1) * mat.columns + j] * mat2.matrix_data[col_number + mat2.columns * j - 1];
			}
			result.matrix_data[i] = mult;
		}
	}
	return result;
}
// Remove ith row and jth column function to produce new sub-matrix
matrix matrix::remove_ij(int i, int j)
{
	if (i > rows || j > columns || i < 1 || j < 1)
	{
		std::cout << "Error: trying to remove rows/columns out of bounds of matrix." << std::endl;
		throw("Out of Bounds error");
	}
	int size = rows * columns;
	int new_size = (rows - 1) * (columns - 1);
	double* new_matrix_data = new double[new_size];
	int count{ -1 };
	for (int k{}; k < size; k++)
	{
		if ((k + 1) % columns != j % columns && ((k + 1) > columns * i || (k + 1) < columns * (i - 1) + 1)) { //checks element is not in a removed col/row
			count += 1;
			if (count < new_size) { new_matrix_data[count] = matrix_data[k]; }
			else { std::cout << "Trying to assign value with index out of range" << std::endl; } //just a check statement, shouldn't be invoked
		}
	}
	matrix sub_matrix{ rows - 1, columns - 1 };
	sub_matrix.matrix_data = new_matrix_data;
	return sub_matrix;
}
// Find determinant of matrix
double matrix::determinant()
{
	std::cout << "Calculating determinant" << std::endl;
	if (rows != columns)
	{
		std::cout << "Error: can only take determinant of square matrix" << std::endl;
		throw("Non-square matrix error");
	}
	double det = 0;
	int n = rows;
	if (n == 1) {
		return matrix_data[0]; //quick check for easiest case
	}
	if (n == 2) {
		return (matrix_data[0] * matrix_data[3]) - (matrix_data[1] * matrix_data[2]); //will always be invoked at end of recursive loop
	}
	int sign = 1;
	for (int i = 1; i <= n; i++) {//only need to work along the first row of a matrix
		matrix minor = this->remove_ij(1, i);
		det += sign * matrix_data[i - 1] * minor.determinant(); //recursion used
		sign = -sign; //sign alternates when calculating determinant
	}
	return det;
}
// Main program
int main()
{
	//
	// First part: constructing and deep copying matrices
	//
	// Demonstrate default constructor
	matrix a1;
	std::cout << "matrix a1: \n" << a1;
	// Parameterized constructor
	const int m{ 2 };
	const int n{ 3 };
	matrix a2{ m,n };

	//Demonstrate cin cout overloading
	matrix a7;
	std::cin >> a7;
	std::cout << "You inputted: \n" << a7 << std::endl;

	// Set values for a2 here
	a2(1, 1) = 1.; a2(1, 2) = 2.; a2(2, 1) = 3.; a2(2, 2) = 4.; a2(1, 3) = -1.;
	//Print matrix a2
	std::cout << "matrix a2: \n" << a2;

	// Deep copy by assignment: define new matrix a3 then copy from a2 to a3
	matrix a3{ m,n };
	std::cout << "matrix a3: \n" << a3;
	a3 = a2;
	std::cout << "matrix a3: \n" << a3;
	// Modify contents of original matrix and show assigned matrix is unchanged here
	a2(2, 3) = 0.5;
	std::cout << "matrix a2: \n" << a2;
	std::cout << "matrix a3: \n" << a3;

	// Deep copy using copy constructor
	matrix a4{ a2 };
	std::cout << "matrix a4: \n" << a4;
	// Modify contents of original matrix and show copied matrix is unchanged here
	a2(2, 3) = 11.;
	std::cout << "matrix a2: \n" << a2;
	std::cout << "matrix a4: \n" << a4;

	// Move construction demonstration
	matrix a6{ std::move(a2) };
	std::cout << "matrix a6: \n" << a6;
	std::cout << "matrix a2: \n" << a2; //some compilers give warning message here as a3 has been 'moved from' but that's OK as it's expected

	// Move assignment demonstration
	a1 = std::move(a3);
	std::cout << "matrix a1: \n" << a1;
	std::cout << "matrix a3: \n" << a3; //some compilers give warning message here as a3 has been 'moved from' but that's OK as it's expected
	//
	// Second part: matrix operations
	//
	// Addition of 2 matrices
	matrix sum{ a1 + a6 };
	std::cout << "a1 + a6 = \n" << sum << std::endl;
	// Subtraction of 2 matrices
	matrix diff{ a1 - a6 };
	std::cout << "a1 - a6 = \n" << diff << std::endl;
	// Multiplication of 2 matrices
	matrix a5{ 3,2 };
	a5(1, 1) = 2.; a5(1, 2) = 2.; a5(2, 1) = -1.; a5(2, 2) = 1.; a5(3, 2) = -1.;
	std::cout << "matrix a5: \n" << a5;
	matrix prod{ a5 * a6 };
	std::cout << "prod = a5 x a6 = \n" << prod << std::endl;

	// Minor
	std::cout << "Removing 1st row and 2nd column from a6 to create a6_sub" << std::endl;
	matrix a6_sub = a6.remove_ij(1, 2);
	std::cout << "matrix a6: \n" << a6;
	std::cout << "matrix a6_sub: \n" << a6_sub;
	//Determinant
	double prod_det = prod.determinant();
	std::cout << "Determinant of prod: " << prod_det << std::endl;
	return 0;
}