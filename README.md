# Group E: A solution for simulation lifecycle management

 - Françoise Blanc
 - Román Cárdenas Rodríguez
 - Ghida Dandan
 - Aidan Fahlman
 - Bruno St-Aubin

## Problem Statement

In many application domains, modelling and simulation (M&S) has shown to be useful for studying real-world systems and supporting decision-making through models that abstract the systems under study. M&S requires fewer resources and involves less risk than studying the system itself. It is a way to evaluate new or upgraded systems without compromising limited resources, interrupting operations, and compromising safety. However, building accurate models that adequately represent real-world systems is a difficult and time-consuming task that requires extensive domain knowledge and a deep understanding of the simulation method used. Because of the significant effort involved in understanding a specific domain and building the required models, the current trend in the field is to build single-use simulators.

Generic simulation methods, such as the Discrete Event System Specification (DEVS), are well-positioned to break down the disciplinary silos. DEVS supports hierarchical and modular model development, it favours the reusability of models. It is based on a rigorous formalism to manage inputs and outputs of atomic models, the basic block of the specification. Atomic models can be integrated into larger, more complex coupled models. However, the DEVS specification sees only limited usage in the industry and outside the research community due to its lack of approachability. Compounding the issue is the fact that DEVS approachability rarely is the subject of research by the community. Thus, the available development environment, particularly for non-expert users, does not adequately support modelling, experiment management, or building DEVS based applications.

Over the past decade of conferences on M&S, panels of eminent researchers have identified several grand challenges that should guide research by the community. Notably, two of these challenges are related to the approachability of the discipline by non-expert users:

- Fujimoto identifies the need to lessen the burden on the modellers by reducing the efforts required to develop models [1]. It should be possible to compose existing models of subsystems into an accurate model of a wider system.

- Taylor et al. identify the democratization of M&S as a challenge to be addressed [2]. Researchers now require adequate tools to support their work, including building online platforms, mobile applications, developing customized robot behaviours, and planning the use of space. These tools empower researchers in fields other than M&S to collaborate across multiple application domains.

<b>[1]</b> <i>R. M. Fujimoto, "Research challenges in parallel and distributed simulation," ACM Transactions on Modeling and Computer Simulation (TOMACS), vol. 26, no. 4, p. 29, 2016.</i>

<b>[2]</b> <i>S. J. Taylor, A. Khan, L. K. Morse, A. Tolk, L. Yilmaz, J. Zander, and P. J. Mosterman, "Grand challenges for modeling and simulation: simulation everywhere — from cyberinfrastructure to clouds to citizens," Simulation: Transactions of the Society for Modeling and Simulation International, vol. 91, no. 7, pp. 648-665, 2015.</i>

## Software Purpose

In this project, we propose to implement a workflow-based approach meant to support the complete simulation lifecycle. Through this system, users will be able to submit geospatial data, convert them into spatial DEVS simulation models, and execute the simulation using the Cadmium simulator developed at the Advanced Real-Time Simulation Laboratory. Simulation results will then be optimized for web visualization in the DEVS WebViewer developed in the same laboratory.

The purpose of this software is to address the two challenges previously identified. Automated generation of large-scale spatial simulation models from geographic information system data seeks to lessen the burden of modelling for modellers. The integration of several already existing applications (i.e. Model generator, Cadmium and DEVS WebViewer) through this software aims at democratizing the simulation lifecycle for non-expert users.

## Software Requirements

1. Users must be able to provide geospatial data as a source for the model generation workflow in the following geospatial data formats: shapefile, CSV, and GeoJSON. The data files provided should also contain non-spatial attributes.

2. Users must be able to select the simulation model to be mapped onto data from a library of models.

3. The system should be portable to any mainstream operating systems: Linux, macOS, and Windows.

4. The system will perform spatial analysis of the geospatial data provided to establish model parameters. The system must support common spatial analysis functions to establish neighbourhood and parameters:
	1. Topological relationships (e.g. intersection, contains, within, and touches);
	2. Geospatial statistics;
	3. Network distance analysis; and
	4. Spatial buffers.

5. Users must be able to provide a workflow file that contains the steps for automated model generation. The system will automatically execute the workflow to convert geospatial data into a Cadmium model configuration file. Through the workflow, the system will:
	1. Map geospatial features onto selected parametric simulation models.
	2. Establish neighbourhoods or couplings through spatial analysis of the geospatial data.
	3. Output a configuration file (json) properly formatted for Cadmium.

6. The system will execute the simulation and return the results.

7. The system must convert the Cadmium results into a common specification for web-based visualization through an existing conversion web service.

8. The system must allow users to configure a simulation visualization (colour, classification of data, number of classes, map symbology, etc.) for the DEVS WebViewer application.

## Software Releases
The software will be released in three versions.

The purpose of the first release is to provide the necessary tools enabling users to upload all the required data related to the simulation lifecycle. This first release will include requirements 1, 2, and 3.

The purpose of the second release is twofold: first, the software will be able to perform common spatial analysis functions; on the other hand, the software will be able to interact with the Cadmium M&S tool to perform the simulations selected by the users. This second release will include requirements 4, 5, and 6.

The purpose of the third release is the integration of visualization into the simulation lifecycle. This third release will include requirements 7 and 8.
