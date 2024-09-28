#include <voro_helper.h>
#include <memory>

using namespace godot;

VoroCell::VoroCell() {
    
    //need to initalize properly in case we dont compute....
    centroid = Vector3(0, 0, 0);
	index = 0;
	neighbors = TypedArray<int>();
	num_polys = 0;
	num_verts = 0;
	poly_num_verts = TypedArray<int>();
	poly_indices = TypedArray<int>();
	verts = TypedArray<Vector3>();
}

VoroCell::~VoroCell() {
    neighbors.clear();
    verts.clear();
    poly_num_verts.clear();
    poly_indices.clear();
}

VoroCell::VoroCell(VoroCell& cell) {
	centroid = cell.centroid;
	index = cell.index;
	neighbors.append_array(cell.get_neighbors());
	num_polys = cell.num_polys;
	num_verts = cell.num_verts;
	poly_num_verts.append_array(cell.get_poly_num_verts());
	poly_indices.append_array(cell.get_poly_indices());
	verts.append_array(cell.get_verts());
}

VoroHelper::VoroHelper()
{
    Vector3 min = Vector3(-1, -1, -1);
    Vector3 max = Vector3(1, 1, 1);
    int num_cells = 1;
    cells = TypedArray<VoroCell>();
	Ref<VoroCell> c(memnew(VoroCell));
	// UtilityFunctions::print(c);
    cells.append(c);
	//UtilityFunctions::print(cells[0]);
}

VoroHelper::~VoroHelper()
{
    cells.clear();
}

void VoroHelper::compute_cells() {
	int i = 0, v = 0, fo = 0, fv = 0, n = 0;
    voro::container* cn = container;
    voro::particle_order* po = particle_order;
	voro::voronoicell_neighbor vc;
	voro::c_loop_order vl(*cn, *po);

	cells.clear();

	if(vl.start()) {
		do {
			//std::shared_ptr<VoroCell> cx = std::make_shared<VoroCell>(new VoroCell);
			Ref<VoroCell> c(memnew(VoroCell));

			if (cn->compute_cell(vc,vl)) {
				// adapted from voro++
				std::vector<double> vertices;
				std::vector<int> face_orders;
				std::vector<int> face_verts;
				std::vector<int> neighbors;
				double *pp, centroid[3];
				pp = vl.p[vl.ijk]+vl.ps*vl.q;

				// cell particle index
				c->set_index(cn->id[vl.ijk][vl.q]);

				// verts
				vc.vertices(*pp, pp[1], pp[2], vertices);
				c->set_num_verts(vc.p);
				TypedArray<Vector3> verts;
                verts.resize(c->get_num_verts());
				for (v = 0; v < c->get_num_verts(); v++) {
                    Vector3 cv = verts[v];
					cv[0] = vertices[v * 3];
					cv[1] = vertices[v * 3 + 1];
					cv[2] = vertices[v * 3 + 2];
                    verts[v] = cv;
				}
				c->set_verts(verts);

				// faces
				c->set_num_polys(vc.number_of_faces());
				vc.face_orders(face_orders);
				TypedArray<int> poly_num_verts;
                poly_num_verts.resize(c->get_num_polys());

				for (fo = 0; fo < c->get_num_polys(); fo++) {
					poly_num_verts[fo] = face_orders[fo];
				}

				vc.face_vertices(face_verts);
				TypedArray<int> poly_indices;
				int skip = 0;
				for (fo = 0; fo < c->get_num_polys(); fo++) {
					int num_verts = poly_num_verts[fo];
					for (fv = 0; fv < num_verts; fv++) {
						poly_indices.append(face_verts[skip + 1 + fv]);
					}
					skip += (num_verts+1);
				}

				c->set_poly_num_verts(poly_num_verts);
				c->set_poly_indices(poly_indices);

				// neighbors
				vc.neighbors(neighbors);
				TypedArray<int> neigh;
				neigh.resize(c->get_num_polys());
				for (n = 0; n < c->get_num_polys(); n++)
				{
					neigh[n] = neighbors[n];
				}
				c->set_neighbors(neigh);

				// centroid
				vc.centroid(centroid[0], centroid[1], centroid[2]);
				Vector3 centr;
				centr[0] = centroid[0] + *pp;
				centr[1] = centroid[1] + pp[1];
				centr[2] = centroid[2] + pp[2];
				c->set_centroid(centr);

				// volume
				c->set_volume(vc.volume());
			}

			cells.append(c);
			i++;
		}
		while(vl.inc());
	}
}

void VoroHelper::new_container() {
	int n_size = 8;

	if (container != nullptr) {
		delete container;
	}

    container = new voro::container(min[0], max[0], min[1], max[1], min[2], max[2],
								n_size, n_size, n_size, false, false, false,
								num_cells); 
}

