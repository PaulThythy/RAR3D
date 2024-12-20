#include "MeshAnalysis.h"
#include <CGAL/IO/PLY/PLY_reader.h>
#include <CGAL/IO/OBJ.h>
#include <cmath>

bool loadPLY(const std::string &filePath, Mesh &mesh) {
    std::ifstream input(filePath);
    return input && CGAL::IO::read_PLY(input, mesh);
}

bool loadOBJ(const std::string &filePath, Mesh &mesh) {
    std::ifstream input(filePath);
    return input && CGAL::IO::read_OBJ(input, mesh);
}

void printMeshInfos(const Mesh &mesh) {
    size_t num_vertices = mesh.number_of_vertices();
    size_t num_faces = mesh.number_of_faces();
    std::cout << "Nombre de sommets : " << num_vertices << std::endl;
    std::cout << "Nombre de faces : " << num_faces << std::endl;
}

std::vector<int> calculateVertexValences(const Mesh &mesh) {
    std::vector<int> valences;
    for (auto vertex : mesh.vertices()) {
        int valence = 0;
        for (auto halfedge : mesh.halfedges_around_target(mesh.halfedge(vertex))) {
            ++valence;
        }
        valences.push_back(valence);
    }
    return valences;
}

std::vector<double> calculateFaceAreas(const Mesh &mesh) {
    std::vector<double> areas;

    for (auto face : mesh.faces()) {
        // Extraire tous les sommets de la face
        std::vector<Point> faceVertices;
        auto start = mesh.halfedge(face);
        auto curr = start;
        do {
            faceVertices.push_back(mesh.point(mesh.target(curr)));
            curr = mesh.next(curr);
        } while (curr != start);

        double area = 0.0;

        // Si la face est un triangle, on calcule directement
        if (faceVertices.size() == 3) {
            Vector v12 = faceVertices[1] - faceVertices[0];
            Vector v13 = faceVertices[2] - faceVertices[0];
            area = std::sqrt(CGAL::cross_product(v12, v13).squared_length()) / 2.0;
        }
        else {
            // on calcule l'aire de (v0, v1, v2) et (v0, v2, v3).
            for (size_t i = 1; i + 1 < faceVertices.size(); ++i) {
                Vector v1 = faceVertices[i]   - faceVertices[0];
                Vector v2 = faceVertices[i+1] - faceVertices[0];
                double triArea = std::sqrt(CGAL::cross_product(v1, v2).squared_length()) / 2.0;
                area += triArea;
            }
        }

        areas.push_back(area);
    }

    return areas;
}

std::vector<double> calculateDihedralAngles(const Mesh &mesh) {
    std::vector<double> angles;
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

            Vector n1 = CGAL::cross_product(v2 - v1, v3 - v1);
            Vector n2 = CGAL::cross_product(v5 - v4, v6 - v4);
            angles.push_back(std::acos(n1 * n2 / (std::sqrt(n1.squared_length()) * std::sqrt(n2.squared_length()))));
        }
    }
    return angles;
}