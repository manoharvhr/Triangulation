#include "Triangulation.h"
using namespace std;

struct one { // Functor used for testing integration method
    float operator()(float x, float y) { // Simply returns 1 for any value given to it.
        return 1.0;
    }
};

int main()
{
    // The main() consists of the various tests conducted to showcase the correct functionality and usage of the classes provided.
    Triangulation myTriangulation; // Create an object to hold the mesh.
    char filename[] = "./triangulation#1.tri"; // Read and write file names.
    char filename2[] = "./triangulation#1_written.tri";

    std::ifstream myFile; // Creating a input file stream
    myFile.open(filename); // Open the file.
    if (!myFile.is_open()) // Verify if it is opened.
    {
        std::cerr << "Unable to open the file.\n"; // Print error message.
        return -1; // Specific error codes to help debug.
    }

    myFile >> myTriangulation; // Bring in data into the myTriangulation object.

    /********************************Test*1************************************/
    // Tests for isPointInTriangle() where the point lies inside the triangle with ID as 983.
    // Test 1
    Vertex myTestVertex1(172, -3); // Create a dummy vertex.
    vector<Triangle*> myTestContainer1; // Create a container to hold the output of the method.
    cout << "\nMy Test 1 result = \n";
    myTriangulation.isPointInAnyTriangle(myTestVertex1, myTestContainer1); // Calling the method with the dummy point and the container to hold triangles.
    for (vector<Triangle*>::iterator it = myTestContainer1.begin(); it != myTestContainer1.end(); ++it) { // Iterate through the container holding the result.
        cout << "The triangle number: " << (*it)->getId() << " Contains the point\n"; // Print the IDs.
    }

    /********************************Test*2************************************/
    // Tests for isPointInTriangle() where the point lies outside the triangle with ID as 983.
    // Test 2
    Vertex myTestVertex2(-69.3406, -1.875); // Create dummy point which actually happens to be the vertex for 7 triangles.
    std::vector<Triangle*> inTriangles2; // Container to hold the output.
    myTriangulation.isPointInAnyTriangle(myTestVertex2, inTriangles2); // Check if the point lies in any triangle and place the output in the provided container.
    cout << "\nMy Test 2 result = \n";
    for (std::vector<Triangle*>::iterator it = inTriangles2.begin(); it != inTriangles2.end(); ++it) // Iterate through the container.
    {
        cout << "The triangle number:" << (*it)->getId() << " Contains the point\n"; // Print out the IDs of the triangle which contain the point.
        // This tests an extreme case and showcases a clear problem of precision in the program.
    }

    /********************************Test*3************************************/
    // Test for calculateCircumcentre() where the properties such as the circumcentre and radius are printed.
    // Test 3
    myTriangulation.calculateCircumcentreOf(983); // calculate the properties of the circumcircle of the triangle with ID as 983.
    cout << "\nMy Test 3 result = \n";
    // Print the outputs which are placed in the Triangle objects:
    cout << "The circumcentre of triangle 983 is: Ox = " << (*myTriangulation.getMyTriangles().at(983)).getCircumcentrePoint()[0] << " Oy = " << (*myTriangulation.getMyTriangles().at(983)).getCircumcentrePoint()[1] << "\n";
    cout << "The radius of triangle 983 is " << (*myTriangulation.getMyTriangles().at(983)).getRadius() << "\n";

    /********************************Test*4************************************/
    // Test for isPointInCircumcircle() where the dummy point is chosen such that it lies inside the circumcircle.
    // Test 4
    // The dummy point is from Test 1.
    cout << "\nMy Test 4 result = \n";
    // Print the result of the test:
    cout << "Does first test point lie inside circumcircle: " << (*myTriangulation.getMyTriangles().at(983)).isPointInCircumcircle(myTestVertex1) << "\n";

    /********************************Test*5************************************/
    // Test for isPointInCircumcircle() where the dummy point does not lie inside.
    // Test 5
    Vertex myTestVertex3(169, -4); // Dummy point which actually lies outside the circumcircle of triangle 983.
    cout << "\nMy Test 5 result = \n";
    cout << "Does second test point lie inside circumcircle: " << (*myTriangulation.getMyTriangles().at(983)).isPointInCircumcircle(myTestVertex3) << "\n";

    /********************************Test*6************************************/
    // Test for calculateArea() where the area of a chosen triangle is calculated and compared with the 11th attribute of the triangle.
    // Test 6
    myTriangulation.calculateAreaOf(983); // Compute the area of the triangle with ID 983.
    cout << "\nMy Test 6 result = \n";
    cout << "Calculated area = " << (*myTriangulation.getMyTriangles().at(983)).getArea() << " "; // Print the value.
    // Square the area taken from the attributes as it stores the sqrt(area) and display it.
    cout << "Area from attributes: " << (*myTriangulation.getMyTriangles().at(983)).getAttributes()[10] * (*myTriangulation.getMyTriangles().at(983)).getAttributes()[10] << "\n";

    /********************************Test*7************************************/
    // Test for isDelaunay() to check whether the triangulation#1.tri mesh follows DT or not along with details of the points which violate the rules.
    // Test 7
    cout << "\nMy Test 7 result = \n";
    cout << "Is Delaunay? " << myTriangulation.isDelaunay() << "\n"; // Check and print the result.

    /********************************Test*8************************************/
    // Test for integration() using both approximations over a self-created .tri file which consists of a single triangle.
    // Therefore, the result should be the area of the triangle as the functor is simply a constant 1 signal.
    // Test 8
    cout << "\nMy Test 8 result = \n";
    Triangulation test8; // Create a Triangulation type object.
    std::ifstream myFileTest8; // Create a input file stream for the new test file.
    one myOne; // Create an instance of the functor.
    myFileTest8.open("./Test8.tri"); // Open the file.
    if (!myFileTest8.is_open()) // Verify if it is opened.
    {
        std::cerr << "Unable to open the file.\n"; // Print the error message if failed.
        return -2;
    }
    myFileTest8 >> test8; // Obtain the data.
    cout << "Linear Interpolation Approximation output = " << test8.integration<one>(myOne, false) << "\n"; // Perform the first integration type and print the result.
    cout << "Constant Approximation output = " << test8.integration<one>(myOne, true) << "\n"; // Perform the second integration type and print the result.
    test8.calculateAreaOf(0); // To verify the integration result, we must know the area of the single triangle in out mesh.
    cout << "Area of the triangle = " << (*test8.getMyTriangles().at(0)).getArea() << "\n"; // Print that value.

    /********************************Test*9************************************/
    // Test for operator<< Triangulation if we can write out to a file in the correct format.
    // Test 9
    cout << "\nMy Test 9 result = \n";
    std::ofstream myFileTest9; // Create an output stream.
    myFileTest9.open("./Test9.tri"); // Create a new file unless it is there.
    if (!myFileTest9.is_open()) // Check if it was opened or created.
    {
        std::cerr << "Unable to open the file.\n"; // Print an error message if not successful.
        return -3;
    }
    // Test with the bigger mesh as well.
    std::ofstream myFile2; // Create an output stream.
    myFile2.open(filename2); // Open the file.
    if (!myFile2.is_open()) // Verify if it was open.
    {
        std::cerr << "Unable to open the file.\n"; // Print an error message if not successful.
        return -4;
    }
    myFileTest9 << test8; // Write out.
    myFile2 << myTriangulation; // Write out.
    cout << "Completed!\n";

    /********************************Test*10************************************/
    // Test for addNewVertex() to check if we can add a new vertex with the correct data and ID in place.
    // Test 10
    cout << "\nMy Test 10 result = \n";
    float x(0), y(1), z(2); // Random coordinates.
    test8.addNewVertex(x, y, z); // Add the vertex.
    // Print out the vertex ID and coordinates from the container for verification.
    cout << (*test8.getMyPoints().back()).getId() << " " << (*test8.getMyPoints().back())[0] << " " << (*test8.getMyPoints().back())[1] << " " << (*test8.getMyPoints().back())[2] << "\n";

    /********************************Test*11************************************/
    // Test for addNewTriangle() to check if we can add a new triangle with the correct data and ID in place.
    // Test 11
    cout << "\nMy Test 11 result = \n";
    int v0(0), v1(1), v2(2); // Random vertex references.
    test8.addNewTriangle(v0, v1, v2); // Add the new triangle.
    // Print the ID and data using the container for verification.
    cout << (*test8.getMyTriangles().back()).getId() << " " << (*test8.getMyTriangles().back())[0] << " " << (*test8.getMyTriangles().back())[1] << " " << (*test8.getMyTriangles().back())[2] << "\n";

    // Close the files opened for all tests.
    myFile.close();
    myFile2.close();
    myFileTest8.close();
    myFileTest9.close();

    return 0;
}

