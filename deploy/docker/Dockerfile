FROM <<<DOCKER_BASE_IMAGE>>>
EXPOSE <<<PORT>>>

# install certificates so that SSL can be used
RUN apt-get update
RUN apt-get install ca-certificates -y
RUN update-ca-certificates

# include the service
COPY build/src/app/<<<NAME>>> app/<<<NAME>>>
CMD ["./app/<<<NAME>>>"]