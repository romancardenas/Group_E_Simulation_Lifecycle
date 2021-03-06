# Group E - A solution for simulation lifecycle management

## Table of Contents
* [Team Members](#team-members)
* [Application Overview](#application-overview)
* [File Organization](#file-organization)
* [Getting Started](#getting-started)
    * [1. Dependencies](#1-dependencies)
    * [2. Building the Software](#2-building-the-software)
    * [3. Testing the Software](#3-testing-the-software)
    * [4. Running the Main Application](#4-running-the-main-application)
* [Documentation](#documentation)

## Team Members
**Organization:** Carleton University

**Course:** SYSC 5709 - Software Development with C

**Authors:**

| Student                  | GitHub ID      |
|--------------------------|----------------|
| Françoise Blanc          | francoiseblanc |
| Román Cárdenas Rodríguez | romancardenas  |
| Ghida Dandan             | ghidadandan    |
| Aidan Fahlman            | AidanFahlman   |
| Bruno St-Aubin           | staubibr       |

## Application Overview

This application is meant to support its users through the entire simulation lifecycle. To achieve this, it allows users to:
* submit a workflow file containing a series of operations to build a simulation scenario from geospatial data;
* construct a spatial DEVS simulation model;
* run a simulation using the Cadmium simulator developed at the Advanced Real-Time Simulation (ARS) Laboratory;
* prepare a visualization of the simulation results; and
* visualize the simulation results in the DEVS WebViewer, also developed at the ARS Lab.

Detailed information about the application can be found [here](https://github.com/romancardenas/Group_E_Simulation_Lifecycle/wiki).

## File Organization
The application file organization is as follows:

| Main Folder Structure | Description                                                                                                                                             |
|-----------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------|
| `.github/`            | Configuration files for GitHub actions on `devel` and `main` branches.                                                                                  |
| `bin/`                | Executables for the application and the tests.                                                                                                          |
| `build/`              | Compiler's build files.                                                                                                                                 |
| `data/`               | Contains all required data to run the application. It includes samples of workflow files, spatial data, and any results generated during the execution. |
| `include/`            | Application's header files.                                                                                                                             |
| `logs/`               | Contains the results files generated by Cadmium.                                                                                                        |
| `src/`                | Function sources.                                                                                                                                       |
| `test/`               | Test sources and required data for testing.                                                                                                             |
| `third_party/`        | Contains third-party libraries and software. Includes Cadmium, Cell-DEVS models,  cJSON and Unity libraries.                                            |
| `.dockerignore`       | Specifies which files should be ignored by Docker when creating a Docker image of the application.                                                      |
| `.gitignore`          | Specifies which files should be ignored by git.                                                                                                         |
| `.gitmodules`         | Specifies third-party git repositories that are used by this application.                                                                               |
| `action.yml`          | Needed by GitHub for creating a GitHub action from a docker image based on the `Dockerfile` of this repository.                                         |
| `build.sh`            | Shell script for building the application. You must execute `setup.sh` before.                                                                          |
| `CMakeLists.txt`      | CMake lists file. It specifies how to create the `Makefile` of the application using the `cmake` tool.                                                  |
| `Dockerfile`          | File that describes how to create a Docker image for testing the application. This image is used by GitHub actions.                                     |
| `LICENSE`             | License of the application.                                                                                                                             |
| `README.md`           | Project's ReadMe file.                                                                                                                                  |
| `run_tests.sh`        | Shell script to run all tests.                                                                                                                          |
| `setup.sh`            | Shell script to download third-party repositories and build the software. If called with the flag `-b`, this shell script will execute `build.sh`.      |
| `setup_docker.sh`     | Special shell script used by setting up the Docker image for testing the software. It is not meant to be used by users.                                 |

## Getting Started
### 1. Dependencies
We rely on third-party software to run our tool. The main dependencies for each platform are as follows:

**Windows Users** 
* Cygwin 
* GNU C Compiler, Git, Make, and CMake 
* apt-cyg Package Manager 
* Boost Library

**MacOS Users** 
* Command Line Tools 
* Homebrew 
* Boost Library 

**Linux Users**
* GNU C Compiler, Git, Make, and CMake 
* Boost Library 

Detailed instructions about the installation of these dependencies are provided [here](https://github.com/romancardenas/Group_E_Simulation_Lifecycle/wiki/1.-Software-Dependencies).

### 2. Building the Software
Once you have installed all the dependencies, building the application should be straightforward, as we provide a set of shell scripts for automating the process.

1. Open a terminal (Windows users should use a Cygwin terminal)
2. Download this repository in your home directory and move inside the repository:
```
cd
git clone https://github.com/romancardenas/Group_E_Simulation_Lifecycle.git
cd Group_E_Simulation_Lifecycle
```
3. Run the `setup.sh` shell script to download third-party repositories and build the software. Use the `-b` flag to make a complete build:
```
./setup.sh -b
```

Once the process is finished, you should be able to see:
1. a `/build` directory with the `Makefile` generated by CMake
2. a `/bin` directory with all the binaries (main function and tests) generated.

### 3. Testing the Software
We use [GitHub Actions](https://github.com/romancardenas/Group_E_Simulation_Lifecycle/actions), a tool for Continuous Integration (CI), to ensure that the `devel` and `main` branches pass all the tests. You can check there whether the code is stable or not. However, after building everything on your machine, you may want to run some tests to make sure that everything works as expected. You have two options:
1. Use `make` to run the tests:
```
cd build
make test
```
2. Run the `run_tests.sh` shell script:
```
./run_tests.sh
```
If you want to store the test results in a file (e.g., the `test_results.txt` file), you can type the following command:
```
./run_tests.sh > test_results.txt
```
### 4. Running the Main Application

To run the application, type the following commands

```
cd bin
./sim_lifecycle RELATIVE_PATH_TO_WORKFLOW_JSON_FILE
```

Where `RELATIVE_PATH_TO_WORKFLOW_JSON_FILE` corresponds to the relative path from the `bin/` folder to the JSON file containing all the simulation workflow fields.

You **must** run the main application from the `bin/` folder. All the file paths included in your workflow JSON file must be relative to this folder. Otherwise, the application won't be able to find the desired files.

We provide a set of valid workflow JSON files in the `data/` folder. For example, you can run a simple model as follows:

```
cd bin
./sim_lifecycle ../data/demo_1/demo.json
```

The simulation output of this workflow example will be stored in the `data/demo_1/output` folder.

You can run more complex scenario as follows:

```
cd bin
./sim_lifecycle ../data/demo_2/provinces_workflow.json
```

The simulation output of this workflow example will be stored in the `data/demo_2/output` folder.

## Documentation
All documentation regarding this application can be found in the project's GitHub repository.
Please refer to the [user documentation](https://github.com/romancardenas/Group_E_Simulation_Lifecycle/wiki/3.-User-Documentation:-How-to-Use) and [developer documentation](https://github.com/romancardenas/Group_E_Simulation_Lifecycle/wiki/4.-Developer-Documentation).
