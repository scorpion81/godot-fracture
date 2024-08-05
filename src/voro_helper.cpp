#include <voro_helper.h>
#include "../voro/src/voro++.cc"

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

VoroHelper::VoroHelper()
{
    Vector3 min = Vector3(-1, -1, -1);
    Vector3 max = Vector3(1, 1, 1);
    int num_cells = 1;
	int i = 0;
	int n_size = 8;

    container = new voro::container(min[0], max[0], min[1], max[1], min[2], max[2],
								n_size, n_size, n_size, false, false, false,
								num_cells);

    particle_order = new voro::particle_order();
    cells = TypedArray<VoroCell>();
    cells.resize(num_cells);
	VoroCell c; //better on heap ?
    cells[0] = Resource::cast_to<VoroCell>(&c);
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

	if(vl.start()) {
		do {
			VoroCell c;
			if (cn->compute_cell(vc,vl)) {
				// adapted from voro++
				std::vector<double> verts;
				std::vector<int> face_orders;
				std::vector<int> face_verts;
				std::vector<int> neighbors;
				double *pp, centroid[3];
				pp = vl.p[vl.ijk]+vl.ps*vl.q;

				// cell particle index
				c.index = cn->id[vl.ijk][vl.q];

				// verts
				vc.vertices(*pp, pp[1], pp[2], verts);
				c.num_verts = vc.p;
				c.verts = TypedArray<Vector3>();
                c.verts.resize(c.num_verts);
				for (v = 0; v < c.num_verts; v++) {
                    Vector3 cv = c.verts[v];
					cv[0] = verts[v * 3];
					cv[1] = verts[v * 3 + 1];
					cv[2] = verts[v * 3 + 2];
                    //c.verts[v] = cv;
				}

				// faces
				c.num_polys = vc.number_of_faces();
				vc.face_orders(face_orders);
				c.poly_num_verts = TypedArray<int>(); 
                c.poly_num_verts.resize(c.num_polys);

				for (fo = 0; fo < c.num_polys; fo++) {
					c.poly_num_verts[fo] = face_orders[fo];
				}

				vc.face_vertices(face_verts);
				c.poly_indices = TypedArray<int>();
				int skip = 0;
				for (fo = 0; fo < c.num_polys; fo++) {
					int num_verts = c.poly_num_verts[fo];
					for (fv = 0; fv < num_verts; fv++) {
						c.poly_indices[skip + 1 + fv] = face_verts[skip + 1 + fv];
					}
					skip += (num_verts+1);
				}

				// neighbors
				vc.neighbors(neighbors);
				c.neighbors.resize(c.num_polys);
				for (n = 0; n < c.num_polys; n++)
				{
					c.neighbors[n] = neighbors[n];
				}

				// centroid
				vc.centroid(centroid[0], centroid[1], centroid[2]);
				c.centroid[0] = centroid[0] + *pp;
				c.centroid[1] = centroid[1] + pp[1];
				c.centroid[2] = centroid[2] + pp[2];

				// volume
				c.volume = vc.volume();

				// valid cell, store
				cells[i] = Resource::cast_to<VoroCell>(&c);

			}
			else { // invalid cell, set NULL XXX TODO (Somehow !!!)
				c.centroid[0] = 0.0;
				c.centroid[1] = 0.0;
				c.centroid[2] = 0.0;
				c.index = 0;
				c.neighbors = TypedArray<int>();
				c.num_polys = 0;
				c.num_verts = 0;
				c.poly_num_verts = TypedArray<int>();
				c.poly_indices = TypedArray<int>();
				c.verts = TypedArray<Vector3>();
				cells[i] = Resource::cast_to<VoroCell>(&c);
			}
			i++;
		}
		while(vl.inc());
	}
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
	ClassDB::bind_method(D_METHOD("put"), &VoroHelper::put);
	ClassDB::bind_method(D_METHOD("compute_cells"), &VoroHelper::compute_cells);
}