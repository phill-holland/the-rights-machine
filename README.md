<b>The Rights Machine (pre-release BETA)</b>

Rights Availability Reporting System.

When selling a particular asset, such as a TV programme and/or series a number of rights are attached to it detailing it's broadcast rights, for a fix period of time, within a given territory, language and transmission mediums (such as Live Broadcast TV, or Over the Internet, with services such as Netflix).

This system simplies the concept of acquired rights, excluded rights and granted rights, reducing the problem to positive rights and negatives, all rights can be viewed as a function of either adding or removing rights, although the traditional notion can still be kept.

Presented here is a rights management system for the computation of those rights, allowing a person to quickly determine unexploited sales opportunities related to those assets, the sytem is designed to be fast at computing this information for a large number of assets, in bulk.

It is designed to be provided "as-a-service" inside the cloud and to be as simple to use as possible.  Typical Rights Management systems come at great expense and unavailable at smaller scales to small/medium sized businesses, whilst also being largely restricted to the Media industry.  The system presented here has also been designed to be flexible, allowing aribtary rights assignments, with up to 10 components (typically these might include language, country, TX Methods, but are not limited to these, extra components can be added easily)

There is also poentially for the development of a "real-time" broadcast TV channel, whilst can calculate a broadcast schedule for a channel in seconds, determined by it's rights availability and other statistics, such as popularity of a particular show, at the particular time of day.

Other applications are many beyond the Media industry, in-fact if you have a large number of digital assets that you sell globally (such as music and/or computer games) that may have a fixed term of sales (i.e. expires on a certain date, or in perputuity) then this system would be useful to you.

This software is in the early stages of development (pre-release BETA), and requires more support and backing if it proves to be useful to a large group of people, whilst it functions correctly, it still lacks polish and scalability.  This software has been written from scratch after 10+ years extensive experience working in the rights management software industry.

<b>Summary of Features;</b>

- 10 Rights Components per Rights Lines - Configuratable to More
- Interchangable Computation Engines (from CPU to GPU when you're dealing with millions of lines of rights information)
- Real-Time Computations (no need to wait for whole rights data to be transmitted, will be processed with a partial JSON file)
- Switchable Rights Data Queue from In-Memory to In-Database.
- Designed for High Performance
- Capable of identifying rights opportunities not yet exploited

<b>Usage;</b>

Whilst the system can be integrated into other systems via a Web Rest API, we wanted it to be acccessible to all, no matter how big the company using it is, to this end, a spreadsheet has been provided with some sample data included, demonstrating it's use and configuration, you simply edit the data, and then click the compute button (???)

<<<google sheets link>>>
<<<excel link>>>

<b>Technical</b>

<b>Requirements</b>

The system has been written entirely in C++ for maximum performance, it was originally designed to be compiled on a Linux based system (such as Ubuntu).  In it's current form, it is very lightweight and does not require much resources.  Although it has database support, this is not currently enabled within the current build of the code.

The installation example below also requires that docker and docker-compose is already pre-installed.

Rights are processed in an asyncronise manor, whilst loading a JSON data file stream into the service, it will not wait for the whole data file to be loaded, before it starts processing and returning results.

<b>Installation</b>

Inside this code repo is provided a Dockerfile and docker-compose.yaml for simplicity. Currently the spreadsheet examples are hosted on a remote server, but also be self-hosted for greater performance on your local network.

To start, at the terminal window in the pre-existing directory containing the docker-compose.yaml file, type;

```
docker-compose build
docker-compose up -d
```

This may take some time to complete, the docker file has two stages, the first stage is building the code inside the container, and then a second container is built with just the runtime within.

The code during build also installs a debian package, obtain from the PPA repo within this Github account.

There are also a number of VSCode support files provided (and a workspace file) including a devcontainer, which is recommended for editing the code.

<b>Automated Testing</b>

A set of tests are included in the code, supported by the google testing suite (they require a little work at the moment, with an async call needed to wait for the response from the server)

<b>Manual Testing</b>

Curl can be used to issue a simple test query to the service (or indeed Postman), as shown below;

```
curl --header "Content-Type: application/json"  --request POST --data-binary @body.json http://127.0.0.1:5454
```

For example queries, the json structure below (for a basic availability check) can be written to a file called body.json, for use with the example above (more example also available within the test/data directory);

```
{
	"task" :
	{
		"queries" :
		[
			{
				"start" : "2018-01-02",
				"end" : "2018-05-01",
				"components" :
				[
					{
						"name" : "country",
						"elements" :
						[
							{ "value" : "england" },
							{ "value" : "france" }
						]
					}
					,
					{
						"name" : "language",
						"elements" :
						[
							{ "value" : "english" },
							{ "value" : "french" }
						]
					}
				]
			}
		],
		"items":
		[
			{
				"name" : "myitem",
				"lines" :
				[
					{
						"start" : "2018-01-01",
						"end" : "2019-01-01",
						"exclusivity" : 0,
						"type" : 0,
						"components" :
						[
							{
								"name" : "country",
								"elements" :
								[
									{ "value" : "england" },
									{ "value" : "france" }
								]
							}
							,
							{
								"name" : "language",
								"elements" :
								[
									{ "value" : "english" },
									{ "value" : "french" }
								]
							}
						]
					}
					,
					{
						"start" : "2018-06-01",
						"end" : "2019-08-01",
						"exclusivity" : 0,
						"type" : 1,
						"components" :
						[
							{
								"name" : "country",
								"elements" :
								[
									{ "value" : "england" },
									{ "value" : "france" }
								]
							}
							,
							{
								"name" : "language",
								"elements" :
								[
									{ "value" : "english" },
									{ "value" : "french" }
								]
							}
						]
					}
				]
			}
		]
	}
}
```

<b>Todo;</b>

- Enabling SSL/Https Support 
- OAuth2 Username+Password Authentication
- Load Balancing and Auto Scaling
- CUDA Testing
- Persistent Query Storage
- NUGET Package for DOTNET
- Add Configuration File
- Benchmarking for performance logging

<b>Support</b>

If you find a problem, or a bug, feel free to contact me, but support is currently limited due to time constraints.