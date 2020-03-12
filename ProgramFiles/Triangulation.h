#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "Triangle.h" // Triangulation requires triangles and other header files within this.
#include <fstream> // File streaming.
#include <algorithm> // Sorting algorithm from STL.
#include <iostream> // To print out some information on the screen.

/*
    The following class holds information of the mesh. It is the main interface
    the user will use to interact with the Triangulation mesh.
*/
class Triangulation
{
public:
    Triangulation() {;} // Constructor

    ~Triangulation()   // Destructor for cleaning up the container objects.
    {
        delete[] temp1;
        delete[] temp2;
    }

    std::vector<Vertex*>& getMyPoints() // Returns a reference to the container.
    {
        return myPoints;
    }

    std::vector<Triangle*>& getMyTriangles() // Returns a reference to the container.
    {
        return myTriangles;
    }

    int getNumberOfPoints() // Provides the number of vertexes.
    {
        return numberOfPoints;
    }

    int getNumberOfDimensions() // Provides the number of dimensions in the grid space.
    {
        return numberOfDimensions;
    }

    int getNumberOfAttributesPerPoint() // Provides the number of attributes each vertex has.
    {
        return numberOfAttributesPerPoint;
    }

    int getNumberOfCells() // Provides the number of triangles.
    {
        return numberOfCells;
    }

    int getNumberOfVerticesPerCell() // Provides the number of vertices per cell. A triangle will always have three.
    {
        return numberOfVerticesPerCell;
    }

    int getNumberOfAttributesPerCell() // Number of attributes per triangle.
    {
        return numberOfAttributesPerCell;
    }

    void setNumberOfPoints(int numberOfPoints) // Sets the number of points/vertexes.
    {
        this->numberOfPoints = numberOfPoints;
    }

    void setNumberOfDimensions(int numberOfDimensions) // Sets the number of dimensions.
    {
        this->numberOfDimensions = numberOfDimensions;
    }

    void setNumberOfAttributesPerPoint(int numberOfAttributesPerPoint) // Sets the number of attributes per vertex.
    {
        this->numberOfAttributesPerPoint = numberOfAttributesPerPoint;
    }

    void setNumberOfCells(int numberOfCells) // Sets the number of triangles.
    {
        this->numberOfCells = numberOfCells;
    }

    void setNumberOfVerticesPerCell(int numberOfVerticesPerCell) // Sets the number of vertices a triangle has. Would not make sense if not 3.
    {
        this->numberOfVerticesPerCell = numberOfVerticesPerCell;
    }

    void setNumberOfAttributesPerCell(int numberOfAttributesPerCell) // Sets the number of attributes per triangle.
    {
        this->numberOfAttributesPerCell = numberOfAttributesPerCell;
    }

    void isPointInAnyTriangle(Vertex &newPoint, std::vector<Triangle*> &inTriangles); // Method fills the inTriangles container with triangles which contain the newPoint.
    void calculateCircumcentreOf(int id); // Method calculates the circumcentre point of the triangle with ID id and stores it in the triangle's object.
    void calculateAreaOf(int id); // Calculates the area of the triangle with ID as id and stores it in the triangle's object.
    void isPointInCircumcircle(Vertex &newPoint, std::vector<Triangle*> &inTriangles); // Method to evaluate whether the given newPoint lies inside the circumcircle of any triangle in the mesh. The triangles are returned in inTriangles.
    bool isOldPointInCircumcircle(Vertex &oldPoint); // Method which checks if any of the old points in the mesh already lie in the circumcentre of any triangle.
    bool isDelaunay(); // Method to check if this mesh is DT.
    void addNewVertex(float &x, float &y, float &z); // To add a new point.
    void addNewTriangle(int &v0, int &v1, int &v2); // To add a new triangle into the mesh.

    template<typename T>
    float integration(T t, bool method); // Method for integration which uses wildcard T for the function to integrate over the triangle domain.

