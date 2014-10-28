MD2Combiner
===========

Combines all frames from two MD2s into one MD2

##Usage
MD2Combiner input1.md2 input2.md2 output.md2

For example:

MD2Combiner marioWalk.md2 marioRun.md2 marioCombined.md2

##Building
Run make on the directory and it should build MD2Combiner.exe.

I haven't tested building with other OSes, you may need to make some tweaks to the Makefile (remove ".exe" in TARGET).

##Acknowledgements
David Henry - the MD2 loading code
