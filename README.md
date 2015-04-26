# FT5206
Library for capacitive TouchScreen drived by FT5206xx chip<br>
Originally created by Eastrising, modded by Helge Langehaug with the name of FT5x06 and finally recoded by myself.<br>
The FT5x06 library has several errors and I cannot really get reliable data for the 5 touch concurrent positions so I have recoded the all thing.<br>
Now I can get the correct data and even example it's much more correct, not more garbage due incorrect coordinates, etc.<br>
In addition I've added a gesture decoder that should intercep various gestures but strangely I can get only the zoom in and zoom out with my FT chip, maybe I have to understand how to change it's firmware... dunno... The library it's now protected for possible 2 or more instances (before NOT!) and works in I2C at 400Khz.<br>
The FT chip it's really a controller by itself, it has his CPU and it does all the calculations so at the end the driver it's really simple job<br>
I have mainly recoded to work in tandem with my https://github.com/sumotoy/RA8875 library so I added some new features:<br><br>
<b>setTouchLimit</b> can dinamically restrict the touches on screen from 1 to 5 so in your code if you want just one touch you can use setTouchLimit(1);<br><br>
<b>getGesture</b> it decodes gestures issued with 2 fingers (so if you use setTouchLimit(1) it will always return 0!).<br>
Decoded gestures are:<br>
 - [0]: not recognized
 - [1]: move up
 - [2]: move left
 - [3]: move down
 - [4]: move right
 - [5]: zoom in
 - [6]: zoom out
 
 I do not understand why my chip recognize only 5 and 6 but try it, maybe you have a better firmware than mine in your FT chip!<br><br>
<b>getTSflag</b> return if you are phisically touching the touch screen and when you leaved the finger from the touch screen.<br>
See the encluded example for reference!<br><br>
*Connections:*<br>

..to continue...

 
