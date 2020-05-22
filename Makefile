BUILD="_build"
GENERATOR="Unix Makefiles"

cmake:
	cmake -G${GENERATOR} -B${BUILD} -DCMAKE_BUILD_TYPE=Release .
	ln -s ${BUILD}/compile_commands.json

cmake_debug:
	cmake -G${GENERATOR} -B${BUILD} -DCMAKE_BUILD_TYPE=Debug .
	ln -s ${BUILD}/compile_commands.json

build:
	cmake --build ${BUILD}

clean:
	rm -rf ${BUILD}
	rm compile_commands.json
