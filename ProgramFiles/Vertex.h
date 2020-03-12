#ifndef VERTEX_H
#define VERTEX_H

/*
    The following Vertex class is responsible for holding a coordinate in a grid space.
    The class is hardwired for a 3 dimensional coordinate and has no attributes.
*/
class Vertex
{
public:
    Vertex() { // Constructor for simply creating the object.

    }

    Vertex(float x, float y) // Constructor with initialization of coordinates except z.
    {
        coordinate[0] = x;
        coordinate[1] = y;
    }

    Vertex(float x, float y, float z) // Constructor with initialization of coordinates.
    {
        coordinate[0] = x;
        coordinate[1] = y;
        coordinate[2] = z;
    }

    ~Vertex()   // Destructor
    {

    }

    float* getCoordinate() // Returns a pointer to the coordinate array.
    {
        return coordinate;
    }

    /*
        Setters for coordinates have been overloaded to provide access in multiple ways.
        The one below simply sets the coordinate array values from the array provided by the caller.
    */
    void setCoordinate(float *coordinate)
    {
        for (int i = 0; i < 3; ++i)
        {
            this->coordinate[i] = coordinate[i];
        }
    }

    int getId() // Getter for the ID of the vertex from the file.
    {
        return id;
    }

    void setId(int id) // Modifying the ID of the vertex.
    {
        this->id = id;
    }

    friend bool operator<(Vertex &v0, Vertex &v1) // This operator allows the use of sort algorithm to ensure that the vertexes are in ID order.
    {
        return (v0.getId() < v1.getId());
    }

    float &operator[](int index) // As for the getters/setters of the coordinates, the operator [] is used to access each dimension.
    {
        return coordinate[index];
    }

private:
    float coordinate[3]; // Array for holding the coordinates.
    int id; // ID of this point used by the triangles to reference to.
};

#endif
