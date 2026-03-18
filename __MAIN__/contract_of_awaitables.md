# The Contract of Awaitables

This writeup describes the contract for awaitables: both these related to I/O operations as well as more generic ones. We try to cover separately:

 1. What different *kinds* of irregular outcomes need to be communicated
 2. *How* they are communicated.

## I/O-specific awaitables

The contract of an I/O-awaitable for `read_some` operation, excluding the zero-size buffer, is as follows:

> I will reach into the stream and let you know when I’ve pulled out any bytes, or whatever additional ‘seaweed’ happens to stick to my hand.

In this analogy, ‘seaweed’ stands for any stream state or event, such as "end of file" or "stream truncated", 
which the I/O functions cannot fully interpret, but the awaiter can.

This promise may not be delivered in two cases:

 1. When system resources required to implement the function (here primarily memory) cannot be acquired.
 2. When the program of the framework decides that an already strated operation is no longer needed and must be canceled. This can happen via:
     * somebody calling `.cancel()`,
     * or a timeout. 


### Representation

We choose to provide a ‘product’ return type (such as `tuple`) that in addition to carrying the number of read bytes, also provides a multi-purpose status code. This status code can be used to:

 1. Represent the ‘seaweed’ extracted from the stream in addition to bytes. In this case it is likely that you will get both the bytes and the seaweed.
 2. Indicate that the framework decided to cancel the operation. In this case you will get no bytes.

The failure to acquire the necessary resources is communicated via throwing an exception. In this case you will get neither any bytes nor the seaweed. This also means that for platforms with `-fno-exceptions` resource acquisition failures are turned into program abort.
