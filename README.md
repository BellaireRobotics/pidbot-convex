pidbot-convex
=============

2013-2014 Toss Up VEX Robot: http://www.vexrobotics.com/wiki/Toss_Up

Control scheme:
Split arcade drive, Y-axis of left joystick controls fwd-reverse motion and X-axis of right joystick controls turning vector. Linearizing array allows smooth linear speed control.

PID-controlled lift, controlled by the right up/down triggers and right button pad. PID control w/ feedback from an IME on the leftTopLift motor locks arm in position regardless of load. Starting from the bottom and going clockwise on the right button pad, the 4 buttons correspond to the following arm height presets: full down, bump traversal, stash height, hang height.

Contra-rotating intake, controlled by left up/down triggers. Up = intake, Down = outtake

Pneumatic lock for high hang, controlled by left button pad. Up = lock, Down = unlock

Autonomous routine:
Unfold intake, attempt to intake a large ball, and then high hang during autonomous. Of course, this autonomous is a hanging zone-only autonomous.

Develop
-------
* [ConVEX](http://jpearman.github.io/convex/)
  * [Getting Started](http://jpearman.github.io/convex/doxygen/html/getstarted.html)
  * [ConVEX Docs](http://jpearman.github.io/convex/doxygen/html/index.html)
