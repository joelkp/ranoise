Random access noise algorithms & test programs
==============================================

This repository contains a copy of my PRNG algorithms, some others for comparison, and related things. It includes test programs, to generate streams of pseudo-random `int` data sent to `stdout` (for use with statistical tests, e.g. `dieharder` or `TestU01_stdin`), and other code of my own. Note that most testing is for _sequential_ use of a PRNG, while the whole point of my own PRNGs is to also allow _random access_ (similar to integer-to-integer hashing) with the simplest code that does the job well enough. Other PRNGs are usually better for sequential purposes.

The basic idea of the "ranoise" functions is to use a simple counter as the state for random number generation. Position can be changed with various positive or negative frequencies, and "jumps" in the stream of noise are trivial. Chaotic waveshaping replaces the current input number with the pseudo-random number it indexes. A sawtooth-ish signal is transformed into white noise. Given their simplicity, these functions can be a good alternative to storing and using arrays of white noise.

For each "ranoise" function, a function named with a `_next`-suffix is also provided; it instead behaves like a normal sequential access PRNG function for getting a next value, updating a passed state on each call. The state should be initialized before the first call to some value (any seed value is fine, including 0).

See the article "[Random access noise: Avalanche effect through chaotic waveshaping](https://joelkp.frama.io/blog/ran-chaos-waveshape.html)" for more on the development of all this. Currently, programs for the following versions are included:
 * `ranoise32` -- Minimal lower-quality version (compares well to 32-bit LCGs)
 * `ranoise32_old` -- Older medium-quality version (mostly obsoleted by `ranoise32b`)
 * `ranoise32b` -- Higher-quality version (compares well to SplitMix32)

There's also a header file for the stuff they and other variations on the same theme have in common, [muvaror32.h](include/muvaror32.h).

The C code files in this repository are licensed under the public domain-equivalent 0BSD license, meaning they can be copied from _without_ an attribution requirement (though credit is nice). The same applies to the code in this README.

The bare functions
------------------

While arguably uglier than using the macros or inline functions for bitrotation, etc., here's everything-in-one-go versions of the function definitions.

For audio, there's no audible improvement when including the xor-and-rightshift steps which were added to improve avalanching and quality of bits below the highest, assuming the higher bits define most of the amplitude of the signal listened to in the usual way. The same may be true for graphics and what's visible.

### `ranoise32`
Stripped-down version of `ranoise32b`, containing only the most important part of the algorithm. Tested with an increasing counter as the argument, it fails some TestU01 SmallCrush tests and as such is comparable to both 32-bit LCGs and xorshift32. The failures are more like those of LCGs, but fewer.
```
int32_t ranoise32(uint32_t x) {
        x *= 2654435769UL;
        x = (x | 1) * ((x >> (x >> 27)) | (x << (32-(x >> 27))));
        return x;
}
```
This is the function most likely to be useful for signal processing purposes.

### `ranoise32_old`
This is an earlier version with some quirks and flaws. Tested with an increasing counter as the argument, it fails 3 of TestU01's medium-sized Crush tests. It works well as long as changes to x between calls are small or have lower bits set.

It's worth keeping for the distinctive smoothness of the output.
```
int32_t ranoise32_old(uint32_t x) {
        x *= 2654435769UL;
        x *= (x >> (x + 14)) | (x << (32-(x + 14)));
        x ^= (x >> 7) ^ (x >> 16);
        return x;
}
```
If the last line before the `return` is removed, the result is a function with failure patterns in dieharder testing which are very similar to those of a 32-bit LCG. (With that last step included, dieharder testing however passes.)

The last step also works well to copy to the later `ranoise32` function as an improvement for lower bits, but slightly less well than the alternative solution instead found in `ranoise32b`. (The difference is a few TestU01 BigCrush failures.) However, the old last step actually works _better_ for variations of the new function which re-add a bitrotation offset so as to produce one of 31 alternative outputs (easiest to do using the `MUVAROR32` macro used in the C file, by changing the last argument from 0 to another number), and then any offset is basically fine to use, unlike this old function where the number 14 appears optimal.

### `ranoise32b`
This is a reworked version which so far seems roughly as good as PRNGs with only 32 bits of state can be. (Many 64-bit PRNGs can do better.) Tested with an increasing counter as the argument, it passes TestU01's medium-sized Crush tests, but still fails 5 BigCrush tests (mainly 4 "Gap" tests).
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
-------------------------

If you install [`dieharder`](https://webhome.phy.duke.edu/~rgb/General/dieharder.php), then you can run a program named after a function from this repository through it as follows:

```
make && ./ranoise32 | dieharder -a -g 200

```

There is also a utility called [`TestU01_stdin`](https://framagit.org/-/snippets/6556) (my 2022 version with added options for bitreversal and verbosity) which, if built and installed after TestU01 is, allows similar testing with TestU01 (`-s` for SmallCrush, `-c` for Crush, `-b` for BigCrush, `-l` for LinearComp):

```
make && ./ranoise32 | TestU01_stdin -s

```

Or to write everything from a verbose TestU01 run (more than just the summary at the end), plus a final count of 32-bit samples generated, to a file:

```
make && ./ranoise32 | TestU01_stdin -sv 3>&1 2>&3 | tee file.txt
```

Current results
---------------

Outputs for various tests with the unmodified current programs in this repository can be found under [results/](results/).

The files for TestU01 tests named `*-rev.txt` use the `-r` option for `TestU01_stdin` to reverse the order of bits within each 32-bit word. When this results in more failures, it points towards randomness in the lowest bits being of worse quality than that in higher bits. (TestU01 is less sensitive to flaws in the lowest bits.)
