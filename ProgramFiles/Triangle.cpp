#include "Triangle.h"

/*
    The following method is used to evaluate whether the newPoint lies inside any triangle.
    This is the required query (part B). It has two inputs: newPoint which is the point to check, the second
    is the container of all points which will allow the triangle to identify its vertex coordinates.
    The return values is simply a true or false.
    Mathematics reference: https://math.stackexchange.com/questions/51326/determining-if-an-arbitrary-point-lies-inside-a-triangle-defined-by-three-points
*/
bool Triangle::isPointInTriangle(Vertex &newPoint, std::vector<Vertex*> &myPoints)
{
    float Ax((*myPoints.at(vertices[0]))[0]), Ay((*myPoints.at(vertices[0]))[1]); // Optimization: avoiding to read these values repeatedly.

    // Setting vetrices[0] as the base, we create these objects to hold the subtraction between points and the base.
    Vertex AB((*myPoints.at(vertices[1]))[0] - Ax, (*myPoints.at(vertices[1]))[1] - Ay); // AB(Bx - Ax, By - Ay)
    Vertex AC((*myPoints.at(vertices[2]))[0] - Ax, (*myPoints.at(vertices[2]))[1] - Ay); // AC(Cx - Ax, Cy - Ay)
    Vertex AP(newPoint[0] - Ax, newPoint[1] - Ay); // AP(Px - Ax, Py - Ay)

    // Applying the formulas suggested for the Barycentric weights and scalar d.
    float d(1.0 / ((AB[0] * AC[1]) - (AC[0] * AB[1]))); // Optimization: calculating 1/d to avoid multiple divisions below.
    float wa(((AP[0] * (AB[1] - AC[1])) + (AP[1] * (AC[0] - AB[0])) + ((AB[0] * AC[1]) - (AC[0] * AB[1]))) * d);
    float wb(((AP[0] * AC[1]) - (AP[1] * AC[0])) * d);
    float wc(((AP[1] * AB[0]) - (AP[0] * AB[1])) * d);

    // Evaluating the condition which determines the decision.
    if ((wa >= 0 && wa <= 1) && (wb >= 0 && wb <= 1) && (wc >= 0 && wc <= 1))
    {
        return true;
    }

    return false;
}

/*
    The following method is used to check whether or not the given newPoint lies inside the circumcircle
    of this triangle. Return value is boolean true or false. The function assumes that the circumcentre function was
    called before invoking this.
    Mathematics reference: https://math.stackexchange.com/questions/198764/how-to-know-if-a-point-is-inside-a-circle
*/
bool Triangle::isPointInCircumcircle(Vertex &newPoint)
{
    float temp(newPoint[0] - circumcentrePoint[0]), temp2(newPoint[1] - circumcentrePoint[1]); // Difference between the point and the centre.
    float distance(sqrt((temp * temp) + ((temp2) * (temp2)))); // Calculating the distance. Optimization: not using pow(, 2).

    // Considers the points on the circumference as well. But it is nullified if it is its own point by the invoker.
    return (distance <= getRadius()); // If the distance is less than or equal to the radius, it lies within the circle.
}

/*
    The following method is used to find the circumcentre of the triangle.
    The coordinates are stored in the Triangle's object itself. The mathematics applied here is
    taken from the appendix of the specifications.
*/
void Triangle::calculateCircumcentre(std::vector<Vertex*> &myPoints)
{
    Vertex centre; // Placeholder for the calculated centre.
    Vertex v0 = *myPoints.at(vertices[0]), v1 = *myPoints.at(vertices[1]), v2 = *myPoints.at(vertices[2]);

    // The following are calculations done to avoid repetition in the main formulas.
    float temp1(v0[0]*v1[1]), temp3(v0[0]*v0[0]), temp4(v1[0]*v0[1]), temp5(v1[0]*v2[1]), temp6(v1[0]*v1[0]), temp7(v2[0]*v2[0]), temp8(v0[1]*v0[1]);
    float temp2(1 / (2*(temp1 - temp4 - v0[0]*v2[1] + v2[0]*v0[1] + temp5 - v2[0]*v1[1])));

    // Calculating the coordinates.
    centre[0] = (v0[0]*temp1 - temp3*v2[1] - v1[0]*temp4 + v1[0]*temp5 + temp7*v0[1] - temp7*v1[1] + temp8*v1[1] - temp8*v2[1] - v0[1]*v1[1]*v1[1] + v0[1]*v2[1]*v2[1] + v1[1]*v1[1]*v2[1] - v1[1]*v2[1]*v2[1]) * temp2;
    centre[1] = (- temp3*v1[0] + temp3*v2[0] + v0[0]*temp6 - v0[0]*temp7 + temp1*v1[1] - v0[0]*v2[1]*v2[1] - temp6*v2[0] + v1[0]*temp7 - temp4*v0[1] + temp5*v2[1] + v2[0]*temp8 - v2[0]*v1[1]*v1[1]) * temp2;

    // Calculating the radius.
    radius = sqrt(temp3 + temp8 - 2*centre[0]*v0[0] - 2*centre[1]*v0[1]+centre[0]*centre[0]+centre[1]*centre[1]);

    this->setCircumcentrePoint(centre); // Storing the centre in the Triangle's object.
}

/*
    The following method is used to calculate the area of the triangle. The result
    is stored within that triangle's object. The mathematics applied is from https://sciencing.com/area-triangle-its-vertices-8489292.html
*/
void Triangle::calculateArea(std::vector<Vertex*> &myPoints) {
    Vertex a = *myPoints.at(vertices[0]), b = *myPoints.at(vertices[1]), c = *myPoints.at(vertices[2]); // Obtain the coordinates using the vertices.

    // Applying the formula.
    area = fabs((a[0] * (b[1] - c[1])) + (b[0] * (c[1] - a[1])) + (c[0] * (a[1] - b[1]))) / 2.0;
}
