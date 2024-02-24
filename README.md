# Remote Control Arduino

Just controling line follower with a remote control.

## Tests on TinkerCad

We carry out tests on TinkerCad to ensure the project works properly. You can view the test results [at this link](https://www.tinkercad.com/things/3ka2T3npHoD-copy-of-remote-control-arduino).

Basically, start in WAITING_FOR_CATEGORY, you choose the category Follower pressing 9 or stalker pressing 0. At this moment, code is for Follower, so it's works better if you pressed 9. After this, you can press 1 or 2, 1 for use an existing calibration and 2 for a new calibration. Finally, after calibration you press 3 for the race. A few seconds after race, come the completed state.

![Projeto no TinkerCad](https://github.com/Gabri-el-Batata/RemoteControlArduino/blob/main/RemoteControlArduino.png)

## How to use

It consists of simply knowing the IR codes in decimal and creating a state machine as you press a certain button
