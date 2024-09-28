@tool
extends Fracture3D

func _add_inspector_buttons() -> Array:
	var buttons = []
	#buttons.push_back({
	#	"name": "Test button",
	#	"icon": preload("res://icon.svg"),
	#	"pressed": _on_test_button_pressed
	#})
	buttons.push_back({
		"name": "Fracture",
		"pressed": _fracture
	})
	#buttons.push_back({
	#	"name": "Other button with lambda",
	#	"pressed": func(): print('Lambda callback')
	#})
	return buttons
	
func _ready():
	pass

# Called when the node enters the scene tree for the first time.
func _fracture():
	execute_fracture()
	print("hello")
	#print(cells)
	#print(cells[0] as VoroCell)
	#var vc: VoroCell = cells[0]
	#print(vc.centroid)
	
	#do this in editor ?!
	#var csg:CSGCombiner3D = CSGCombiner3D.new()
	#get_node("..").add_child(csg)
	#csg.operation = CSGShape3D.OPERATION_UNION
	var csg:CSGCombiner3D = $"../CSG"
	for c in csg.get_children():
		c.queue_free()
		
	#csg.operation = CSGShape3D.OPERATION_UNION
	for i in range(cells.size()):
		var me:ArrayMesh = to_mesh(i)
	#	#print(mesh)
		var mi: CSGMesh3D = CSGMesh3D.new()
		mi.position = cells[i].centroid * 0.01
	#	#perform csg operation here
		mi.mesh = me
		mi.operation = CSGShape3D.OPERATION_UNION
		csg.add_child(mi)
	#	ResourceSaver.save(mi.mesh, "res://cell" + str(i) + ".tres", ResourceSaver.FLAG_COMPRESS)
	
	var orig:CSGMesh3D = CSGMesh3D.new()
	orig.operation = CSGShape3D.OPERATION_INTERSECTION
	orig.mesh = mesh
	csg.add_child(orig)
	#make mesh..
	print(csg.get_meshes())
	var m: ArrayMesh = csg.get_meshes()[1]
	mesh = m
	print(mesh._surfaces[0].vertices)
	#mesh = orig.mesh
	#csg.queue_free()
	#ResourceSaver.save(mesh, "res://orig.tres", ResourceSaver.FLAG_COMPRESS)
		

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
	
func to_mesh(index: int):
	var st:SurfaceTool = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)
	
	var indexes: PackedInt32Array = PackedInt32Array()
	var vertices: PackedVector3Array = PackedVector3Array()
	var normals: PackedVector3Array = PackedVector3Array()
	
	var c:VoroCell = cells[index]
	vertices.append_array(c.verts)
	normals.resize(c.num_verts)
	
	#yuck, manual triangulation...
	#ngon to tris
	var k: int = 0
	#print(c.poly_indices)
	for i in range(0, c.num_polys):
		var n : int = c.poly_num_verts[i]
		var tris: int = n - 2
		for j in range(tris):
			var vind:Array[int] = []
			var ind:Array[int] = []
			#0 1 2  0 2 3  0 3 4  0 4 5
			ind = [k, k+j+1, k+j+2]
			vind = [c.poly_indices[ind[0]], c.poly_indices[ind[1]], c.poly_indices[ind[2]]]
			indexes.append_array(vind)
		k += n
	
	var vi: int = 0
	for v in vertices:
		#st.set_normal(v.normalized())
		st.set_smooth_group(-1)
		st.add_vertex(v)
		vi += 1	
		
	for ind in indexes:
		st.add_index(ind)
		
	st.generate_normals()
	#st.generate_tangents()
	var me: ArrayMesh = st.commit()
	#ResourceSaver.save(me, "res://cell" + str(index) + ".tres", ResourceSaver.FLAG_COMPRESS)
	return me
