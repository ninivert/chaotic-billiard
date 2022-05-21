from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
	Pybind11Extension(
		'physics',
		['../../physics/src/collider.cpp', '../../physics/src/curve.cpp', '../../physics/src/globals.cpp', '../../physics/src/logger.cpp', 'pybind.cpp'],
		include_dirs=['../../physics/include']
	)
]

setup(
	ext_modules=ext_modules
)
