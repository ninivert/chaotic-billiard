import json
import numpy as np
import matplotlib.pyplot as plt
from physics import Segment, Arc, Ball, vec2, World

world = World()
angle0 = 0.02
delta0 = 0.001

world.add_curve(Segment(vec2(50, 50), vec2(450, 50)))
world.add_curve(Segment(vec2(450, 50), vec2(450, 450)))
world.add_curve(Segment(vec2(450, 450), vec2(50, 450)))
world.add_curve(Segment(vec2(50, 450), vec2(50, 50)))
world.add_curve(Arc(vec2(250, 250), 50, 0, 2*np.pi))
world.add_ball(Ball(vec2(350, 250), vec2(np.cos(angle0-delta0/2), np.sin(angle0-delta0/2))))
world.add_ball(Ball(vec2(350, 250), vec2(np.cos(angle0+delta0/2), np.sin(angle0+delta0/2))))

print('>>> running simulation')
nsteps = 100_000
nballs = len(world.balls)
pos = np.empty((nsteps, nballs, 2))
for i in range(nsteps):
	world.step(0.2)
	for j in range(nballs):
		pos[i, j, 0] = world.get_ball(j).pos.x
		pos[i, j, 1] = world.get_ball(j).pos.y

print('>>> plotting')
fig, ax = plt.subplots(tight_layout=True)
ax.set_aspect('equal')
ax.xaxis.set_major_locator(plt.NullLocator())
ax.xaxis.set_minor_locator(plt.NullLocator())
ax.yaxis.set_major_locator(plt.NullLocator())
ax.yaxis.set_minor_locator(plt.NullLocator())
ax.spines.bottom.set_visible(False)
ax.spines.top.set_visible(False)
ax.spines.left.set_visible(False)
ax.spines.right.set_visible(False)
ax.set_xlabel('$x$ position')
ax.set_ylabel('$y$ position')
for j in range(pos.shape[1]):
	ax.plot(pos[:, j, 0].T, pos[:, j, 1].T, linewidth=1)
fig.savefig('../images/lyapunov_xy.png')

fig, ax = plt.subplots(tight_layout=True)
ax.plot(np.linalg.norm(pos[:20_000, 0, :] - pos[:20_000, 1, :], axis=-1), linewidth=1)
ax.set_yscale('log')
ax.set_xlabel('iteration number (time)')
ax.set_ylabel('$\\delta(t) = |\\vec{p_1}(t) - \\vec{p_2}(t)|$')
ax.spines.right.set_visible(False)
ax.spines.top.set_visible(False)
fig.savefig('../images/lyapunov_delta.png')

plt.show()
