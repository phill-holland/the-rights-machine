<b>The Rights Machine</b>


kubectl create secret docker-registry gcr-json-key --docker-server=eu.gcr.io --docker-username=_json_key --docker-password="$(cat /home/phill/.gcloud/annular-net-321608-20e4b74265ec.json)" --docker-email=fotofill.holland@gmail.com

kubectl patch serviceaccount default -p '{"imagePullSecrets": [{"name": "gcr-json-key"}]}'

https://blog.container-solutions.com/using-google-container-registry-with-kubernetes

kubectl patch serviceaccount default \
(out) -p '{"imagePullSecrets": [{"name": "gcr-json-key"}]}'

docker login -u _json_key -p "$(cat ~/Downloads/annular-net-321608-20e4b74265ec.json)" https://gcr.io

docker push gcr.io/annular-net-321608/the-rights-machine

docker pull busybox
docker tag busybox gcr.io/my-project/busybox
docker push gcr.io/my-project/busybox

gcloud container clusters get-credentials the-rights-machine-k8s-cluster-dev --zone europe-west2

kubectl apply -f deployment.yml

kubectl get pods

https://learnk8s.io/terraform-gke

https://blog.container-solutions.com/using-google-container-registry-with-kubernetes

https://cloud.google.com/container-registry/docs/pushing-and-pulling?_ga=2.91084057.-261845117.1627807888

Get CPU code working with simple query
write test
stream out result during request connection
implement SSL
*disable database*
implement rest API query in excel spreadsheet
NUGET package
create react/material UI website

Google Test Framework
https://google.github.io/googletest/quickstart-cmake.html

Cuda 11

Inlude Cmake

Ensure OpenSSL server (freeSSL service)
https://letsencrypt.org/

Double check streaming input and output,
stream in real-time, output streamed even if whole
json not finished inputted yet

(rights are either positive or negative)

Excel Integration

Nuget package (.net version>>)
for api calling

Authentication

oauth2

Front-end website

react
material-ui


input string components mapping into hashtable, single values

update windows sockets wsock file

remove database support?

check concurrency -- each new client, spawns new thread?

GPU job queue?


https://github.com/google/googletest


<b>Old Notes</b>

	// build in-memory queue
	// http client for test json post
	// database queue

	// pass in database connection stirng
	// gpu ID
	// whether it exits after startup
	// or quits

	// create interface for C++ OR Cuda processing
	// template class for this, process, T = interface
	// process loads data from database tables

	// also, use template for database type, ODBC, mySQL



	// need lock for obtaining messages..????

	// get message, update "process status" in table

	// or just assume one database per table, let the higher level deal with
	// a round robin approach

	// status : 1 = new, 2 = processing, 3 = done, 4 = error

	// only need to compute overlapping results
	// split lines into multiple lines!!!!   in xxxxxxxx
	//                                   out     xxxx
	// results in three "has" positions


	// need error console, that redirects to database
	// or console, or both

	// need json result/response system

	//curl --header "Content-Type: application/json"  --request POST --data-binary @body.json http://127.0.0.1:5555

<b>server.h notes</b>

// need error class, to create response error json/or message

// upon connection, create GUID for message return

// http  functions, two one for post, one to get results
// JSON post /input
// JSON GET // wait for result in response queue, per items

// need item limit depending on userID account

// need timeout, for inactivity on connection

// COUNT LINES IN JSON, RETURN LINE NUMBER IF ERROR

// check http headers
// check content-length

// PRE DECLARE GIANT ARRAY OF COMPONENTS
// AND HAVE FIFOS WITH POINTERS ONLY
// COMPONENT MEMORY MANAGER ..??

// NEED LOCKS FOR THAT -- WELL JUST FOR THE LAST PTR

// HAVE SMALLER QUEUE BUFFER FOR SERVER,
// ONE MESSAGE CAN BE SPLIT OVER MANY QUEUE INSERTS, PER ITEM

//#include "custom/string.h"

/*
void bsttst::bsttst::go(std::string filename)
{
    //std::string filename;

     std::ifstream in(filename, std::ios::binary);
    if(!in.is_open()) return;

    in.seekg(0, std::ios::end);
    std::streampos end = in.tellg();
    in.seekg(0, std::ios::beg);

    char *buffer = new char[end];
    if(buffer == NULL) return;
    memset(buffer, 0, end);

    in.read(buffer, end);
    in.close();

// ****
    boost::json::error_code ec;
    write(buffer, end, ec);
}
*/

