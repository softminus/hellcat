hellcat
=======

netcat that takes unfair advantage of traffic shaping systems that apply a
higher rate limit for the first k bytes or t seconds of a TCP connection than
for the rest of the connection. We just send k bytes over a TCP connection,
then we close it and then create another one. That way, data transfer happens
in the initial regime (with a higher rate limit), increasing the average rate
of data transfer.

