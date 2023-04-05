build:
#<<<SAPPER SECTION BEGIN MAKEFILE-BUILD>>>
	mkdir -p build	
	conan install . -if build -s build_type=Release
	cmake -Bbuild -S. -DCMAKE_BUILD_TYPE=Release
	cmake --build build
#<<<SAPPER SECTION END MAKEFILE-BUILD>>>
test: build
#<<<SAPPER SECTION BEGIN MAKEFILE-TEST>>>
#<<<SAPPER SECTION END MAKEFILE-TEST>>>

deploy: build
#<<<SAPPER SECTION BEGIN MAKEFILE-DEPLOY>>>
#<<<SAPPER SECTION END MAKEFILE-DEPLOY>>>