void VoroHelper::new_particle_order() {
	if (particle_order != nullptr) {
		delete particle_order;
		particle_order = nullptr;
	}

	particle_order = new voro::particle_order();
}


void VoroHelper::put(int n, double x,double y,double z)
{
	voro::container* c = container;
	voro::particle_order* po = particle_order;
	
	if (po)
	{
		c->put(*po, n, x, y, z);
	}
}

void VoroHelper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_max"), &VoroHelper::get_max);
	ClassDB::bind_method(D_METHOD("set_max", "p_max"), &VoroHelper::set_max);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "max"), "set_max", "get_max");

	ClassDB::bind_method(D_METHOD("get_min"), &VoroHelper::get_min);
	ClassDB::bind_method(D_METHOD("set_min", "p_min"), &VoroHelper::set_min);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "min"), "set_min", "get_min");

	ClassDB::bind_method(D_METHOD("get_num_cells"), &VoroHelper::get_num_cells);
	ClassDB::bind_method(D_METHOD("set_num_cells", "p_max"), &VoroHelper::set_num_cells);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_cells"), "set_num_cells", "get_num_cells");

	ClassDB::bind_method(D_METHOD("get_cells"), &VoroHelper::get_cells);
	ClassDB::bind_method(D_METHOD("set_cells", "p_cells"), &VoroHelper::set_cells);

	ADD_PROPERTY(
		PropertyInfo(Variant::ARRAY, "cells", PROPERTY_HINT_TYPE_STRING,
		String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":VoroCell"), 
		"set_cells", "get_cells"
	);


	ClassDB::bind_method(D_METHOD("put"), &VoroHelper::put);
	ClassDB::bind_method(D_METHOD("compute_cells"), &VoroHelper::compute_cells);
	ClassDB::bind_method(D_METHOD("new_container"), &VoroHelper::new_container);
	ClassDB::bind_method(D_METHOD("new_particle_order"), &VoroHelper::new_particle_order);
}

void VoroCell::_bind_methods() {
	 //those are bound to readonly properties later

	ClassDB::bind_method(D_METHOD("get_verts"), &VoroCell::get_verts);
	ClassDB::bind_method(D_METHOD("get_poly_num_verts"), &VoroCell::get_poly_num_verts);
	ClassDB::bind_method(D_METHOD("get_poly_indices"), &VoroCell::get_poly_indices);

	ClassDB::bind_method(D_METHOD("get_neighbors"), &VoroCell::get_neighbors);
	ClassDB::bind_method(D_METHOD("get_centroid"), &VoroCell::get_centroid);
	ClassDB::bind_method(D_METHOD("get_index"), &VoroCell::get_index);

	ClassDB::bind_method(D_METHOD("get_volume"), &VoroCell::get_volume);
	ClassDB::bind_method(D_METHOD("get_num_verts"), &VoroCell::get_num_verts);
	ClassDB::bind_method(D_METHOD("get_num_polys"), &VoroCell::get_num_polys);

	ClassDB::bind_method(D_METHOD("set_verts", "p_verts"), &VoroCell::set_verts);
	ClassDB::bind_method(D_METHOD("set_poly_num_verts", "p_poly_num_verts"), &VoroCell::set_poly_num_verts);
	ClassDB::bind_method(D_METHOD("set_poly_indices", "p_poly_indices"), &VoroCell::set_poly_indices);

	ClassDB::bind_method(D_METHOD("set_neighbors", "p_neighbors"), &VoroCell::set_neighbors);
	ClassDB::bind_method(D_METHOD("set_centroid", "p_centroid"), &VoroCell::set_centroid);
	ClassDB::bind_method(D_METHOD("set_index", "p_index"), &VoroCell::set_index);

	ClassDB::bind_method(D_METHOD("set_volume", "p_volume"), &VoroCell::set_volume);
	ClassDB::bind_method(D_METHOD("set_num_verts", "p_num_verts"), &VoroCell::set_num_verts);
	ClassDB::bind_method(D_METHOD("set_num_polys", "p_num_polys"), &VoroCell::set_num_polys);


	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "verts"), "set_verts", "get_verts");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "poly_num_verts"), "set_poly_num_verts", "get_poly_num_verts");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "poly_indices"), "set_poly_indices", "get_poly_indices");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "neighbors"), "set_neighbors", "get_neighbors");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "centroid"), "set_centroid", "get_centroid");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume"), "set_volume", "get_volume");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_verts"), "set_num_verts", "get_num_verts");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_polys"), "set_num_polys", "get_num_polys");
}