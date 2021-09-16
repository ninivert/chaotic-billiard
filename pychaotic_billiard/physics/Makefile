.PHONY: build-physics

build-physics:
	python setup.py build_ext --build-lib ../

install-pybind-smart_holder:
	sudo pip uninstall pybind11
	git clone --branch smart_holder https://github.com/pybind/pybind11.git
	cd pybind11; \
	sudo python setup.py install
	rm pybind11 -r
