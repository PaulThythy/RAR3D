#include <iostream>
#include <string>
#include "tinyply/tinyply.h"

#include <gnuplot-iostream.h>

#include <CGAL/boost/graph/IO/OBJ.h>
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>

#include "MeshAnalysis.h"
#include "Histogram.h"

namespace PMP = CGAL::Polygon_mesh_processing;

const std::string filePath = "models/suzanne.obj";

int main(int argc, char *argv[])
{
    Mesh mesh;
    if (!loadOBJ(filePath, mesh)) {
        std::cerr << "Erreur : Impossible de lire le fichier." << std::endl;
        return EXIT_FAILURE;
    }

    printMeshInfos(mesh);

    auto valences = calculateVertexValences(mesh);
    auto faceAreas = calculateFaceAreas(mesh);
    auto dihedralAngles = calculateDihedralAngles(mesh);

    auto histValence = createValencyHistogram(valences);
    auto histArea = createAreaHistogram(faceAreas);
    auto histDihedral = createDihedralHistogram(dihedralAngles);

    //plotHistogram(histValence, "Histogramme des valences des sommets", "Valence", "Nombre de sommets", 1);
    //plotHistogram(histArea, "Histogramme des aires des faces", "Aire", "Nombre de faces", 2);
    //plotHistogram(histDihedral, "Histogramme des angles dièdres", "Angle (radians)", "Nombre d arêtes", 3);

    PMP::smooth_shape(mesh, 0.0001, PMP::parameters::number_of_iterations(10));

    //std::cin.get();

    // Sauvegarde du maillage lissé dans un nouveau fichier
    std::string outputFile = "models/suzanne_smoothed.obj";
    {
        std::ofstream out(outputFile);
        if (!out || !CGAL::IO::write_OBJ(out, mesh)) {
            std::cerr << "Erreur : Impossible d'écrire le fichier lissé." << std::endl;
            return EXIT_FAILURE;
        }
    }

    return 0;
}
