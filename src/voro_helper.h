#ifndef VORO_HELPER_H
#define VORO_HELPER_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource.hpp>

#include "../voro/src/voro++.hh"

using namespace voro;

namespace godot {

class VoroContainer: public Resource {
    GDCLASS(VoroContainer, Resource)

    private:
        Vector3 min;
        Vector3 max;
        int num_cells;
        container* handle; 

    protected:
	    static void _bind_methods();

    public:
        VoroContainer(const VoroContainer& con);
        VoroContainer();
	    ~VoroContainer();

        void set_num_cells(int p_num_cells);
        int get_num_cells();

        void set_min(Vector3 p_min);
        Vector3 get_min();

        void set_max(Vector3 p_max);
        Vector3 get_max();
};

class VoroParticleOrder: public Resource {
    GDCLASS(VoroParticleOrder, Resource)

    private:
         particle_order* handle;

    protected:
	    static void _bind_methods();

    public:
        VoroParticleOrder();
	    ~VoroParticleOrder();
};

class VoroCell : public Resource {
    GDCLASS(VoroCell, Resource)

    protected:
	    static void _bind_methods();

    public:
        TypedArray<Vector3> verts;
        TypedArray<int> poly_totvert;
        TypedArray<TypedArray<int>> poly_indices;
        TypedArray<int> neighbors;

        Vector3 centroid;
        double volume;

        int index;
        int totvert;
        int totpoly;
        VoroCell();
        ~VoroCell();
};

// for Fracture 3D
class VoroHelper : public Resource {
    GDCLASS(VoroHelper, Resource)

    protected:
	    static void _bind_methods();

    public:

        TypedArray<VoroCell> cells;
        VoroContainer container;
        VoroParticleOrder particle_order;

	    VoroHelper();
	    ~VoroHelper();
        void compute_cells();
        void put(int n, double x, double y, double z);

        void set_num_cells(int p_num_cells);
        int get_num_cells();

        void set_min(Vector3 p_min);
        Vector3 get_min();

        void set_max(Vector3 p_max);
        Vector3 get_max();
};

}

#endif