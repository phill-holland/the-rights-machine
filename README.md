<b>The Rights Machine (pre-release BETA)</b>

Rights Availability Computation System.

When selling a particular asset, such as a TV programme and/or series a number of rights are attached to it detailing it's broadcast rights, for a fix period of time, within a given territory, language and transmission mediums (such as Live Broadcast TV, or Over the Internet, with services such as Netflix).

Presented here is a rights management system for the computation of those rights, allowing a person to quickly determine unexploited sales opportunities related to those assets, the sytem is designed to be fast at computing this information for a large number of assets, in bulk.

It is designed to be provided "as-a-service" inside the cloud and to be as simple to use as possible.  Typical Rights Management systems come at great expense and unavailable at smaller scales to small/medium sized businesses, whilst also being largely restricted to the Media industry.  The system presented here has also been designed to be flexible, allowing aribtary rights assignments, with up to 10 components (typically these might include language, country, TX Methods, but are not limited to these, extra components can be added easily)

There is also poentially for the development of a "real-time" broadcast TV channel, whilst can calculate a broadcast schedule for a channel in seconds, determined by it's rights availability and other statistics, such as popularity of a particular show, at the particular time of day.

Other applications are many beyond the Media industry, in-fact if you have a large number of digital assets that you sell globally (such as music and/or computer games) that may have a fixed term of sales (i.e. expires on a certain date, or in perputuity) then this system would be useful to you.

This software is in the early stages of development (pre-release BETA), and requires more support and backing if it proves to be useful to a large group of people, whilst it functions correctly, it still lacks polish and scalability.  This software has been written from scratch after 10+ years extensive experience working in the rights management software industry.

Usage;

Whilst the system can be integrated into other systems via a Web Rest API, we wanted it to be acccessible to all, no matter how big the company using it is, to this end, a spreadsheet has been provided with some sample data included, demonstrating it's use and configuration, you simply edit the data, and then click the compute button (???)

<<<google sheets link>>>
<<<excel link>>>

If you find a problem, or a bug, feel free to contact me.

// ***

A demo has been setup

Typically a TV show is sold 
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

```docker-compose up -d

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