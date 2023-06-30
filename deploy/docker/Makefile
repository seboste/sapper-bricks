#<<<SAPPER SECTION BEGIN APPEND MAKEFILE-BUILD>>>
	docker build . -t <<<NAME>>>
#<<<SAPPER SECTION END APPEND MAKEFILE-BUILD>>>

#<<<SAPPER SECTION BEGIN REPLACE MAKEFILE-RUN>>>	
	docker run -p <<<PORT>>>:<<<PORT>>> <<<NAME>>>

# add the following parameter to enable the container to access the host network, e.g. to connect to a locally running db such as postgres://postgres:my_password@host.docker.internal:5432
# --add-host=host.docker.internal:host-gateway

#<<<SAPPER SECTION END REPLACE MAKEFILE-RUN>>>

#<<<SAPPER SECTION BEGIN REPLACE MAKEFILE-STOP>>>
	docker ps | grep <<<NAME>>> | awk '{print $$1}' | xargs docker kill --signal="SIGTERM"
#<<<SAPPER SECTION END REPLACE MAKEFILE-STOP>>>
