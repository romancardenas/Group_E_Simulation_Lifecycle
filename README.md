# Group E: A solution for simulation lifecycle management

Françoise Blanc,
Román Cárdenas Rodríguez,
Ghida Dandan,
Aidan Fahlman,
Bruno St-Aubin

## Problem Statement and Software Purpose

In many domains of application, Modeling and Simulation (M&S) has shown to be useful to study real world systems and to support decision-making through models that abstract the systems under study. M&S requires fewer resources and involves less risk than studying the system itself. It is a way to evaluate new or upgraded systems without compromising limited resources, interrupting operations, compromising safety, etc. However, building accurate models that adequately represent real-world systems is a difficult and time-consuming task that requires extensive domain knowledge and a deep understanding of the simulation method used. Because of the significant effort involve in understanding a specific domain and building the required models, the tendency in the field is to build single-use simulators.

Generic simulation methods, such as the Discrete Event System Specification (DEVS), are well-positioned to break down the disciplinary silos. DEVS supports hierarchical and modular model development, it favors reusability for models. It is based on a rigorous formalism to manage inputs and outputs of atomic models, the basic block of the specification. Atomic models can be integrated into larger, more complex coupled models. Regardless, because of its lack of approachability, the DEVS specification sees only limited usage in the industry and outside the research community. Compounding the issue is the fact that DEVS approachability rarely is the subject of research by the community. Thus, the available development environment, particularly for non-expert users, does not adequately support modeling, experiment management, or building DEVS based applications.

Over the past decade of conferences in the field of modeling and simulation, panels of eminent researchers have identified several grand challenges that should guide research by the community. Notably, two of these challenges are related to the approachability of the discipline by non-expert users:

- Fujimoto identifies the need to lessen the burden on the modelers by reducing the efforts required to develop models [1]. It should be possible to compose existing models of subsystems into an accurate model of a wider system.

- Taylor et al. identify the democratization of M&S as a challenge to be addressed [2]. Researchers now require adequate tools that will support them in building online platforms, mobile applications, developing customized robot behaviors, planning the use of space, etc. These tools empower researchers in fields other than M&S to collaborate across multiple application domains.

In this project, we propose to implement a workflow-based approach meant to support the complete simulation lifecycle. Through this system, users will be able to submit geospatial data, convert them into spatial DEVS simulation models, and execute the simulation using the Cadmium simulator developed at the Advanced Real-Time Simulation Laboratory. Simulation results will then be optimized for web visualization in the DEVS WebViewer developed in the same laboratory.

The purpose of this software is to address the two challenges previously identified. Automated generation of large-scale spatial simulation models from GIS data seeks to lessen the burden of modeling for its users. The integration of several already existing applications (Model generator, Cadmium & DEVS WebViewer) through this software aims at democratizing the simulation lifecycle for non-expert users.

<b>[1]</b> <i>R. M. Fujimoto, "Research challenges in parallel and distributed simulation," ACM Transactions on Modeling and Computer Simulation (TOMACS), vol. 26, no. 4, p. 29, 2016.</i>

<b>[2]</b> <i>S. J. Taylor, A. Khan, L. K. Morse, A. Tolk, L. Yilmaz, J. Zander, and P. J. Mosterman, "Grand challenges for modeling and simulation: simulation everywhere — from cyberinfrastructure to clouds to citizens," Simulation: Transactions of the Society for Modeling and Simulation International, vol. 91, no. 7, pp. 648-665, 2015.</i>

## Software Requirements

1. Users must be able to provide geospatial data as a source for the model generation workflow:
	1. All geometric primitives (point, line and polygon)
	2. Spatial network data  
	3. In all common geospatial data formats (shapefile, geopackages, csv, geojson, etc.)
2. Users must be able to provide attribute data in common formats (csv, excel, json) for the model generation process.
3. Users must be able to provide a workflow file that contains the steps for automated model generation
4. The system must automatically execute the workflow to convert geospatial data into a Cadmium model configuration file.
	1. Map geospatial features onto simulation models from an existing library of parametric models
	2. Establish neighborhoods or couplings through spatial analysis of the geospatial data
	3. The file output must be a json file properly formatted according to Roman’s model generation process
5. The system must support common spatial analysis functions to establish neighborhood and parameters:
	1. Topological relationships (intersection, contains, within, touches, etc.)
	2. Network distance analysis
	3. Volume analysis
	4. Spatial buffers
	5. Voronoi diagrams
	6. Geospatial statistics
6. The system must support common SQL like function to determine model parameters
7. The system must automatically convert the model configuration into a Cadmium model using an existing configuration generator.
8. The system must compile the simulator for the model, execute the simulation and return the results.
9. The system must convert the Cadmium results into a common specification for web based visualization through existing conversion web service
10. The system must store the converted results into a remote database through existing web services
11. The users must be able to configure a visualization through existing web service and retrieve a URL to access the remotely hosted, existing web visualization platform.

## Software Releases
The software will be released in 3 versions.

The purpose of the first release is… This first release will include the following requirements:

The purpose of the second release is… This second release will include the following requirements:

The purpose of the third release is… This second release will include the following requirements:
