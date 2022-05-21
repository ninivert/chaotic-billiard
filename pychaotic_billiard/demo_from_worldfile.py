import json
from physics import World, Segment, Arc, BezierCubic, Ball, vec2
from typing import Union

def from_dict(j: dict) -> Union[Segment, Arc, BezierCubic, Ball, vec2]:
	if j["class"] == "vec2":
		return vec2(j["parameters"]["x"], j["parameters"]["y"])
	elif j["class"] == "Ball":
		return Ball(from_dict(j["parameters"]["pos"]), from_dict(j["parameters"]["vel"]))
	elif j["class"] == "Segment":
		return Segment(from_dict(j["parameters"]["p1"]), from_dict(j["parameters"]["p2"]))
	elif j["class"] == "Arc":
		return Arc(from_dict(j["parameters"]["p0"]), j["parameters"]["r"], j["parameters"]["theta_min"], j["parameters"]["theta_max"])
	elif j["class"] == "BezierCubic":
		return BezierCubic(from_dict(j["parameters"]["p0"]), from_dict(j["parameters"]["p1"]), from_dict(j["parameters"]["p2"]), from_dict(j["parameters"]["p3"]))
	else:
		raise RuntimeError(f'unkown class {j["class"]}')

def world_from_dict(j: dict) -> World:
	world = World()

	for j_ball in j['balls']:
		world.add_ball(from_dict(j_ball))
	
	for j_curve in j['curves']:
		world.add_curve(from_dict(j_curve))

	return world

if __name__ == '__main__':
	print('>>> loading world file from json')
	with open('../worldfiles/world_circle3.json') as file:
		world_json = json.load(file)

	world = world_from_dict(world_json)
	print(world)

	print('>>> re-serializing and asserting equality')
	assert world_json == json.loads(world.json())
	print('OK')