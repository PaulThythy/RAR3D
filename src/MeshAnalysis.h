#ifndef MESHANALYSIS_H
#define MESHANALYSIS_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <vector>
#include <iostream>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef CGAL::Surface_mesh<Point> Mesh;

bool loadPLY(const std::string &filePath, Mesh &mesh);

bool loadOBJ(const std::string &filePath, Mesh &mesh);

void printMeshInfos(const Mesh &mesh);

std::vector<int> calculateVertexValences(const Mesh &mesh);

std::vector<double> calculateFaceAreas(const Mesh &mesh);

std::vector<double> calculateDihedralAngles(const Mesh &mesh);

#endif