
My comment applies to the optional part in section 4.3 (about heap exhaustion).
It seems to be implying that 'heap exhaustion' is the only condition for failure in heap memory allocating functions. But I do not think this is the case. Other reasons for allocation failure may include:
* requested memory block is too big
* permission-related issues

Here is the situation with one of the programs in my company. We need to efficiently store huge data structures in RAM, with lots of pointers, and we have, say, 64 GB of RAM on the machines. In order to address this 64GB we require 32 + 4 bits in a pointer, the remaining 28 bits would be a waste. So, in order to avoid waste, we build the apps in 32-bit mode: this way we are able to address 4GB memory with 32-bit pointers and if we run 16 instances on one box, we are able to address the required 64GB of RAM. Of course, I got the numbers oversimplified, but you get the picture. Because we are using 32-bit applications we may get allocation failures not because we run out of virtual memory, but because we run out of addressable space. And this is reported cleanly by throwing std::bad_alloc on Linux systems.

It happens that while processing some customer requests huge amount of data is generated (which we cannot easily predict ahead of time from just reading the request), and a huge allocation is performed. There is still lots of heap memory available, but because memory is fragmented there is no single chunk to be found capable of storing the required amount of data. This is neatly reported by throwing std::bad_alloc. The stack is unwound and and the customer request is reported as failed, but the program still runs and is still able to allocate heap memory (even during the stack unwinding).

This situation is reproducible fairly easily even in 64-bit application. I have the following short program which tries to allocate a huge chunk of memory:

```
#include <iostream>
#include <vector>
#include <string>

int main()
try {
  std::vector<char> v (1024 * 1024 * 1024); // huge allocation
  std::cout << "OK" << std::endl;
}
catch(std::bad_alloc const& e)
{
  std::vector<char> s {'E', 'R', 'R', 'O', 'R', ':', ' '}; // reasonable allocation
  std::cout << std::string(s.begin(), s.end()) << e.what() << std::endl;
}
```

Then in Linux, I set a limit on virtual memory allocation with ` ulimit -S -v 204800`, and I run the program. It throws, catches and reports bad_alloc. It even allocates memory while handling the exception. This is all fine because the heap has not really been exhausted. This example disproves the claim that allocation failure cases are difficult to test.

Now, P0709 has this advice for me: use functions like `try_push_back()` or `new (std::nothrow)` instead. I do not find it acceptable, because it wants me to treat this recoverable error in a different way than any other error. While in general the motivation for P0709 is that exceptions are used for handling recoverable errors, for my error I am told to do just the opposite, with all the downsides that follow:
1. I will now have to manually convey the Boolean information about allocation failure through dozens layers of stack: much like error codes are reported in C, which is (as we know) error prone.
2. I cannot report allocation failures from constructors. I allocate memory not only with explicit calls to `push_back()` but also through copying std::vectors and std::strings.
In a way this pushes on me problems that P0709 claims to solve.

The solution to this tension that I can see is to provide a way to tell apart heap exhaustion form any other allocation failure reported by std::bad_alloc. One way, suggested to me by Tomasz Kaminski, is to provide a program-wide constant which represents the memory allocation threshold. A programmer can set it while building the program, or maybe even later, when the program is run. If memory allocation fails for whatever reason and the memory size requested is smaller than the threshold, it means "we cannot allocate even tiny amount of memory, we will likely not be able to even unwind the stack", this can be called heap exhaustion; otherwise (if the caller requested for more memory than the threshold), it may mean an unusually big allocation, which cannot be interpreted as heap exhaustion.

But even in this case memory allocating functions would have to be marked as potentially throwing, and the goal of "STL funcitons almost never throw" is still compromised.

I guess, my recommendation would be to drop the direction that any function annotated with `throws` requires an explicit `try` upon any potentially throwing path. At least drop it from P0709. If such explicit try-expressions are needed they would have to be explicitly requested for with additional syntax: programmers would do it only locally and would probably be only calling functions that never allocate memory.
