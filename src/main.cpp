#include <iostream>

#include "plyUtils.hpp"
#include "tinyply/tinyply.h"

#include <string>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/generators.h>

#include "examples/exempleLecturePly.h"

typedef CGAL::Simple_cartesian<double> K;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef Mesh::Vertex_index vertex_descriptor;
typedef Mesh::Face_index face_descriptor;

int verticesValency[5];
float verticesDiadricAngles[5];
float facesArea[5];

int main()
{
    // TODO nombre de sommets et de faces

    // TODO histogramme de la valence des sommets

    // TODO histogramme des angles dièdres

    // TODO calculer les aires des faces

    // TODO calculer les cartes de courbure et en prendre quelques images de "référence" (meshlab peut le faire)

    Mesh m;
    read_ply_file("models/example_cube-ascii.ply", false);

    

    return 0;
}