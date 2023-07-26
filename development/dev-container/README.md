# development-dev-container

The *development-dev-container* brick provides a Dockerfile for a container image that contains many required tools for working with sapper (e.g. sapper, conan, cmake, gcc, clang, gdb, ...) for the use with the [Visual Studio Code](https://code.visualstudio.com/) IDE. This allows you develop in an isolated environment.

## usage

Make sure to have [Docker](https://www.docker.com/) and the [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) VS Code extension installed.

Then add the brick the usual way: `sapper brick add development-dev-container` and open the service's folder in VS Code (e.g. by executing `code .` in the folder). VS Code will then ask if you want to open the folder in the container. Do so. 

This brick can also be used for bootstrapping sapper. Create a new empty folder and copy the `.devcontainer` directory from this brick into that folder. Then start VS Code and confirm that you want to open the folder in the container. Now you can use *sapper* right away without installing it on your machine.

## next steps

*-*