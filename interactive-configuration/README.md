#Interactive Configuration (IC)

This repository maintains the interactive configuration components, including the *client & server* programs. The design of this component will take each feature as a ***module***. In each module, it may contain one or more types of requests, we see each request as a ***component of a module***.

Currently, the program implements two modules:
* the ***basic*** module, which supports the basic operations to maintain a connection, such as connection setup, termination, etc.
* the ***lpm*** module, which support operations to update the LPM rules, such as obtaining filename, data, update the LPM rules, etc. The *rule_proc* folder contains the scripts to obtain the LPM rules. The *lpm-client* folder contains the client script.

##Modularity Design

As the design see each feature as a module, one can easily add new modules to the current server program, i.e., *fill up several template functions and data structures*. 
* For the client program, an easy way is to implement different clients for different features, as they may support completely different set of functionalities.
* For the server program, it needs to support all the features.
