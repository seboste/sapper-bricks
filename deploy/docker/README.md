# deploy-docker

This brick provides a simple *Dockerfile* for your service. After building the service, the *sapper service build* command also creates the container image and copies the service's binary into it.

## usage

Please make sure that you have [docker](https://www.docker.com/) installed on your system and the docker engine is up and running. Alternatives, such as e.g. [podman](https://podman.io/) should also be fine.

Then, the following command can be used to build the container image:
```bash
sapper service build .
```
To run the service, use the following command:
```bash
sapper service run .
```
The correct port should already be forwarded to the host system, so that your service would be available on `locàlhost:PORT`. That's it.

## parameters

- **DOCKER_BASE_IMAGE**: The base image of the container image. default: `ubuntu:latest`.

- **PORT**: The port to be exposed. This has typically already been defined by the handler brick in case it had been added before.

## next steps

The next step is typically deploying the container. You can push it to a remote container registry (e.g. the one provided by your cloud provider) using the `docker push` command, or you can directly use it in a container orchestration framework such as [kubernetes](https://kubernetes.io/). Consider adding the *deploy-kubernetes* brick for that.

## open issues

*-*
 