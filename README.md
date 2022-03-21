Random access noise test programs
=================================

This repository contains test programs for my "ranoise" algorithms, which generate a stream of pseudo-random data sent to `stdout` for test purposes.

The basic idea of "ranoise" is to use a simple counter, to be changed with some positive or negative frequency, and use chaotic waveshaping in order to transform or map the current counter value to the pseudo-random number it indexes. A stream of noise is produced by looping through the use of a ranoise function, calling it with the different counter values. Random access within a noise stream is also allowed, by simply changing the argument of the ranoise function. Keep the function simple and fast enough, and it becomes a better alternative to storing and using arrays of random data.

See the article "[Random access noise: Avalanche effect through chaotic waveshaping](https://joelkp.frama.io/blog/ran-chaos-waveshape.html)" for more. Currently, the following versions are included:
 * ranoise32

The C programs in this repository, which include a bit more than the bare algorithms (mainly a bit of text), are provided under the ISC license.

Running dieharder tests
-----------------------

If you install [dieharder](https://webhome.phy.duke.edu/~rgb/General/dieharder.php), then you can run a program from this repository through it as follows:

```
make && ./ranoise32 | dieharder -a -g 200

```

That one-liner is convenient to have at hand when tweaking a program to vary the value of the counter, or other things.
