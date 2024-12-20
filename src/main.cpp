#include <iostream>
#include <string>
#include "tinyply/tinyply.h"

#include <gnuplot-iostream.h>

#include <CGAL/boost/graph/IO/OBJ.h>
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>

#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

#include <boost/iterator/function_output_iterator.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "MeshAnalysis.h"
#include "Histogram.h"

namespace PMP = CGAL::Polygon_mesh_processing;

std::string filePath;

int main(int argc, char *argv[])
{

    if(argc == 2)
    {
        filePath = std::string(argv[argc-1]);
    }
    else
    {
        std::cerr << "Object path missing\n";
        return EXIT_FAILURE;
    }

    Mesh mesh;
    if(!PMP::IO::read_polygon_mesh(filePath, mesh) || !CGAL::is_triangle_mesh(mesh))
    {
        std::cerr << "Invalid input." << std::endl;
        return 1;
    }

    CGAL::Polygon_mesh_processing::triangulate_faces(mesh);


    size_t pos = filePath.find_last_of("/\\");
    std::string nom_fichier_format = filePath.substr(pos + 1); //objet.obj
    std::string path = filePath.substr(0,pos + 1); //path

    size_t posExtention = nom_fichier_format.find_last_of('.');
    std::string nom_fichier = nom_fichier_format.substr(0, posExtention);


    printMeshInfos(mesh);

    auto valences = calculateVertexValences(mesh);
    auto faceAreas = calculateFaceAreas(mesh);
    auto dihedralAngles = calculateDihedralAngles(mesh);

    auto histValence = createValencyHistogram(valences);
    auto histArea = createAreaHistogram(faceAreas);
    auto histDihedral = createDihedralHistogram(dihedralAngles);

    plotHistogram(histValence, "Histogramme des valences des sommets", "Valence", "Nombre de sommets", 1);
    plotHistogram(histArea, "Histogramme des aires des faces", "Aire", "Nombre de faces", 2);
    plotHistogram(histDihedral, "Histogramme des angles dièdres", "Angle (radians)", "Nombre d arêtes", 3);

    PMP::smooth_shape(mesh, 0.0001, PMP::parameters::number_of_iterations(64));

    valences = calculateVertexValences(mesh);
    faceAreas = calculateFaceAreas(mesh);
    dihedralAngles = calculateDihedralAngles(mesh);

    histValence = createValencyHistogram(valences);
    histArea = createAreaHistogram(faceAreas);
    histDihedral = createDihedralHistogram(dihedralAngles);

    plotHistogram(histValence, "Histogramme des valences des sommets", "Valence", "Nombre de sommets", 1);
    plotHistogram(histArea, "Histogramme des aires des faces", "Aire", "Nombre de faces", 2);
    plotHistogram(histDihedral, "Histogramme des angles dièdres", "Angle (radians)", "Nombre d arêtes", 3);


    //std::cin.get();

    // Sauvegarde du maillage lissé dans un nouveau fichier
    std::string outputFile = path + nom_fichier + "_output.obj";
    {
        std::ofstream out(outputFile);
        if (!out || !CGAL::IO::write_OBJ(out, mesh)) {
            std::cerr << "Erreur : Impossible d'écrire le fichier lissé." << std::endl;
            return EXIT_FAILURE;
        }
    }

    return 0;
}
