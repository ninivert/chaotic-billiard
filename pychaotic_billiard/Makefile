.PHONY: build install-pybind-smart_holder

build:
	cd physics; \
	python setup.py build_ext --inplace; \
	mv physics*.so ..

install-pybind-smart_holder:
	sudo pip uninstall pybind11
	git clone --branch smart_holder https://github.com/pybind/pybind11.git
	cd pybind11; \
	sudo python setup.py install; \
	cd .. && sudo rm -rf pybind11