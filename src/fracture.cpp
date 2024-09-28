#include "fracture.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Fracture3D::Fracture3D() {
	// Initialize any variables here.
	shard_count = 1;
	shard_seed = 0;
}

Fracture3D::~Fracture3D() {
	// Add your cleanup here.
}


void Fracture3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_shard_count"), &Fracture3D::get_shard_count);
	ClassDB::bind_method(D_METHOD("set_shard_count", "p_shard_count"), &Fracture3D::set_shard_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shard_count", PROPERTY_HINT_RANGE, "1,200,1"), "set_shard_count", "get_shard_count");

	ClassDB::bind_method(D_METHOD("get_shard_seed"), &Fracture3D::get_shard_seed);
	ClassDB::bind_method(D_METHOD("set_shard_seed", "p_shard_seed"), &Fracture3D::set_shard_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shard_seed"), "set_shard_seed", "get_shard_seed");

	ClassDB::bind_method(D_METHOD("get_cells"), &Fracture3D::get_cells);
	ClassDB::bind_method(D_METHOD("set_cells", "p_cells"), &Fracture3D::set_cells);

	ADD_PROPERTY(
		PropertyInfo(Variant::ARRAY, "cells", PROPERTY_HINT_TYPE_STRING, 
		String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":VoroCell"), 
		"set_cells", "get_cells"
	);

    ClassDB::bind_method(D_METHOD("execute_fracture"), &Fracture3D::execute_fracture);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "shard_seed"), "set_shard_seed", "get_shard_seed");

	//ADD_SIGNAL(MethodInfo("position_changed", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::VECTOR2, "new_pos")))
}


void Fracture3D::_process(double delta) {
}

void Fracture3D::set_shard_count(const int p_shard_count) {
	shard_count = p_shard_count;
}

int Fracture3D::get_shard_count() const {
	return shard_count;
}

void Fracture3D::set_shard_seed(const int p_shard_seed) {
	shard_seed = p_shard_seed;
}

int Fracture3D::get_shard_seed() const {
	return shard_seed;
}


void Fracture3D::execute_fracture() {
	// get mesh from current mesh instance
	// construct bounding box

	// make pointcloud across boundingbox (perhaps transform the bbox for it beforehand, hence get a transformed pointcloud)
	// or apply the transform on each point generated in the cloud ?

	// "untransform" mesh... mesh should have "Identity matrix", and the transform is in the node
	// make a voro++ container from untransformed bbox, add transformed points into it, discard points out of box

	Ref<Mesh> me = get_mesh();
	//UtilityFunctions::print(me);

	AABB bbox = me->get_aabb();
	Vector3 min = bbox.get_position();
	Vector3 max = bbox.get_end();
	int n_size = 8;
	int s = 0;

	// point cloud
	//TypedArray<Vector3> *points = new TypedArray<Vector3>();
	//points->resize(shard_count);

	RandomNumberGenerator *rng = new RandomNumberGenerator();
	rng->randomize();
	rng->set_seed(shard_seed);

	// UtilityFunctions::print(shard_count, shard_seed);

	Ref<VoroHelper> vh(memnew(VoroHelper));
	vh->set_min(min);
	vh->set_max(max);
	vh->set_num_cells(shard_count);
	vh->new_container();
	vh->new_particle_order();

	for (s = 0; s < shard_count; s++) {
		double x = rng->randf_range(min[0], max[0]);
		double y = rng->randf_range(min[1], max[1]);
		double z = rng->randf_range(min[2], max[2]);

		vh->put(s, x, y, z);
		//print("%d %f %f %f\n", s, x, y, z);
		//UtilityFunctions::print(s, x, y, z);
	}
	
	/* we expect as many raw cells as we have particles */
	//voro_cells = cells_new(pointcloud->totpoints);
	// UtilityFunctions::print("computing cells");
	vh->compute_cells();
	//UtilityFunctions::print("computed cells");

	//UtilityFunctions::print(vh->get_cells());

	//cells.clear();
	//cells.append_array(vh.get_cells());
	set_cells(vh->get_cells());
	//memdelete(vh);

	// compute voronoi cells
	/*Compute directly...*/
	// vc->compute_cell()
	//container_compute_cells(voro_container, voro_particle_order, voro_cells);

	//make a temporary csg mesh (or a mesh) from each cell data, also pass UVs accordingly, normals... (on the csg'ed mesh)
	// 1. make mesh from voro++ data
	// 2. generate csg mesh from cell mesh
	// 3. generate csg mesh from original geometry (how to handle UVs)
	// 4. perform csg operation (intersection) of each cell mesh with original geometry OR make union of cell meshs first and then intersect
	// convert to mesh, and deal with UV data accordingly

	// create a mesh resource per (csg) mesh and store mesh data this way

	// create meshinstances per shard and pass correctly transformed mesh resources to them...
}



