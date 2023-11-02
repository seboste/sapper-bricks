#<<<SAPPER SECTION BEGIN REPLACE MAKEFILE-RUN-ENV>>>
run-env: dbstart
#<<<SAPPER SECTION END REPLACE MAKEFILE-RUN-ENV>>>

#<<<SAPPER SECTION BEGIN APPEND MAKEFILE-MISC>>>
dbstart:
	$(eval ENTITY_CONTAINER_LINE_COUNT_WITH_HEADER := $(shell docker ps -a -f name=entity-redis | wc -l))									
	@if [ "$(ENTITY_CONTAINER_LINE_COUNT_WITH_HEADER)" != "2" ]; then																		\
	 	docker run -d --name entity-redis -p 6379:6379 -p 8001:8001 -e REDIS_ARGS="--requirepass my_password" redis/redis-stack:7.2.0-v6; 	\
	else																																	\
		docker start entity-redis;																											\
	fi																																		
	sleep 3
	
dbstop:
	docker stop entity-redis || true
#<<<SAPPER SECTION END APPEND MAKEFILE-MISC>>>

