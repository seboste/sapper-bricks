#<<<SAPPER SECTION BEGIN APPEND MAKEFILE-BUILD>>>
	docker build . -t <<<NAME>>>
#<<<SAPPER SECTION END APPEND MAKEFILE-BUILD>>>

#<<<SAPPER SECTION BEGIN REPLACE MAKEFILE-RUN>>>	
	docker run -p <<<PORT>>>:<<<PORT>>> <<<NAME>>>

# add the following parameter to enable the container to access the host network, e.g. to connect to a locally running db such as postgres://postgres:my_password@host.docker.internal:5432
# --add-host=host.docker.internal:host-gateway

#<<<SAPPER SECTION END REPLACE MAKEFILE-RUN>>>
