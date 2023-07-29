
# MEA8000SIM

A software simulator for the MEA8000 speech synthesis chip, available as a speech device for various 80's computers


## Installation

A C compiler and make command should be enough.

You will also need the lib libsndfile on your system.

```bash
  $ make
  gcc -c -o main.o main.c -Wall
  gcc -c -o bin.o bin.c -Wall
  gcc -c -o filter.o filter.c -Wall
  gcc -c -o synth.o synth.c -Wall
  gcc -o mea8000sim main.o bin.o filter.o synth.o -Wall -lm -lsndfile
  $
```
    
## Usage/Examples

The file demo_TO.bin is provided as an example. It comes from the demonstation program of the cartridge 'Cedic - Synthétiseur de paroles' for the Thomson T07 computer.

```
$ ./mea8000sim demo_TO.bin 1 demo_TO_sample1.wav
File demo_TO.bin, read 6288 bytes.
Sample 1 : start offset 51, length 497.
Output file demo_TO_sample1.wav written.
$
```


## Optimizations

The code is not particularly optimized, the idea was more to understand how the chip works.

The filter code is borrowed from the Mame implementation , see https://github.com/mamedev/mame/blob/master/src/devices/sound/mea8000.cpp

I'm not sure this code matches what happens in the MEA8000 chip, but I didn't manage to make it work by implementing it as decribed in the Philips Application Note (see Appendix below).

## License

Copyright (c) 2023 Laurent FRANCOISE . All rights reserved. 

This project is released under the GNU Public License.


## Appendix

The application note from Philips http://zx81.ordi5.free.fr/vtr/download/MEA8000-2.pdf

General information on the MEA8000 chip https://www.cpcwiki.eu/index.php/MEA8000

More data files here https://forum.system-cfg.com/viewtopic.php?t=12940 (look for datas.zip)

A book in french related to this chip http://dcmoto.free.fr/documentation/parole_micros/index.html