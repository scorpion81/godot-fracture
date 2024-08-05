#include <voro_helper.h>
#include "../voro/src/voro++.cc"

using namespace godot;

VoroCell::VoroCell() {
    
    //need to initalize properly in case we dont compute....
    centroid = Vector3(0, 0, 0);
	index = 0;
	neighbors = TypedArray<int>();
	totpoly = 0;
	totvert = 0;
	poly_totvert = TypedArray<int>();
	poly_indices = TypedArray<TypedArray<int>>();
	verts = TypedArray<Vector3>();
}

VoroCell::~VoroCell() {
    neighbors.clear();
    verts.clear();
    poly_totvert.clear();
    poly_indices.clear();
}

VoroHelper::VoroHelper()
{
    Vector3 min = Vector3(-1, -1, -1);
    Vector3 max = Vector3(1, 1, 1);
    int num_cells = 10;

    container = VoroContainer();
    particle_order = VoroParticleOrder();

    cells = TypedArray<VoroCell>();
    cells.resize(();
    const VoroCell &c = VoroCell();
    cells.fill(c);
}

VoroHelper::~VoroHelper()
{
    delete container;
    delete particle_order;
    cells.clear();
}

void VoroHelper::compute_cells() {
	int i = 0, v = 0, fo = 0, fv = 0, n = 0;
    voro::container* cn = container.handle;
    voro::particle_order *po = particle_order.handle;
	voro::voronoicell_neighbor vc;
	voro::c_loop_order vl(*cn, *po);
	VoroCell c;

	if(vl.start()) {
		do {
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
				c.totvert = vc.p;
				c.verts = TypedArray<Vector3>();
                c.verts.resize(c.totvert);
				for (v = 0; v < c.totvert; v++) {
                    Vector3 cv = c.verts[v];
					cv[0] = verts[v * 3];
					cv[1] = verts[v * 3 + 1];
					cv[2] = verts[v * 3 + 2];
                    c.verts[v] = cv;
				}

				// faces
				c.totpoly = vc.number_of_faces();
				vc.face_orders(face_orders);
				c.poly_totvert = TypedArray<int>(); 
                c.poly_totvert.resize(c.totpoly);

				for (fo = 0; fo < c.totpoly; fo++) {
					c.poly_totvert[fo] = face_orders[fo];
				}

				vc.face_vertices(face_verts);
				c.poly_indices = TypedArray<TypedArray<int>>();
                c.poly_indices.resize(c.totpoly);
				int skip = 0;
				for (fo = 0; fo < c.totpoly; fo++) {
					int num_verts = c.poly_totvert[fo];
                    TypedArray<int> pv = c.poly_indices[fo];
                    pv.resize(num_verts);
					for (fv = 0; fv < num_verts; fv++) {
						pv[fv] = face_verts[skip + 1 + fv];
					}
                    c.poly_indices[fo] = pv;
					skip += (num_verts+1);
				}

				// neighbors
				vc.neighbors(neighbors);
				c.neighbors.resize(c.totpoly);
				for (n = 0; n < c.totpoly; n++)
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
				cells[i] = c;

			}
			else { // invalid cell, set NULL XXX TODO (Somehow !!!)
				c.centroid[0] = 0.0;
				c.centroid[1] = 0.0;
				c.centroid[2] = 0.0;
				c.index = 0;
				c.neighbors = TypedArray<int>();
				c.totpoly = 0;
				c.totvert = 0;
				c.poly_totvert = TypedArray<int>();
				c.poly_indices = TypedArray<TypedArray<int>>();
				c.verts = TypedArray<Vector3>();
				cells[i] = c;
			}
			i++;
		}
		while(vl.inc());
	}
}


void VoroHelper::put(int n, double x,double y,double z)
{
	voro::container* c = container.handle;
	voro::particle_order* po = particle_order.handle;
	
	if (po)
	{
		c->put(*po, n, x, y, z);
	}
}

VoroContainer::VoroContainer() {
    int n_size = 8;

    min = Vector3(-1, -1, -1);
    max = Vector3(1, 1, 1);
    num_cells = 10;

    handle = new voro::container(min[0], max[0], min[1], max[1], min[2], max[2],
								n_size, n_size, n_size, false, false, false,
								num_cells);
}

VoroContainer::VoroContainer(const VoroContainer& con) {
    min = con.min;
    max = con.max;
    num_cells = con.num_cells;
    int n_size = 8;

    handle = new voro::container(min[0], max[0], min[1], max[1], min[2], max[2],
								n_size, n_size, n_size, false, false, false,
								num_cells);
};

VoroContainer::~VoroContainer() {
    delete handle;
}

VoroParticleOrder::VoroParticleOrder() {
    handle = new voro::particle_order();
}

VoroParticleOrder::~VoroParticleOrder() {
    delete handle;
}