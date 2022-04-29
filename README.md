# Species-Simulator
A simulator for neural networks that run on separate machines while sharing the same environment via a client-server framework. 
Species-Simulator is a project to practice asynchronous system design with a focus on scalability and effective, easy to manage code architecture. 

### How it works
The server connects two clients by sharing player positions and map updates between them. The clients run the neural nets of each player locally
and apply a genetic algorithm every few generations. The players that leave the client's map are sent to the next client via the server. This framework enables two different machines to compute neural nets locally in a shared simulation. Thus, computational power can be split over multiple PCs. 
In the future, this feature will be used to simulate the behavior of distinct communities represented by various clients. Additionally, this architecture allows for different 
environments to be run by separate machines depending on their computational power.  

## Features
- Netframework for the sharing of serialized data
- Creating a simulation environment and distributing it to the clients
- Sharing entities between server and client
- Custom Windows Console Handler
- Thread save console buffer
- Sendable protocol in which each data type can be serialized and deserialized
- Abstraction levels and derivable data structures
- Clear seperation of client and server tasks
### Still in Progess
- Encoding of neural net structures in chromosomes (todo)
- Neural nets featuring sensory-, inter-, and output neuron (todo)
- Genetic algorithm to assign fitness scores and control the population (todo)
- Different communities (todo)
- Statistic insides regarding the population's performance (todo)
- Logger function to replicate and/or keep track of results (todo)
- Multithreading and asynchronous functions (todo (partly))

## Skills Practice
New skills to be implemented throughout this project include:
- asynchronous algorithm design
- network connections and sharing data via sockets using boost::asio
- smart pointers
- multithreading
- data management using serializable structures
- easy to use protocols and libraries to abstract low-level code from the actual functional code
- Github Projects beta workflow
