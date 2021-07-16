
Time Triggered Protocol


This example is based on the network time synchronisation protocol.
The Time syc client and server are replicated here, with some minor modifications.


In this example we have 4 time triggered senders and one receiver.
The sender (placed in the directory client) use a synchronised UTC and an node Id assigned by server. This node Id goes from 0 to 3.
To avoid collisions we use a time triggered protocol. Each sender sends a message per seconds at different offsets:
	node 0 : offset 0 ms
	node 1 : offset 250 ms
	node 2 : offset 500 ms
	node 3 : offset 750 ms


The ttp receiver (placed in the directory server) just print the local utc time for each arrived message together with sender id and content. Here you can see how the offset work.


The placement of ttp sender with time client, and ttp receiver with time server is just because they use the same print mask.





