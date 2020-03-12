#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector> // Needed for using the vector container.
#include "Vertex.h" // Each triangle will have vertexes.
#include "math.h" // Needed for mathematical operations used in methods.
/*
    The following Triangle class is used to store a triangle with its associated
    properties.
*/
class Triangle
{
public:
    Triangle() {;} // Constructor used for simply creating an object.

    Triangle(int node1, int node2, int node3, int lengthOfAttributes) // Overloaded constructor which initializes the vertexes.
    {
        vertices[0] = node1;
        vertices[1] = node2;
        vertices[2] = node3;
        attributes = new float[lengthOfAttributes]; // Allocate memory for attributes as well.
    }

    ~Triangle() { // Destructor.
        delete[] attributes; // clean up memory allocated for attributes.
    }

    void setVertices(int *vertices) // Setter uses provided array to set values.
    {
        for (int i = 0; i < 3; ++i)
        {
            this->vertices[i] = vertices[i];
        }
    }

    void setId(int id) // Sets the ID
    {
        this->id = id;
    }

    void setAttributes(float *attributes) // Setter for the attributes requires an array input. Note that it should be dynamically allocated.
    {
        this->attributes = attributes;
    }

    void setArea(float area) // Sets the area of the triangle.
    {
        this->area = area;
    }

    void setCircumcentrePoint(Vertex &point) // Given the circumcentre, the setter saves it to the triangle.
    {
        circumcentrePoint[0] = point[0];
        circumcentrePoint[1] = point[1];
    }

    void setRadius(float radius) // Sets the radius of the circumcircle.
    {
        this->radius = radius;
    }

    int getId() // Returns the ID of this triangle.
    {
        return id;
    }

    int* getVertices() // Getter for the vertices array.
    {
        return vertices;
    }

    float *getAttributes() // Returns the pointer to the array of attributes.
    {
        return attributes;
    }

    float getArea() // Returns the area of the triangle.
    {
        return area;
    }

    Vertex &getCircumcentrePoint() // Returns the circumcentre coordinates which are stored as a Vertex.
    {
        return circumcentrePoint;
    }

    float getRadius() // Returns the radius.
    {
        return radius;
    }

    bool isPointInTriangle(Vertex &newPoint, std::vector<Vertex*> &myPoints); // Method to check if a new point is inside a triangle.
    bool isPointInCircumcircle(Vertex &newPoint); // Checks whether a point is inside the circumcircle of this triangle.
    void calculateCircumcentre(std::vector<Vertex*> &myPoints); // Calculates the circumcentre.
    void calculateArea(std::vector<Vertex*> &myPoints); // Calculates the area of the triangle.

    friend bool operator<(Triangle &t0, Triangle &t1) // Less than operator used for comparing and sorting the triangles by ID.
    {
        return (t0.getId() < t1.getId());
    }

    int &operator[](int index) // As for the getters/setters of the coordinates, the operator [] is used to access each dimension.
    {
        return vertices[index];
    }

private:
    int vertices[3]; // Array stores the ID's of the vertex this triangle has. [0] => A, [1] => B, [2] => C
    int id; // Unique ID of the triangle.
    float *attributes; // Attributes of this triangle.
    float radius; // Radius of the circumcircle.
    Vertex circumcentrePoint; // Circumcentre of the triangle.
    float area; // Area of the triangle.
};

#endif
