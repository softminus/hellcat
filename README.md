hellcat
=======

netcat that takes unfair advantage of traffic shaping systems that apply a
higher rate limit (more bandwidth) for the first k bytes (or t seconds, configs
vary) of a TCP connection than for the rest of the connection. This is usually
done to make web browsing feel faster while still throttling big downloads. In
order to exploit this behavior, we send k bytes over a TCP connection, then we
close it and then create another one. That way, data transfer always happens in
the initial regime (the one with a higher rate limit), increasing the average
rate of data transfer.

You *need* to give the same chunksize value to the receiving end and the
transmitting end otherwise you will have incomplete transfer.

TODO: transfer the chunksize over the control connection from the sender to the
receiver so you don't have to specify it once on each end.

