# Group_E_Simulation_Life_Cycle
Françoise Blanc
Roman Cardenas Rodriguez
Ghida Dandan
Aidan Fahlman
Bruno St-Aubin

## Description
Need to Develop this Section


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



