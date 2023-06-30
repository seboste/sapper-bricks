#<<<SAPPER SECTION BEGIN MAKEFILE-BUILD>>>
build:
	mkdir -p build	
	conan install . -if build -s build_type=Release --build=missing
	cmake -Bbuild -S. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake	
	cmake --build build
#<<<SAPPER SECTION END MAKEFILE-BUILD>>>

#<<<SAPPER SECTION BEGIN MAKEFILE-TEST>>>
test: build
#<<<SAPPER SECTION END MAKEFILE-TEST>>>

#<<<SAPPER SECTION BEGIN MAKEFILE-RUN-ENV>>>
run-env:
#<<<SAPPER SECTION END MAKEFILE-RUN-ENV>>>

run: build run-env
#<<<SAPPER SECTION BEGIN MAKEFILE-RUN>>>
	build/src/app/<<<NAME>>>
#<<<SAPPER SECTION END MAKEFILE-RUN>>>

stop:
#<<<SAPPER SECTION BEGIN MAKEFILE-STOP>>>
	ps -e
	ps -e | grep <<<NAME>>>
	ps -e | grep <<<NAME>>> | awk '{print $$1}'
	ps -e | grep <<<NAME>>> | awk '{print $$1}' | xargs kill -15
#<<<SAPPER SECTION END MAKEFILE-STOP>>>

#<<<SAPPER SECTION BEGIN MAKEFILE-DEPLOY>>>
deploy: build
#<<<SAPPER SECTION END MAKEFILE-DEPLOY>>>

#<<<SAPPER SECTION BEGIN MAKEFILE-MISC>>>
#<<<SAPPER SECTION END MAKEFILE-MISC>>>
