Random access noise algorithms & test programs
==============================================

This repository contains a copy of my PRNG algorithms and related things. It includes test programs, both my own tests and for generating a stream of pseudo-random `int` data sent to `stdout` for test purposes.

The basic idea of the "ranoise" functions is to use a simple counter as state for random number generation, to be changed with some positive or negative frequency; chaotic waveshaping transforms the current counter value to the pseudo-random number it indexes. A stream of noise is produced by looping through the use of a ranoise function, calling it with the different counter values. Random access within a noise stream is also allowed by simply using different arguments of the function, just like array indices. These functions can be a good alternative to storing and using arrays of random data.

For each "ranoise" function, a function named with a `_next`-suffix is also provided; it behaves like a normal PRNG function for getting a next value, updating a passed state on each call. The state should be initialized before the first call to some value (any seed value is fine, including 0).

See the article "[Random access noise: Avalanche effect through chaotic waveshaping](https://joelkp.frama.io/blog/ran-chaos-waveshape.html)" for more. Currently, programs for the following versions are included:
 * `ranoise32` -- Minimal lower-quality version (compares well to 32-bit LCGs)
 * `ranoise32_old` -- Older medium-quality version (mostly obsoleted by `ranoise32b`)
 * `ranoise32b` -- Higher-quality version (compares well to SplitMix32)

There's also a header file for the stuff they and other variations on the same theme have in common, [muvaror32.h](include/muvaror32.h).

The C code files in this repository are licensed under the public domain-equivalent 0BSD license, meaning they can be copied from _without_ an attribution requirement (though credit is nice). The same applies to the code in this README.

The bare functions
------------------

While uglier than using the macros or inline functions for bitrotation, etc., here's everything-in-one-go function definitions.

For audio, there's no audible improvement when including the xor-and-rightshift steps which were added to improve avalanching and quality of bits below the highest, assuming the higher bits define most of the amplitude of the signal listened to in the usual way. The same may be true for graphics and what's visible.

### `ranoise32`
Stripped-down version of `ranoise32b`, which fails some TestU01 SmallCrush tests and as such is comparable to both 32-bit LCGs and xorshift32. The failures are more like those of LCGs, but fewer.
```
int32_t ranoise32(uint32_t x) {
        x *= 2654435769UL;
        x = (x | 1) * ((x >> (x >> 27)) | (x << (32-(x >> 27))));
        return x;
}
```

### `ranoise32_old`
This is an earlier version with some quirks and flaws, failing 3 of TestU01's medium-sized Crush tests. It works well as long as changes to x between calls are small or have lower bits set.

It's worth keeping for the distinctive smoothness of the output.
```
int32_t ranoise32_old(uint32_t x) {
        x *= 2654435769UL;
        x *= (x >> (x + 14)) | (x << (32-(x + 14)));
        x ^= (x >> 7) ^ (x >> 16);
        return x;
}
```
If the last line before the `return` is removed, the result is a function with failure patterns in dieharder testing which are very similar to those of a 32-bit LCG. (With that last step included, dieharder testing passes.)

The last step also works well to copy to the later `ranoise32` function as an improvement, but slightly less well than the alternative solution instead found in `ranoise32b`. (The difference is a few TestU01 BigCrush failures.) However, the old last step actually works _better_ for variations of the new function which re-add a bitrotation offset to produce one of 31 alternative outputs (easiest to do using the `MUVAROR32` macro, by changing the last argument from 0 to another number), and then any offset is basically fine to use, unlike this old function where the number 14 appears optimal.

### `ranoise32b`
This is a reworked version which passes TestU01's medium-sized Crush tests, but still fails 5 BigCrush tests (mainly four "Gap" tests), when used with a plain increasing counter argument.
```
int32_t ranoise32b(uint32_t x) {
        x *= 2654435769UL;
        x ^= x >> 14;
        x = (x | 1) * ((x >> (x >> 27)) | (x << (32-(x >> 27))));
        x ^= x >> 13;
        return x;
}
```

Running statistical tests
-----------------------

If you install [dieharder](https://webhome.phy.duke.edu/~rgb/General/dieharder.php), then you can run a program named after a function from this repository through it as follows:

```
make && ./ranoise32 | dieharder -a -g 200

```

There is also a utility called `TestU01_stdin` (multiple versions can be found) which, if built and installed after TestU01 is, allows similar testing with TestU01 (`-s` for SmallCrush, `-c` for Crush, `-b` for BigCrush):

```
make && ./ranoise32 | TestU01_stdin -s

```
