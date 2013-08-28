## Tint2 - the lightweight panel/taskbar

tint2 is a simple panel/taskbar unintrusive and light (memory / cpu / aestetic). 
We follow freedesktop specifications.

*NOTE: This is a fork slightly changed for my needs. Original svn repository can be found on https://code.google.com/p/tint2/*

## Run it:

```bash
$ tint2  -c  path_to_config_file
```

## Custom changes:

  - added left & right click support for battery panel

```
  // left click: minimise all windows
  battery_lclick_command = xdotool key --delay 300 super+d

  // right click: open power statistics  
  battery_rclick_command = gnome-power-statistics
```

## Original repository:
Check http://code.google.com/p/tint2/ for latest release, documentation and sample config file.
