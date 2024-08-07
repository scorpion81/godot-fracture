extends Fracture3D


# Called when the node enters the scene tree for the first time.
func _ready():
	execute_fracture()
	print("hello")
	print(cells)
	print(cells[0] as VoroCell)
	var vc: VoroCell = cells[0]
	print(vc.centroid)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
	
func to_mesh():
	pass
