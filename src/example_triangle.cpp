//RRaffin, UB, LIB
//Exemple simple : description d'un triangle, extractions des informations
#include <string>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/generators.h>

//futur type des coordonnées
typedef CGAL::Simple_cartesian<double> K;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef Mesh::Vertex_index vertex_descriptor;

int main()
{

	//création d'un objet maillage
	Mesh m;

	//3 vertex descriptor avec un Point_3 chacun
	vertex_descriptor v0 = m.add_vertex(K::Point_3(0,2,0));
	vertex_descriptor v1 = m.add_vertex(K::Point_3(2,2,0));
	vertex_descriptor v2 = m.add_vertex(K::Point_3(0,0,0));
  
	//création de la face
	m.add_face(v0, v1, v2);

	//sortie simple des "statistiques"
	std::cout << "Nombres de sommets : " << num_vertices(m) << std::endl;
	std::cout << "Nombres d'arêtes : " << num_edges(m) << std::endl;
	std::cout << "Nombres de faces : " << num_faces(m) << std::endl;
  
	//utilisation de propriétés : "location" référence les positions
	Mesh::Property_map<vertex_descriptor, K::Point_3> location = m.points();

	//sortie de la propriété "location"
	for(vertex_descriptor vd : m.vertices()) {
		std::cout << location[vd] << std::endl;
	}

	//autre manière, par l'extraction des sommets, conservation des propriétés
	for (auto v : vertices(m)) {
		std::cout << location[v] << std::endl;
	}

	//manière plus générale, par un objet vecteur de std::string
	std::cout << "Propriétés du maillage :" << std::endl;
	std::vector<std::string> props = m.properties<vertex_descriptor>();
	for(std::string p : props){
		std::cout << p << std::endl;
	}

return 0;
}
