MD2Combiner
===========

Combines all animations (frames) from two MD2s into one MD2.

Skins, texture coordinates, triangles, and gl commands are taken from the first model.

##Usage
`MD2Combiner input1.md2 input2.md2 output.md2`

For example: `MD2Combiner marioWalk.md2 marioRun.md2 marioCombined.md2`

##Building
For Windows, run `make` on the directory and it should build `MD2Combiner.exe`.

I haven't tested building with other OSes, you may need to make some tweaks to the Makefile (remove `.exe` from `TARGET`).

##Acknowledgements
[David Henry](http://tfc.duke.free.fr/coding/md2-specs-en.html) - the MD2 loading code
