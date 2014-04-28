# uncomment these if running from default project location
# Path to ChibiOS
CHIBIOS    = ../../ChibiOS-RT
# Path to convex
#CONVEX     = ../../convex/convex/cortex
CONVEX     = /Users/leroux/src/robotics/vex/convex-gh/convex/cortex

# uncomment to use the optional code like the smart motor library
CONVEX_OPT  = yes

# User C code files
VEXUSERSRC = $(wildcard src/*.c)

# Uncomment and add/modify user include files
VEXUSERINC = include
