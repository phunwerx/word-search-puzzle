## Synopsis

Nothing big and fancy here, just solves a 2d word puzzle by banging /usr/share/dict/words against a different views of a matrix.

## Code Example

for ruby:

```
s = Solver.new

s.solve
```

or python:

```
s = Solver()

s.solve()
```

pretty simple

## Motivation

A friend of mine had written a word puzzle solver on Mac OS using C++ and shared the code with me, which I was able to compile and tinker with it using gnu g++ on my Fedora Linux system.  It inspired me because it's a fun and interesting problem without being too complicated and I wanted to show how ruby, python, and friends are wonderfully small, beautiful, powerful, and expressive languages. :-)

## Installation

git clone git@github.com:phunwerx/word-search-puzzle.git

## API Reference

N/A

## Tests

chmod 755 solve

./solve

## Contributors

Philip Burchfield

## License

[GNU General Public License (GPL)](https://github.com/phunwerx/word-search-puzzle/blob/master/LICENSE.md)

