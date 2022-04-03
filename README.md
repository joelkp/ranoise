Random access noise algorithms & test programs
==============================================

This repository contains a copy of my PRNG algorithms and related things. It includes test programs, both my own tests and for generating a stream of pseudo-random `int` data sent to `stdout` for test purposes.

The basic idea of the "ranoise" functions is to use a simple counter as state for random number generation, to be changed with some positive or negative frequency; chaotic waveshaping transforms the current counter value to the pseudo-random number it indexes. A stream of noise is produced by looping through the use of a ranoise function, calling it with the different counter values. Random access within a noise stream is also allowed, by simply changing the argument of the function. Given the simplicity of the function, it's a good alternative to storing and using arrays of random data, and the randomness is not too shabby either.

See the article "[Random access noise: Avalanche effect through chaotic waveshaping](https://joelkp.frama.io/blog/ran-chaos-waveshape.html)" for more. Currently, programs for the following versions are included:
 * ranoise32
 * ranoise32b

There's also a header file for the stuff they and other variations on the same theme have in common, [muvaror32.h](muvaror32.h).

The C code files in this repository are licensed under the public domain-equivalent 0BSD license, meaning they can be copied from _without_ an attribution requirement (though credit is nice). The same applies to the code in this README.

The bare functions
------------------

While uglier than using macros or inline functions for bitrotation, etc., here's everything-in-one-go function definitions.

### ranoise32
This is an earlier version with some quirks and flaws, failing 3 of TestU01's medium-sized Crush tests. Works well as long as changes to x between calls are small or have lower bits set.
```
int32_t ranoise32(uint32_t x) {
        x *= 2654435769UL;
        x *= (x >> ((x + 14) & 31)) | (x << ((32-(x + 14)) & 31));
        x ^= (x >> 7) ^ (x >> 16);
        return x;
}
```

### ranoise32b
This is a reworked version which passes TestU01's medium-sized Crush tests, but still fails 5 BigCrush tests, when used with a plain increasing counter argument. Quality should also degrade more gracefully with suboptimal function argument patterns.
```
int32_t ranoise32b(uint32_t x) {
        x *= 2654435769UL;
        x ^= x >> 14;
        x = (x | 1) * ((x >> ((x >> 27) & 31)) | (x << ((32-(x >> 27)) & 31)));
        x ^= x >> 13;
        return x;
}
```

Running dieharder tests
-----------------------

If you install [dieharder](https://webhome.phy.duke.edu/~rgb/General/dieharder.php), then you can run a program named after a function from this repository through it as follows:

```
make && ./ranoise32 | dieharder -a -g 200

```

That one-liner is convenient to have at hand when tweaking a program to vary the value of the counter, or other things.
