extends Fracture3D

# Called when the node enters the scene tree for the first time.
func _ready():
	execute_fracture()
	print("hello")
	#print(cells)
	#print(cells[0] as VoroCell)
	#var vc: VoroCell = cells[0]
	#print(vc.centroid)
	for i in range(cells.size()):
		mesh = to_mesh(i)
		#print(mesh)
		var mi: MeshInstance3D = MeshInstance3D.new()
		mi.position = cells[i].centroid * 0.1
		mi.mesh = mesh
		add_child(mi)
		

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
	
func to_mesh(index: int):
	var st:SurfaceTool = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)
	
	var c:VoroCell = cells[index]
	for i in range(c.num_verts):
		st.add_vertex(c.verts[i])
		#st.set_normal(c.verts[i].normalized())
	
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
			
			st.add_index(vind[0])
			st.add_index(vind[1])
			st.add_index(vind[2])
		k += n
		
	#st.generate_normals()
	#st.generate_tangents()
	var me: ArrayMesh = st.commit()
	#ResourceSaver.save(me, "res://cell" + str(index) + ".tres", ResourceSaver.FLAG_COMPRESS)
	return me