    template<typename T>
    friend T &operator>>(T &myFile, Triangulation &myTriangulation) // Stream operator to read data into the object. Assumes only files.
    {
        myTriangulation.readPoints(myFile); // Reads the first segment of the file which are the vertexes.
        myTriangulation.readCells(myFile); // Followed by the triangles.
        std::sort(myTriangulation.myPoints.begin(), myTriangulation.myPoints.end()); // Sorts points according to ID to ensure that when accessing the container the at() method is consistent. Assumes that IDs are not skipped.
        std::sort(myTriangulation.myTriangles.begin(), myTriangulation.myTriangles.end()); // Sorts the triangles according to their IDs assuming that none are skipped.
        // If IDs are skipped, we can implement a Predicate function and use find_if but this will be immensely slow.
        return myFile;
    }

    friend std::ofstream &operator<<(std::ofstream &myFile, Triangulation &myTriangulation) // Stream operator to write data to a file. Assumes only files
    {
        myTriangulation.writePoints(myFile); // Writes the vertexes out first.
        myTriangulation.writeCells(myFile); // Followed by the triangles.
        return myFile;
    }

private:
    std::vector<Vertex*> myPoints; // Container with all vertexes.
    std::vector<Triangle*> myTriangles; // Container with all triangles

    // Properties read from the data files:
    int numberOfPoints, numberOfDimensions, numberOfAttributesPerPoint;
    int numberOfCells, numberOfVerticesPerCell, numberOfAttributesPerCell;

    // Pointers to objects in the containers used to deallocate when destroying this object.
    Triangle *temp1;
    Vertex *temp2;

    // I/O methods called by the stream operators above. All incorporate wildcard input types for flexibility.
    template<typename T>
    void readPoints(T &myFile);

    template<typename T>
    void readCells(T &myFile);

    template<typename T>
    void writePoints(T &myFile);

    template<typename T>
    void writeCells(T &myFile);

    // Integration methods to provide modular design for the integration() method.
    template<typename T>
    float constantValueApprox(T t);

    template<typename T>
    float linearInterpolationApprox(T t);
};

/*
    The following method uses generic programming and has a wildcard T. The purpose of this is to
    allow the user to provide a function which we will integrate over the triangle's domain. The
    calculation is done based on the second input of the function, which decides whether Constant
    Value Approximation (0) or Linear Interpolation Approximation (1) is performed. Based on this,
    a float value is returned.
*/
template<typename T>
float Triangulation::integration(T t, bool method) // Method for integration which uses wildcard T for the function to integrate over the triangle domain.
{
    if (method) // Based on selection.
    {
        // Constant Value Approximation
        return constantValueApprox<T>(t);
    }
    else
    {
        // Linear Interpolation Approximation
        return linearInterpolationApprox<T>(t);
    }
}

/*
    Templated method to calculate the integral using Constant Value Approximation. The
    method uses the area and the circumcentre of the triangle which is passed to the function.
*/
template<typename T>
float Triangulation::constantValueApprox(T t)
{
    std::vector<Triangle*>::iterator it;
    float sum = 0; // Summing variable initialised to 0.
    for (it = myTriangles.begin(); it != myTriangles.end(); ++it) // Iterate through each triangle.
    {
        calculateAreaOf((**it).getId()); // Calculate the area of the triangle.
        calculateCircumcentreOf((**it).getId()); // Calculate the circumcentre.
        // sum += A * fc
        sum += ((**it).getArea() * t((**it).getCircumcentrePoint()[0], (**it).getCircumcentrePoint()[1])); // Calculate the integral and accumulate it.
    }
    return sum;
}

/*
    Templated method used for calculating the integral using Linear Interpolation Approximation.
    The implementation uses the value of the function at the vertex and multiplies it to a fraction of the
    area of the triangle.
*/
template<typename T>
float Triangulation::linearInterpolationApprox(T t)
{
    std::vector<Triangle*>::iterator it;
    float sum = 0; // Summing variable initialised to 0.
    for (it = myTriangles.begin(); it != myTriangles.end(); ++it) // Also iterates through all triangles.
    {
        calculateAreaOf((**it).getId()); // Calculate the area of the triangle.
        // sum += (A/3) * (f0 + f1 + f2)
        sum += ((((**it).getArea()) / 3) * (t((*myPoints.at((**it)[0]))[0], (*myPoints.at((**it)[0]))[1]) + t((*myPoints.at((**it)[1]))[0], (*myPoints.at((**it)[1]))[1]) + t((*myPoints.at((**it)[2]))[0], (*myPoints.at((**it)[2]))[1])));
    }
    return sum;
}

