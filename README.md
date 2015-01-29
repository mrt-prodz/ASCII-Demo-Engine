#ASCII Demo Engine
 
####Experimenting with 2D graphics in ASCII 

This is a simple ASCII demo engine still at an early stage and in progress.

##Features:

Some of these features might be removed or changed:

* effect sequencer
* asset manager
* ASCII font
* ASCII sprite
* LUT based plane deformations
* starfield
* some simple 2d effects (waterfall, small explosion)
* keyboard shortcuts (toggle play/pause)

This project is for Windows only at the moment, Makefile and code have been tested with MinGW/MSYS.
If you compile the project do not forget to copy the assets folder in the same directory as the binary.
(or simply copy the binary from bin/Debug to the root directory of the project)

##Screenshot:
![Basic flowchart showing how the engine works](https://raw.githubusercontent.com/mrt-prodz/ASCII-Demo-Engine/master/flowchart.png)

![Some screen captures](https://raw.githubusercontent.com/mrt-prodz/ASCII-Demo-Engine/master/screenshot.png)

##TODO:

* transition example
* line drawing
* simple 3d objects
* add music library to play modules

##Reference:
[Ben Ryves Windows Console](http://www.benryves.com/tutorials/winconsole/)

[Iñigo Quílez Plane Deformations](http://iquilezles.org/www/articles/deform/deform.htm)

[deWiTTERS Game Loop](http://www.koonsolo.com/news/dewitters-gameloop/)
