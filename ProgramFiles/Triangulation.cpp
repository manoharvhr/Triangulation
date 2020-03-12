#include "Triangulation.h"

/*
    The following method checks whether a given newPoint lies within any triangles. There are two inputs.
    First of which is the point in question. The second is the container in which the triangles which contain
    that point will be pushed onto. Since only a few triangles are expected to have the point, no reserve is
    done on the container.
*/
void Triangulation::isPointInAnyTriangle(Vertex &newPoint, std::vector<Triangle*> &inTriangles)
{
    std::vector<Triangle*>::iterator it; // Iterator to access the container elements.

    for (it = myTriangles.begin(); it != myTriangles.end(); ++it) // Browse through all elements.
    {
        if ((**it).isPointInTriangle(newPoint, myPoints)) // Check if the point lies inside the triangle.
        {
            inTriangles.push_back(&(**it)); // If it does, go ahead and push it onto the return container.
        }
    }
}

/*
    The following method is used to find the circumcentre of the triangle with the provided ID.
    The coordinates are stored in the Triangle's object itself. The mathematics applied here is
    taken from the appendix of the specifications.
*/
void Triangulation::calculateCircumcentreOf(int id)
{
    (*myTriangles.at(id)).calculateCircumcentre(myPoints);
}

/*
    The following method is used to calculate the area of the triangle with the given ID. The result
    is stored within that triangle's object.
*/
void Triangulation::calculateAreaOf(int id)
{
    (*myTriangles.at(id)).calculateArea(myPoints);
}

/*
    The following method is used to calculate whether a new point is within the circumcircle of any
    old triangles. This is useful when deploying DT. The inputs are the newPoint and a container where the
    output will be placed. The output will be the triangles whose circumcircle has the point. The reason
    why the triangles are returned is so that they can be modified when deploying DT.
*/
void Triangulation::isPointInCircumcircle(Vertex &newPoint, std::vector<Triangle*> &inTriangles)
{
    for (std::vector<Triangle*>::iterator it = myTriangles.begin(); it != myTriangles.end(); ++it) // Loop to go through each triangle.
    {
        calculateCircumcentreOf((**it).getId()); // Calculate the circumcentre before beginning checks.
        if ((**it).isPointInCircumcircle(newPoint)) // Call the function which checks if a point is in the circumcentre.
        {
            // The if-else below is to skip the point if it actually belongs to the triangle.
            if ((*myPoints.at((**it)[0]))[0] == newPoint[0] && (*myPoints.at((**it)[0]))[1] == newPoint[1])
            {
                continue;
            }
            else if ((*myPoints.at((**it)[1]))[0] == newPoint[0] && (*myPoints.at((**it)[1]))[1] == newPoint[1])
            {
                continue;
            }
            else if ((*myPoints.at((**it)[2]))[0] == newPoint[0] && (*myPoints.at((**it)[2]))[1] == newPoint[1])
            {
                continue;
            }
            std::cout << "Point id: " << newPoint.getId() << " In triangle id: " << (**it).getId() << "\n"; // Used for checking points which lie inside the circumcircle.
            inTriangles.push_back(&(**it)); // Store the triangle in the container if it meets the criteria.
        }
    }
}

/*
    The following method is used to verify whether an existing point in the mesh
    is in the circumcircle of any triangle. Used for the Delaunay check. Input is the
    point whereas the output will be a true/false.
*/
bool Triangulation::isOldPointInCircumcircle(Vertex &oldPoint)
{
    int counter(0); // Initialise the counter to zero. Only for debugging.
    for (std::vector<Triangle*>::iterator it = myTriangles.begin(); it != myTriangles.end(); ++it) // Iterate through each triangle.
    {
        // Since this point is an existing point, it has an ID which we can use to filter the result if the point belongs to the triangle.
        if (oldPoint.getId() == (**it)[0] || oldPoint.getId() == (**it)[1] || oldPoint.getId() == (**it)[2])
        {
            continue; // Skip if it is a vertex of the triangle.
        }
        calculateCircumcentreOf((**it).getId()); // Otherwise, calculate the circumcentre.
        if ((**it).isPointInCircumcircle(oldPoint)) // Check if the point is in the circumcircle.
        {
            std::cout << "Point id: " << oldPoint.getId() << " In triangle id: " << (**it).getId() << "\n"; // Used for debugging.
            counter++; // Counts the number of times when the point was in the circumcircle of a triangle.
            //return true; // If we want to stop once we find one point, we can simply return from here.
        }
    }

    if (counter != 0) // If the debugging is used, then we return true if counter is not 0 otherwise remove this if-statement.
    {
        return true;
    }
    return false;
}

/*
    The following method checks whether the mesh data in this Triangulation
    object follows DT or no. It has no inputs as it assumes data is fed into
    the object already. The output is a true/false.
*/
bool Triangulation::isDelaunay()
{
    int counter(0); // Initialise counter to zero.
    for (std::vector<Vertex*>::iterator it = myPoints.begin(); it != myPoints.end(); ++it) // Iterate through each point.
    {
        // We check if any of the points is inside the circumcircle of a triangle.
        if (isOldPointInCircumcircle((**it))) // Check if it lies in the circumcircle of any triangle.
        {
            counter++; // Keep a track of the number of times it was inside the circumcircle.
            //return false; // If not debugging, we can simply return false at the first piece of evidence we find that it is not DT.
        }
    }
    if (counter != 0)
    {
        return false; // Return false as long as counter is more than 0.
    }
    return true;
}

/*
    The following method adds a new vertex to the container with the provided
    data. The ID of the vertex is simply an increment of the ID of the last vertex in the container.
    It is also important to update the number of points to make it consistent.
*/
void Triangulation::addNewVertex(float &x, float &y, float &z)
{
    Vertex *pointToAdd = new Vertex(); // dynamically allocate a new Vertex object.
    // Set the coordinates:
    (*pointToAdd)[0] = x;
    (*pointToAdd)[1] = y;
    (*pointToAdd)[2] = z;
    pointToAdd->setId((*myPoints.back()).getId() + 1); // Calculate and set the new ID based on the old one.
    numberOfPoints++; // Increment the total number of points.
    myPoints.push_back(pointToAdd); // Push back into the container.
}

/*
    The following method is used to add a new triangle. Similar to the above, the
    ID of the triangle is an increment of the last triangle. We also increment the
    number of triangles.
*/
void Triangulation::addNewTriangle(int &v0, int &v1, int &v2)
{
    Triangle *triangleToAdd = new Triangle(v0, v1, v2, numberOfAttributesPerCell); // Allocate memory for a new Triangle.
    triangleToAdd->setId((*myTriangles.back()).getId() + 1); // Calculate and set the new ID.
    numberOfCells++; // Increment the number of cells.
    myTriangles.push_back(triangleToAdd); // Add the triangle to the container.
}

