# Elfield - Electric Field Simulator<br />Easy to install - Easy to demonstrate
Simple graphic electric field simulator written on C++ using GTK+.
<br />
<img src="https://github.com/svlv/elfield/blob/master/.screenshots/potential.png" alt="Screenshot..." width="400" display="block" margin-left="auto" margin-right="auto"/>

# Build
```
git clone git@github.com:svlv/elfield.git
cd elfield
mkdir build && cd build
cmake ..
make
```

# Features
* Drawing charges:
    * Moving a charge under the cursor;
    * Removing a charge under the cursor;
* Drawing arrows to demonstrate electric field direction:
    * Toggling the arrows;
    * Hightlighting arrows under the cursor with the corresponding line;
* Drawing electric field lines coming from positive to negative charges:
    * Toggling the lines;
    * Choosing straight lines or bezier curves;
    * Choosing line step drawing;
* Drawing electric field potential

# Mouse events
| Event        | Action           |
| ------------- |:-------------:|
| Press left button | Add a positive charge |
| Press right button | Add a negative charge |
| Press left button and move cursor| Move a charge |
| Double press left button on a charge | Open *Charge properties* dialog |

# Keybindings
| Key        | Action           |
| ------------- |:-------------:|
| `q`      | Quit simulator |
| `s`      | Save to png file |
| `c`      | Remove all charges |
| `a`      | Toggle arrows |
| `l`      | Toggle lines |
| `p`      | Toggle potential |
| `x`      | Remove a charge under the cursor |
| `t`      | Toggle straight line / bezier curve |
| `j`      | Decrease line step for drawing |
| `k`      | Increase line step for drawing |