if (get() == MODE::POST)
							{
								if (!parents.isempty())
								{
									::data::json::request::json *current = task.message.find(parents.FQDN());
									if (current != NULL) current->add(custom::pair(label, value));
								}

								queue::base *b = task.message.findQ(parents.FQDN());
								if (b != NULL) b->flush();

								if (parents.FQDN().icompare(task.message.items.FQDN()))
								{
									//data::user user = c->configuration.users->get(task.message.user);
									/*
									if (!user.isempty())
									{
										if(user.validate(task.message))
										{
											guid::guid g;
											task.message.guid = g.get();
											task.message.created = global::datetime::now();

											if (!c->configuration.manager->set(task))
											{
												error(string("MESSAGE_PUSH"));
											}
											else
											{
												if (c->configuration.requested != NULL)
												{
													
													//if (!c->configuration.requested->add(::pending::waiting(task.message.guid, task.message.user)))
													//{
														//error(string("ALREADY_REQUESTED"));
													//}
												}

												::data::response::response response;

												response.guid = task.message.guid;
												response.created = datetime::now();
												response.available = false;

												outputter.set(&response);
											}
										}
										else error(string("INVALID_APIKEY"));

									}
									else error(string("INVALID_USER"));
									*/
								}
							}
							else if (get() == MODE::GET)
							{
								if (!parents.isempty())
								{
									::data::json::request::json *current = requested.find(parents.FQDN());
									if (current != NULL) current->add(custom::pair(label, value));

									if (parents.FQDN().icompare(requested.FQDN()))
									{
										if (requested.guid.count() > 0L)
										{
											data::response::response result = task.response->find(requested.guid);
											if(result.validate(requested))
											{
												if (c->configuration.requested != NULL)
												{
													::pending::waiting temp(requested.guid, requested.user);
													if (!c->configuration.requested->remove(temp)) error(string("NOT_IN_PENDING"));
												}

												outputter.set(&result);
											}
											else
											{
												data::response::response::STATUS status = data::response::response::STATUS::UNKNOWN;

												if (c->configuration.requested != NULL)
												{
													::pending::waiting temp(requested.guid, requested.user);
													if (c->configuration.requested->contains(temp)) status = data::response::response::STATUS::PENDING;
													else error(string("NOT_IN_PENDING"));
												}

												result.clear();

												result.guid = requested.guid;
												result.user = requested.user;
												result.status = status;

												outputter.set(&result);
												// NEED TO ADD STATUS PENDING, IF WAITING FOR OBJECT

												// push not found back to user (create hash lookup for valid GUI'IDs)
												// check hash that GUI was pushed through to be processed

												// create output queue for pointers for json::response *
												// hmmm, but need to store the actual objects somewhere??
											}
										}
										else
										{
											Log << "no guid found\r\n";
										}
										// need a special type of task queue
										// push things onto queue, with key
										// have find function to retreive item
										// have queue item with datetime
										// so that responses expire if not picked up

										// validate request
										// check task queue
										// task.response
									}
								}
							}

							left = true;

							label.clear();
							value.clear();

							if (!parents.pop())
							{
								error(string("BRACKET_MISMATCH"));
							}

							--brackets;
							if (brackets < 0)
							{
								error(string("BRACKET_MISMATCH"));
							}
						}
						else if (receiving[i] == '[')
						{
							if (!parents.push(label))
							{
								error(string("NESTING_TOO_DEEP"));
							}

							label.clear();
							value.clear();

							left = true;
							++squares;
						}
						else if (receiving[i] == ']')
						{
							left = true;

							label.clear();
							value.clear();

							if (!parents.pop())
							{
								error(string("BRACKET_MISMATCH"));
							}

							--squares;
							if (squares < 0)
							{
								error(string("BRACKET_MISMATCH"));
							}
						}
						else if (receiving[i] == ':')
						{
							left = false;
						}
						else if (receiving[i] == ',')
						{
							if (get() == MODE::POST)
							{
								if (!parents.isempty())
								{
									::data::json::request::json *current = task.message.find(parents.FQDN());
									if (current != NULL) current->add(custom::pair(label, value));
								}
							}
							else if (get() == MODE::GET)
							{
								if (!parents.isempty())
								{
									::data::json::request::json *current = requested.find(parents.FQDN());
									if (current != NULL) current->add(custom::pair(label, value));
								}
							}

							left = true;

							label.clear();
							value.clear();
						}
						else if ((receiving[i] >= '0') && (receiving[i] <= '9'))
						{
							if (!left)
							{
								if (!value.push(receiving[i]))
								{
									error(string("STRING_TOO_LONG"));
								}
							}
						}
					}
					else
					{
						if (brackets >= 1)
						{
							if ((quotes && left))
							{
								if(!label.push(receiving[i]))
								{
									error(string("STRING_TOO_LONG"));
								}
							}
							else if ((quotes && !left))
							{
								if(!value.push(receiving[i]))
								{
									error(string("STRING_TOO_LONG"));
								}
							}
						}
					}
				}
			}
		}

		if ((bytes <= 0) && (++errors >= ERRORS))
		{
			if (read_counter >= content_length - 2)
			{
				if (get() != MODE::NONE)
				{
					goodbye();
				}
			}
			else error(string("READ"));
		}
	}
