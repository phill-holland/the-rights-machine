<b>The Rights Machine (pre-release BETA)</b>

Description;

Fast Rights Management Calculations as a Service.

// explain problem here

// status : 1 = new, 2 = processing, 3 = done, 4 = error

// only need to compute overlapping results
// split lines into multiple lines!!!!   in xxxxxxxx
//                                   out     xxxx
// results in three "has" positions


Features;

10 Rights Components per Rights Lines - Configuratable to More
Interchangable Computation Engines (from CPU to GPU)
Real-Time Computations (no need to wait for whole rights data to be transmitted, will be processed with a partial JSON file)
Switchable Rights Data Queue from In-Memory to In-Database.
High Performance, Fastest Rights Computations in the World.

Demo;

// spreadsheet links here

Local Development Environment Building;

Use the DeveContainer!

Deployment;

// run the docker-compose!

Testing;

curl --header "Content-Type: application/json"  --request POST --data-binary @body.json http://127.0.0.1:5454

// how to run gtests here

Benchmarking;

TBA

TODO;

SSL Support 
OAuth2 Username+Password Authentication
Load Balancing and Scaling
CUDA Testing
Database Rights Query Storage
NUGET Package
Add Configuration File

// add docker-compose deployment script and deploy to server
// add non-commerical use license
// google spreadsheet template
// excel spreadsheet
// SHOW DEMO JSON
// make plubic cplusplus-core repo
// setup docker container registry on server