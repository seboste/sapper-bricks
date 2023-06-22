# How to contribute to Sapper Bricks

Thank you for your interest in contributing to Sapper Bricks! Sapper can only be as good as its underlying brick library. Your contribution is a valuable step towards facilitating the creation of C++ microservices.

## Adding new bricks

Whether you are the vendor or a user of some piece of C++ code/pattern/library that can be used in cloud-native microservices, please consider contributing a brick as a PR or file an issue. 

### Repository structure

Each brick is located in its own folder in the two level folder hierarchy consisting of the category in the first level (e.g. handler) and the specific technology (e.g. grpc) on the second level. 

Except of the two special files *manifest.yaml* and *README.md*, the files/folder-structure in that folder all belong to the brick and will be copied/merged to the target microservice (See merging files below for details).

### manifest.yaml

The *manifest.yaml* file contains basic information about the brick such as the *id*, *version*, *kind*, *description*, *dependencies* to other bricks, and *parameters* in a yaml-format.

The *id* of a brick must be unique and should be composed of the category and specific technology (e.g. handler-grpc for a request handler that uses grpc).

The *version* should follow semantic versioning. Breaking changes shall be avoided if possible.

> **_WARNING:_**  By now, the version is not exploited by sapper.

The *kind* of a brick describes whether it is an *extension* of an existing microservice or a *template* for creating a new microservice.

The *description* briefly describes the purpose of the brick.

The *dependencies* contain a list of brick *id*s that will automatically be added when this brick is added to a microservice. The dependencies must not be cyclic.

The *parameters* are used to customize the brick's files while merging them into the target microservice. The user specifies the parameter's value in a CLI prompt or as a CLI parameter while adding the brick to the target microservice if it hasn't been defined before for the service. Parameters may have a *default* value. A parameter's *name* must be unique across all bricks, so that it can be reused if it has the same semantics. For example, the parameter 'NAME' for the name of the microservice is initially defined by the template 'hexagonal-skeleton' but can be reused by other bricks that need to use the microservice name as well. 

### README.md

The *README.md* of a brick should contain information about the microservice including a description of the brick, a description of all parameters, how to use the brick, and possibly next steps after adding it.

### Merging files

When adding a brick to a microservice, all files in the folder are recursively copied to the target microservice.

During that process, all occurrences of `<<<PARAMETER-NAME>>>` in the files are replaced with the value that has been assigned to the parameter with the name 'PARAMETER-NAME'.

In case a file already exists in the target folder structure, those files are merged based on *sapper sections* that can be defined in each file using the following two lines:
```c++
...some code...
//<<<SAPPER SECTION BEGIN 'section-name'>>>
...some code inside a section...
//<<<SAPPER SECTION END 'section-name'>>>
...some code...
```
The code inside a section will be merged with the corresponding section (same 'section-name') in the brick's corresponding file:
```c++
//<<<SAPPER SECTION BEGIN 'merge-strategy' 'section-name'>>>
...some brick code to be merged...
//<<<SAPPER SECTION END 'merge-strategy' 'section-name'>>>
```
where the 'merge-strategy' can be one of the following:
- **REPLACE**: target will entirely be replaced by source.
- **PREPEND**: all source lines will appear before the target lines.
- **APPEND**: all source lines will appear after the target lines.
- **MERGE**: only source lines that do not appear in the target are added to the section. Used for e.g. includes.

## Bug fixes

Please submit a PR. If you're not able to do so, please file an issue.

## Small Enhancements

Please submit a PR.

## Quality control
Azure Pipelines is used for building each brick for each PR. However, not all possible combinations of all bricks will be built for complexity reasons. For many combinations, this is not a problem as the hexagonal architecture results in loose coupling of the different bricks.
