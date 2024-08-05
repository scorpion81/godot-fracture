#ifndef FRACTURE_H
#define FRACTURE_H

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <voro_helper.h>

namespace godot {

class Fracture3D : public MeshInstance3D {
	GDCLASS(Fracture3D, MeshInstance3D)

private:
    int shard_count;
	int shard_seed;

protected:
	static void _bind_methods();

public:
	Fracture3D();
	~Fracture3D();

	void _process(double delta) override;

	void set_shard_count(const int p_shard_count);
	int get_shard_count() const;

	void set_shard_seed(const int p_shard_seed);
	int get_shard_seed() const;

	void execute_fracture() const;
};

}

#endif