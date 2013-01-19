# examples startup script
# Execute this with orun runExamples.py.
# This script sets up any needed global setting, then passes execution to the
# selected example.
from omega import *

# Uncomment the line of the example you want to execute.

# -------------------------------------------------------------------------------------------------
# Examples

# Effects
#queueCommand(':r examples/fxdemo')


# -------------------------------------------------------------------------------------------------
# Tutorials

# Step 1: a basic scene
queueCommand(':r examples/tutorial/step1-basicScene')