/*
    The readPoints method reads the points used by the triangle. It has a single input
    which is the input file stream passed by reference. Data is then stored appropriately.
*/
template<typename T>
void Triangulation::readPoints(T &myFile)
{
    int tempId; // Temporary ID storage.

    // Read the line with information about the structure of the rest of the points.
    myFile >> numberOfPoints >> numberOfDimensions >> numberOfAttributesPerPoint;

    temp2 = new Vertex[numberOfPoints]; // Allocate memory based on the number of points.

    myPoints.reserve(numberOfPoints-1); // Reserve enough space in the container to store the points we have in the file.
    for (int j = 0; j < numberOfPoints; ++j) // The rest of the data is of the same format hence looping through it using the number of points mentioned
    {
        myFile >> tempId; // Store temporary ID.
        temp2[j].setId(tempId); // Set the value to the temporary Vertex object.
        for (int i = 0; i < numberOfDimensions; ++i) // Read and store the coordinate value in each dimension.
        {
            myFile >> (temp2[j])[i]; // Copy the values into the Vertex object.
        }
        myPoints.push_back(&temp2[j]); // Push the object into the container.
    }
}

/*
    readCells is similar to readPoints but since the triangles have attributes,
    it takes that into account as well. The input is the input file stream and the
    output is stored into the Triangle container.
*/
template<typename T>
void Triangulation::readCells(T &myFile)
{
    int tempId; // Unique ID of the triangle.

    // Reading the information on the structure of the triangles.
    myFile >> numberOfCells >> numberOfVerticesPerCell >> numberOfAttributesPerCell;

    temp1 = new Triangle[numberOfCells]; // Allocate memory based on the number of triangles.

    float *attributes;
    myTriangles.reserve(numberOfCells-1); // Reserve memory in the container for all data.
    for (int j = 0; j < numberOfCells; ++j) // Going through each line using the number of cells as the limit.
    {
        attributes = new float[numberOfAttributesPerCell];
        temp1[j].setAttributes(attributes);
        myFile >> tempId; // Reading the unique ID of this triangle.
        temp1[j].setId(tempId); // Storing the ID within the object.
        for (int i = 0; i < numberOfVerticesPerCell; ++i) // Reading the ID of the vertex a triangle has.
        {
            myFile >> (temp1[j])[i];
        }

        for (int i = 0; i < numberOfAttributesPerCell; ++i) // Reading the attributes.
        {
            myFile >> attributes[i];
        }

        myTriangles.push_back(&temp1[j]); // Pushing the filled object onto the container.
    }
}

/*
    The following method is used to write points to a provided file. The input is an
    input file stream reference.
*/
template<typename T>
void Triangulation::writePoints(T &myFile)
{
    // First writing the information on the structure of this segment.
    myFile << numberOfPoints << " " << numberOfDimensions << " " << numberOfAttributesPerPoint << "\n";

    for (std::vector<Vertex*>::iterator it = myPoints.begin(); it != myPoints.end(); ++it) // Loop through each object.
    {
        myFile << (**it).getId() << " " << (**it)[0] << " " << (**it)[1] << " " << (**it)[2] << "\n"; // Write it out.
    }
}

/*
    Similar to the above method, the following writes out the cells/triangles to the
    provided file reference.
*/
template<typename T>
void Triangulation::writeCells(T &myFile)
{
    // Pointers used for accessing.
    float *attributes;

    // Information header.
    myFile << numberOfCells << " " << numberOfVerticesPerCell << " " << numberOfAttributesPerCell << "\n";

    for (std::vector<Triangle*>::iterator it = myTriangles.begin(); it != myTriangles.end(); ++it) // Going through each triangle.
    {
        attributes = (*it)->getAttributes(); // Get the attributes.
        myFile << (*it)->getId() << " " << (**it)[0] << " " << (**it)[1] << " " << (**it)[2] << " "; // Write it out in the correct format.
        for (int i = 0; i < numberOfAttributesPerCell; ++i) // Write out the attributes.
        {
            myFile << attributes[i] << " ";
        }
        myFile << "\n";
    }

}

#endif
