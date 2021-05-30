# Elfield - Electric Field Simulator<br />Easy to install - Easy to demonstrate
Simple graphic electric field simulator written on C++ using GTK+.
<br />
<p align="center">
  <img src="https://github.com/svlv/elfield/blob/master/.screenshots/Elfield_20210530_120351.png" alt="Screenshot..." width="400"/>
</p>

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
| Left click | Add a positive charge |
| Right click | Add a negative charge |
| Press left button and move cursor| Move a charge |
| Double left click on a charge | Open **Charge properties** dialog |

# Keybindings
| Key        | Action           |
| -----------|:----------------:|
| `:`      | Begin command line interface |
| `q`      | Quit simulator |
| `c`      | Remove all charges |
| `a`      | Toggle arrows |
| `l`      | Toggle lines |
| `p`      | Toggle potential |
| `x`      | Remove a charge under the cursor |
| `t`      | Toggle straight line / bezier curve |
| `j`      | Decrease line step for drawing |
| `k`      | Increase line step for drawing |
| `Ctrl+s` | Save to png file |
| `Ctrl+c` | Copy to clipboard |

# Command line interface
| Command | Action | Example |
|---------|--------|---------|
|`set_background_color`| Set the given background color | `:set_background_color #ffffff` |
