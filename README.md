<b>The Rights Machine</b>

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