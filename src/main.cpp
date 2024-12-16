#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <stdexcept>

#include "plyUtils.hpp"
#include "tinyply/tinyply.h"

#include <gnuplot-iostream.h>

#include "MeshAnalysis.h"
#include "Histogram.h"

const std::string filePath = "models/originalHead_VenusMilo.ply";

int main()
{
    Mesh mesh;
    if (!loadMeshFromFile(filePath, mesh)) {
        std::cerr << "Erreur : Impossible de lire le fichier PLY." << std::endl;
        return EXIT_FAILURE;
    }

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

    std::cin.get();

    return 0;
}
