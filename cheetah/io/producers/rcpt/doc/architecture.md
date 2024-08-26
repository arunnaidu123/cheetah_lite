/** @addtogroup architectures UDP Receptor
 *@{*/
# UDP Receptor Architecture #

The rcpt module's responsibility is to listen to UDP beamformed packets, parse them, and transfer the data
within into the data structures used for processing in a timely manner.

The diagram below illustrates the mapping that it is expected to make, between the packets and the data structures
that are propagated through the pipeline. The illustration only shows a few packets for each chunk, but this is a configurable
parameter, and there are more likely to be many hundreds of packets per chunk.

UDP Packets                     : <pre>| 1 | 2 | 3 | 4 | 5 | 6 | 7 | ...</pre>
[TimeFrequency] Data structures : <pre>|   chunk 1  |  chunk 2  | ...</pre>
                                  <pre>|----------- time --------------></pre>

In this example, the payload from UDP packets 1, 2, 3 and part of 4 map to the chunk 1.
The remainder of chunk 4's payload should fill the beginning of chunk 2. The remainder of chunk 2
should then contain the data from UDP packets 4, 5, 6 and part of UDP packet 7.

## Glossary ##
[TimeFrequency]: @ref TimeFrequency "TimeFrequency Data Structure"
[Context]: @ref panda::Context "A context object carrying state associated with a chunk"
release : when a [TimeFrequency] chunk is considered ready for passing down to the pipeline.


## Level 1 Requirements ##
For LOW (reference SKA-TEL.CSP.LOW-CSP.PSS-ICD-001):
- 8192 data per spectrum
- 8 bit per datum,
- ~10000 samples per second,
- 4 polarizations (in consecutive packets)
- ~2.6Gbps per beam

For MID (reference SKA-TEL-CSP-0000020):

- 4096 data per spectrum
- 8 bit per datum
- ~20000 samples per second
- 4 polarizations (in consecutive packets)
- ~2.6Gbps per beam

Current design has reception of 2 beams per node on a 10Gbps link.

## Low Level Design Requirements ##

### Pipeline Constraints
    - All [TimeFrequency] chunks must be released in correct time order (i.e. chunk 1 must be released before chunk 2).
      Rationale: This requirement allows us to reduce the complexity of the pipeline as all modules will be able to assume
                 time ordering. No attempt has been made to remove this constraint or analyse the consequences of doing so.
    - [TimeFrequency] chunks must be released in a timely manner
      Rationale: The data processing pipeline must be fed at a rate that can keep the compute resources utilised. Bunching
                 up of the throughput data may put unreasonable demands on the compute.

### The UDP packet format shall be encapsulated.
   Rationale:
    - It is easy to change with evolving system requirements without effecting the rest of the code.
    - A single source of these parameters

### Out of Order Packets
    - Packets may arrive out of order (UDP specification)
    - If a packet arrives after the chunk it corresponds to has already been released by the rcpt, then it shall be ignored
      Rationale:
        - to keep the chunks ordered in time, they must be released in order. Ordered chunks are an assumption of the pipeline.
        - the corresponding chunk will already of been released (or in the process of being released)i with the packet missing handler called.
    - packets will have a sequence number embeded within it
      Rationale: allow us to associate each packet with a specific place in a data chunk

### Missing Packets
    - Packets may be missing
    - All missing packets corresponding to a chunk, must be explicitly notified to some handler before it is released.
      That handler must be free to update part or all of the data in the chunk as it sees fit.
      Rationale:
        The exact behaviour of how to compensate for missing packets in the data is undefined and open to research.
        Using missing packet handlers allows us to try many stratergies without redesigning the whole rcpt component:
        * In line processing    :* we can perform some small scale calculations inside the handelr directly.
                                Compensation algorithms typically might want to access the rest of the data in the chunk to perform, for example, an interpolation.
        * Queued processing     :*  we can use the handler to queue async processing tasks. This will require us to use some mechanism to stop the chunk being released (and
                                    future chunks to maintain ordering) until the async tasks are completed.
        * In pipeline processing:* If more data than this is required (e.g. maintianing running averages etc. then this handler could be used to flag the data
        (using a suitable data structure chunk that supports flagging) deferring these decisions and processing for the main pipeline.
        This has the advantage of leaving decisions to the pipeline stage (different pipelinesi and/or data conditions might prefer different stratergies).
        The disadvantage is that we may have to use resource to iterate over the flags.

    - Any incomplete [TimeFrequency] chunk on rcpt halt (or destruction) shall be suitably resized to accomodate the existing data and released.
      Rationale:
        The rcpt should not be making decisions about wether to use or abandon data. Its job is to pass on everything it is got in a consistent
        manner.

### Implementation Constraints
- The size of the [TimeFrequency] data chunks are configurable but can be considered to be constant for each run.
    Rationale: This is simply a consequnece of the current implementation that maps the packets onto specific chunks.
               No attempt has been made to remove this constraint.
- Startup
    - the first packet recieved after starting shall correspond to the beginning of the first chunk
      Rationale: The chunks start time is not required to be on any boundary, so the first packet is most probably going to correspond
                 to the first data we wish to process.
    - any [TimeFrequency] chunks previous to this first chunk shall be ignored
      Rationale: it would be too expensive to realign an exisitng chunk, and we are unlikely to receive many other packets < first packet received

### COnfiguration Constraints
- Number of threads used shall be configurable
  Rationale: allows us to easily experiment for performance tuning
- The parameters of the chunk (number of samples, number of channels) shall be configurable.
  Rationale: Allows us to experiment with different values for maximum throughput. These parameters may vary
            with different data ranges, or other parameters.
- The UDP socket to listen to shall be configurable.
    Rationale: we are expecting multiple beams on each node, each with its own rcpt module listening on a different port

## Architecture Description ##
Previous experience with streaming UDP data from LOFAR and Alfaburst indicates that a single thread cannot process a real time data stream.
The solution is to use multiple-threads, however this turns a relatively simple problem into something rather more complex.

The rcpt has a single thread that responds to UDP packets arriving on the socket.
This thread creaates a context object, which allows access to the appropriate data chunk to which that packet
should belong as well as its relative location inside it.

The context is then passed onto a thread pool. As a thread becomes available it takes the packet and the context information to copy the payload into the chunk.

\verbatim
    ------
\endverbatim
### Measured Performance ##
**/@}/*  // end doxygen group
