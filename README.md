# Electric field simulator
Simple graphic electric field simulator written on C++ using GTK+.
![](https://github.com/asuvalov/electrodynamics-simulator/blob/master/.screenshots/highlight_arrow.png =250x)

# Build
```
git clone git@github.com:asuvalov/electrodynamics-simulator.git
cd electrodynamics-simulator
mkdir build && cd build
cmake ..
make
```
# Mouse events
| Event        | Action           |
| ------------- |:-------------:|
| Press right button | Add positive charge|
| Press left button | Add negative charge|

# Keybindings
| Key        | Action           |
| ------------- |:-------------:|
| `q`      | Quit simulator |
| `c`      | Remove all charges |
| `a`      | Draw / hide arrows |
| `l`      | Draw / hide lines |
