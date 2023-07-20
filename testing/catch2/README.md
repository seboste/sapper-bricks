# testing-catch2

This brick adds unit testing support to your microservice. It is based on the [catch2](https://github.com/catchorg/Catch2) unit testing framework.

Besides setting up *catch2* in the *conanfile.txt* and *CMakeLists.txt*, it also adds some basic unit tests for the example *core* target so that you can jump right into writing your own tests. All tests will be located in a separate `tests` folder parallel to the `src` folder. Its substructure shall match the `src` folder substructure.

## usage

Building the tests can be turned on and off using CMake's `BUILD_TESTING` variable. The tests can be run using the following command:

```bash
sapper service test .
```
The test results will be written to 'build/tests/test_results.xml' in the [Junit](https://en.wikipedia.org/wiki/JUnit) xml format, that can be interpreted by many CI tools.

## parameters

*-*

## next steps

Implement your own tests for your core and your adapters. The hexagonal architecture of your services allows you to use test doubles for testing. 

Please consider running the tests in your CI pipeline and publish the test report.

## open issues

*-*
 