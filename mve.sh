#!/bin/bash

#https://github.com/simonfuhrmann/mve
#git clone https://github.com/simonfuhrmann/mve.git

#photos dans images/
#binaires de mve dans bin/
#sortie dans le rép. scene/

scene="scene"
bin_mve/makescene -i images $scene
bin_mve/sfmrecon $scene
bin_mve/dmrecon -s2 $scene
bin_mve/scene2pset -F2 $scene $scene/pset-L2.ply
bin_mve/fssrecon $scene/pset-L2.ply $scene/surface-L2.ply
bin_mve/meshclean -t10 $scene/surface-L2.ply $scene/surface-L2-clean.ply