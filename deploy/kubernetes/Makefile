#<<<SAPPER SECTION BEGIN REPLACE MAKEFILE-DEPLOY>>>
deploy: build
	kubectl apply -f k8s/deployment.yaml
	kubectl apply -f k8s/service.yaml
	kubectl rollout restart deployment
#<<<SAPPER SECTION END REPLACE MAKEFILE-DEPLOY>>>
	
#<<<SAPPER SECTION BEGIN APPEND MAKEFILE-MISC>>>

cleanup-deployment:
	kubectl delete service <<<NAME>>>
	kubectl delete deployment <<<NAME>>>-deployment
#<<<SAPPER SECTION END APPEND MAKEFILE-MISC>>>