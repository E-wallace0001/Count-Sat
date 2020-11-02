# Count-Sat

Count Sat is a #sat solver based on the countability of infinite sets. 

Count Sat can count all of the possibilities of a logical problem written in a DIMACS format.

This release will try and count all of the solotuions, without finding individual soloutions independently, and will do so in logarithmic time, but for a superexponential problem; small problems can be solved.

The computational complexity and space complexity of this algorithm are equal. Large interconnected problems will overrun memory very quickly.

## To use
Count Sat as only been tested on Ubuntu 20.04.
in the terminal, run ./CountSat , followed by a .cnf file.

## Compilation
This software require GNU MP bignum installed, and can be compiled using ./make.sh
