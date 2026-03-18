

## I/O-specific awaitables

The contract of an I/O-awaitable for `read_some` operation, excluding the zero-size buffer, is as follows:

> I will reach into the stream and let you know when I’ve pulled out any bytes, or whatever additional ‘seaweed’ happens to stick to my hand.

In this analogy, ‘seaweed’ stands for any stream state or event, such as "end of file" or "stream truncated", that the I/O functions cannot fully interpret, but the awaiter can.

### Representation

We choose to provice a ‘product’ return type (such as `tuple`) that in addition to carrying the number of read bytes, also provides a multi-purpose status code. This status code can be used to:

 1. Represent the ‘seaweed’ extracted from the stream in addition to bytes. In this case it is likely that you will get both the bytes and the seaweed.
 2. Indicate that the operation has been canceled. In this case you will get no bytes.

The failure to acquire the necessary resources is communicated via throwing an exception. This means that for platworms with `-fno-exceptions` resource acquisition failures are turned into program abort.
