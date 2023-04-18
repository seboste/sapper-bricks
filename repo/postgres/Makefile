#<<<SAPPER SECTION BEGIN REPLACE MAKEFILE-RUN-ENV>>>
run-env: dbstart
#<<<SAPPER SECTION END REPLACE MAKEFILE-RUN-ENV>>>

#<<<SAPPER SECTION BEGIN APPEND MAKEFILE-MISC>>>
dbstart:
	$(eval ENTITY_CONTAINER_LINE_COUNT_WITH_HEADER := $(shell docker ps -a -f name=entity-postgres | wc -l))
	 @if [ "$(ENTITY_CONTAINER_LINE_COUNT_WITH_HEADER)" != "2" ]; then														\
	 	docker run --name entity-postgres -v "$(PWD)/db":/db -e POSTGRES_PASSWORD=my_password -p 5432:5432 -d postgres;		\
		sleep 3;																											\
		docker exec -it entity-postgres psql <<<POSTGRES_CONNECTION_STRING>>> -c "\i db/schema.sql";						\
	else																													\
		docker start entity-postgres;																						\
	fi

dbstop:
	docker stop entity-postgres || true
#<<<SAPPER SECTION END APPEND MAKEFILE-MISC>>>

