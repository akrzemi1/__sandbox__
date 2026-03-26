# The Contract of Awaitables

This writeup describes the contract for awaitables:
  * both these provided by Capy/Corosio (dealing directly with I/O),
  * as well as higher-level ones build on top of the Capy/Corosio ones.
 
 We try to cover separately:

 1. What different *kinds* of irregular outcomes need to be communicated, and
 2. *How* they are communicated.


## The context

Most of the awaitables provided by Capy/Corosio when `co_await`-ed return types that are instantiations of `io_result<>`, 
These types can be "destructured" with a structure binding and their first element is of type `error_code`:

```c++
auto [ec, other...] = co_await capy::read_some(buf);
```

It is expected that the user code consuming these awaitables would be coroutines that also expose the awaitable interface on the outside. 
These awaitables would not necessarily, when `co_await`-ed, return `io_result<>` types.


## I/O-specific awaitables

### `read_some`

The contract of an I/O-awaitable for `read_some` operation, excluding the zero-size buffer, is as follows:

> I will reach into the stream and let you know when I’ve pulled out any bytes, or whatever additional ‘seaweed’ happens to stick to my hand.

In this analogy, ‘seaweed’ stands for any stream state or event, such as "end of file" or "stream truncated", 
which the I/O functions cannot fully interpret, but the awaiter can.

This promise may not be delivered in two cases:

 1. When system resources required to implement the function (here primarily memory) cannot be acquired.
 2. When the program or the framework decides that an already strated operation is no longer needed and it should stop wasting resources as soon as possible. This can happen via:
     * somebody calling `.cancel()`,
     * or a timeout.
   
    In this form of cancelation there may be a need for the operation to return all the results it has collected so far.


### `read_until`

The contract of an I/O-awaitable for `read_until` operation is:

> I will keep reading bytes from the stream until one of the following happens:
>
>  1. I get any ‘seaweed’ from the stream (`eof`, `stream_truncated`, which is a special case of `eof`, `connection_reset`, etc.),
>  2. I reach the buffer's `.max_capacity()`,
>  3. I fetched the byte sequence indicated in the argument.

Here also the promise may not be delivered in the same two cases:

 1. Upon resource exhaustion.
 2. Upon cancelation request (including upon timeout).


### Representation

We choose to provide a ‘product’ return type (such as `tuple`) that in addition to carrying the number of read bytes, also provides a multi-purpose status code. This status code can be used to:

 1. Represent the ‘seaweed’ extracted from the stream in addition to bytes. In this case it is likely that you will get both the bytes and the seaweed.
 2. Indicate that the framework decided to cancel the operation. In this case you will get no bytes.
 3. In the case like `read_until` -- with two possible successful non-seaweed outcomes -- to tell which of the two outcomes we reached.

The failure to acquire the necessary resources is communicated via throwing an exception. In this case you will get neither any bytes nor the seaweed. This also means that for platforms with `-fno-exceptions` resource acquisition failures are turned into program abort.


## Other awaitables

Usually it is coroutines that `co_await` on Awaitables. The user code that uses Capy and Corosio will be coroutines. These coroutines at the higher abstraction layers will not deal with I/O streams but with higher-level abstracitons: requests, tables, users. At these levels it is common to provide *stronger postconditions*, such as "I will get you the users" as opposed to "I will maybe get you users or maybe not":

```c++
// working with weak postconditions
result<vector<User>> users = co_await get_users_weak();
if (users)
  process(users);
else
  /* do what ? */
```

```c++
// working with strong postconditions
vector<User> users = co_await get_users_strong();
process(users);
```

In the case of a high-level awaitable with a strong postcondition, we may get the following outcomes:

 1. The data, as we advertised (full success).
 2. We couldn't acquire enough resources to perform the necessary operations. (No data will be returned.)
 3. The bytes we received over the network were not sufficient to form the promissed user list. (No "partial users" will be returned.)
 4. The program or the framework decided to cancel the already started operation because the data was deemed not needed. (No data will be returned.)


### Representations

Case #1 is trivial. Cases #2 and #3 are both simply failures to deliver the promissed data: they only differ by the reason. 
#4 is not easily representable in this interface, but given that the goal of strong postconditions is that the data can just be used without any "checks",
the only possible thing to be done is to throw a special exception. 


## Awaitables with weaker postconditions

For higher-level awaitables with weaker postconditions, ("I will either return the users or not"), not returning users is not a failure. In that case there is many ways to represent the four outcomes:

#2 could still be represented by throwing an exception. #3 could be represented by returning an *empty* result. #4 could be modeled either by an exception or by an empty result with a dedicated code.

Alternatively, given that we have an *empty* state, even the resource failures could be represented by it. A more complicated type, possibly reusing `io_result`, may be needed:

```c++
io_awaitable<optional<vector<Users>>>
```

Altenatively, the designers could communicate #2, #3 and #4 by output function parameters or by a thread-local variable à la `errno`.


## Observations

Regardless of the level or the representation, we always have three distinct cases to report: 
 1. success: we delivered what we promissed.
 2. failure: the external situation -- in the user input, in the environment -- did not allow us to deliver what we promissed.
 3. cancel: we might have been able to deliver the promise, but you didn't want it (and you wanted to save as many resources as possible).

Whenever the outcome is categorized as either failure or cancel there is *never* a need to communicate data or "partial data" to the awaiter. Only in the case of success is there a need to communicate the value, even if this value is only ‘seaweed’. The apparent "partial success" in the I/O-awaitables is the consequence of its representation conflating ‘seaweed’ (success) with cancel.

We do not even consider the incorrect input to the function (precondition violation) as something worth reporting: we assume only correct programs.

A generic `when_all` algorithm needs aclear classification of the awaitable's outcome into at least success and failure. Given that the designers of awaitables can be creative in how they communicate failure, there needs to be a classification tool that makes this uniform. In case of `std::execution` this tool is the three channels. 
