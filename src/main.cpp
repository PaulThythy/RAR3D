#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <stdexcept>

#include "plyUtils.hpp"
#include "tinyply/tinyply.h"

#include <gnuplot-iostream.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/IO/PLY/PLY_reader.h>
#include <CGAL/IO/PLY/PLY_writer.h>
#include <CGAL/circulator.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef CGAL::Surface_mesh<Point> Mesh;

const std::string filePath = "models/cube_blender.ply";

double triangle_area(const Point &p1, const Point &p2, const Point &p3) {
    Kernel::Vector_3 v1 = p2 - p1;
    Kernel::Vector_3 v2 = p3 - p1;
    return std::sqrt(CGAL::cross_product(v1, v2).squared_length()) / 2.0;
}

Vector triangle_normal(const Point &p1, const Point &p2, const Point &p3) {
    Kernel::Vector_3 v1 = p2 - p1;
    Kernel::Vector_3 v2 = p3 - p1;
    return CGAL::cross_product(v1, v2);
}

double dihedral_angle(const Vector &n1, const Vector &n2) {
    return std::acos(n1 * n2 / (std::sqrt(n1.squared_length()) * std::sqrt(n2.squared_length())));
}

std::vector<int> valences;
std::vector<int> airs;
std::vector<double> dihedralAngles;

std::vector<int> histValence;
std::vector<int> histAir;
std::vector<int> histDihedral;

int main()
{
    // TODO histogramme des angles dièdres

    // TODO calculer les cartes de courbure et en prendre quelques images de "référence" (meshlab peut le faire)

    Mesh mesh;
    std::ifstream input(filePath);
    if (!input || !(CGAL::IO::read_PLY(input, mesh))) {
        std::cerr << "Erreur : Impossible de lire le fichier PLY." << std::endl;
        return EXIT_FAILURE;
    }

    size_t num_vertices = mesh.number_of_vertices();
    size_t num_faces = mesh.number_of_faces();
    std::cout << "Nombre de sommets : " << num_vertices << std::endl;
    std::cout << "Nombre de faces : " << num_faces << std::endl;

    std::cout << "\nValences des sommets :" << std::endl;
    for (auto vertex : mesh.vertices()) {
        size_t valence = 0;
        
        // Parcourir les demi-arêtes autour du sommet
        for (auto halfedge : mesh.halfedges_around_target(mesh.halfedge(vertex))) {
            ++valence;
        }

        valences.push_back(valence);

        std::cout << "Sommet " << vertex << ": Valence = " << valence << std::endl;
    }

    for(int i = 0; i < valences.size(); i++)
    {
        if(histValence.size() <= valences[i])
            histValence.resize(valences[i]+1);

        histValence[valences[i]]++;
    }

    std::cout << "\nAires des faces :" << std::endl;
    for (auto face : mesh.faces()) {
        auto halfedge = mesh.halfedge(face);
        
        // Obtenir les sommets de la face
        auto v1 = mesh.point(mesh.target(halfedge));
        auto v2 = mesh.point(mesh.target(mesh.next(halfedge)));
        auto v3 = mesh.point(mesh.target(mesh.next(mesh.next(halfedge))));

        // Calculer l'aire
        double area = triangle_area(v1, v2, v3);
        
        airs.push_back(area);
        std::cout << "Face " << face << ": Aire = " << area << std::endl;
    }

    for(int i = 0; i < airs.size(); i++)
    {
        if(histAir.size() <= airs[i])
            histAir.resize(airs[i]+1);

        histAir[airs[i]]++;
    }

    for (auto edge : mesh.edges()) {
        if (!mesh.is_border(edge)) {
            auto h1 = mesh.halfedge(edge);
            auto h2 = mesh.opposite(h1);

            auto f1 = mesh.face(h1);
            auto f2 = mesh.face(h2);

            auto v1 = mesh.point(mesh.target(mesh.halfedge(f1)));
            auto v2 = mesh.point(mesh.target(mesh.next(mesh.halfedge(f1))));
            auto v3 = mesh.point(mesh.target(mesh.next(mesh.next(mesh.halfedge(f1)))));

            auto v4 = mesh.point(mesh.target(mesh.halfedge(f2)));
            auto v5 = mesh.point(mesh.target(mesh.next(mesh.halfedge(f2))));
            auto v6 = mesh.point(mesh.target(mesh.next(mesh.next(mesh.halfedge(f2)))));

            Vector n1 = triangle_normal(v1, v2, v3);
            Vector n2 = triangle_normal(v4, v5, v6);

            double angle = dihedral_angle(n1, n2);
            dihedralAngles.push_back(angle);
        }
    }

    const int numBins = 36; // Diviser les angles entre 0 et pi en 36 intervalles
    histDihedral.resize(numBins, 0);

    for (double angle : dihedralAngles) {
        int binIndex = static_cast<int>((angle / M_PI) * numBins);
        if (binIndex >= 0 && binIndex < numBins) {
            histDihedral[binIndex]++;
        }
    }


    Gnuplot gpVertexValency;
    std::vector<std::pair<int, int>> dataValency;

    for (size_t i = 0; i < histValence.size(); ++i) {
        dataValency.emplace_back(i, histValence[i]);
    }

    gpVertexValency << "set term wxt 1\n";
    gpVertexValency << "set title 'Histogramme des valences des sommets'\n";
    gpVertexValency << "set xlabel 'Valence'\n";
    gpVertexValency << "set ylabel 'Nombre de sommets'\n";
    gpVertexValency << "set style fill solid 0.5\n";
    gpVertexValency << "plot '-' with boxes title 'Valence'\n";
    gpVertexValency.send1d(dataValency);

    Gnuplot gpFaceArea;
    std::vector<std::pair<int, int>> dataArea;

    for (size_t i = 0; i < histAir.size(); ++i) {
        dataArea.emplace_back(i, histAir[i]);
    }

    gpFaceArea << "set term wxt 2\n";
    gpFaceArea << "set title 'Histogramme des aires des faces'\n";
    gpFaceArea << "set xlabel 'Aire'\n";
    gpFaceArea << "set ylabel 'Nombre de faces'\n";
    gpFaceArea << "set style fill solid 0.5\n";
    gpFaceArea << "plot '-' with boxes title 'Aire'\n";
    gpFaceArea.send1d(dataArea);

    Gnuplot gpDihedralAngles;
    std::vector<std::pair<double, int>> dataDihedralAngles;

    for (int i = 0; i < numBins; ++i) {
        dataDihedralAngles.emplace_back((i + 0.5) * (M_PI / numBins), histDihedral[i]);
    }

    gpDihedralAngles << "set term wxt 3\n";
    gpDihedralAngles << "set title 'Histogramme des angles dihèdres'\n";
    gpDihedralAngles << "set xlabel 'angles dihèdre'\n";
    gpDihedralAngles << "set ylabel 'Nombre arêtes'\n";
    gpDihedralAngles << "set style fill solid 0.5\n";
    gpDihedralAngles << "plot '-' with boxes title 'angle dihèdre'\n";
    gpDihedralAngles.send1d(dataDihedralAngles);

    std::cin.get();

    return 0;
}