#ifndef VORO_HELPER_H
#define VORO_HELPER_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <voro/src/voro++.hh>

using namespace voro;

namespace godot {

class VoroCell : public Resource {
    GDCLASS(VoroCell, Resource)

    private:
        TypedArray<Vector3> verts;
        TypedArray<int> poly_num_verts;
        TypedArray<int> poly_indices; // section lengths are in poly_totvert
        TypedArray<int> neighbors;

        Vector3 centroid;
        double volume;

        int index;
        int num_verts;
        int num_polys;

    protected:
	    static void _bind_methods();

    public:
    
        VoroCell();
        ~VoroCell();
        
        //those are bound to readonly properties later
        TypedArray<Vector3> get_verts() { return verts; };
        TypedArray<int> get_poly_num_verts() { return poly_num_verts; };
        TypedArray<int> get_poly_indices() { return poly_indices; };
        TypedArray<int> get_neighbors() { return neighbors; };
        Vector3 get_centroid() { return centroid; };
        double get_volume() { return volume; }
        int get_index() { return index; };
        int get_num_verts() {return num_verts; };
        int get_num_polys() {return num_polys; };

        void set_verts(TypedArray<Vector3> p_verts) { verts = p_verts;  };
        void set_poly_num_verts(TypedArray<int> p_poly_num_verts) { poly_num_verts = p_poly_num_verts; };
        void set_poly_indices(TypedArray<int> p_poly_indices) { poly_indices = p_poly_indices; };
        void set_neighbors(TypedArray<int> p_neighbors) { neighbors = p_neighbors; };
        void set_centroid(Vector3 p_centroid) { centroid = p_centroid; };
        void set_volume(double p_volume) { volume = p_volume; };
        void set_index(int p_index) { index = p_index; };
        void set_num_verts(int p_num_verts) { num_verts = p_num_verts; };
        void set_num_polys(int p_num_polys) { num_polys = p_num_polys; };
};

// for Fracture 3D
class VoroHelper : public Resource {
    GDCLASS(VoroHelper, Resource)

    private:
        TypedArray<VoroCell> cells;
        voro::container* container = nullptr;
        voro::particle_order* particle_order = nullptr;
        Vector3 min;
        Vector3 max;
        int num_cells;


    protected:
	    static void _bind_methods();

    public:
        
	    VoroHelper();
	    ~VoroHelper();
        void compute_cells();
        void new_container();
        void new_particle_order();
        void put(int n, double x, double y, double z);

        void set_num_cells(int p_num_cells) { num_cells = p_num_cells; };
        int get_num_cells() { return num_cells; };

        void set_min(Vector3 p_min) { min = p_min; };
        Vector3 get_min() { return min; }

        void set_max(Vector3 p_max) { max = p_max; };
        Vector3 get_max() { return max; }

        TypedArray<VoroCell> get_cells() { return cells;}
        void set_cells(TypedArray<VoroCell> p_cells) { cells = p_cells; };
};

}

#endif