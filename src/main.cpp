#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <stdexcept>

#include "plyUtils.hpp"
#include "tinyply/tinyply.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/IO/PLY/PLY_reader.h>
#include <CGAL/IO/PLY/PLY_writer.h>
#include <CGAL/circulator.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Surface_mesh<Point> Mesh;

const std::string filePath = "models/cube_blender.ply";

double triangle_area(const Point &p1, const Point &p2, const Point &p3) {
    Kernel::Vector_3 v1 = p2 - p1;
    Kernel::Vector_3 v2 = p3 - p1;
    return std::sqrt(CGAL::cross_product(v1, v2).squared_length()) / 2.0;
}

std::vector<int> valences;
std::vector<int> airs;

std::vector<int> histValence;
std::vector<int> histAir;

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

    std::cout << "sistValence:\n";

    for(int i = 0; i < histValence.size(); ++i)
        std::cout << histValence[i] << " ";

    std::cout << "\n";

    std::cout << "histAir:\n";

    for(int i = 0; i < histAir.size(); ++i)
        std::cout << histAir[i] << " ";

    std::cout << "\n";

    return 0;
}