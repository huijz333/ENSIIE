#!/bin/zsh

# script pour lancer tous les fichiers .py
print "CTRL-C pour arrêter" ; for f in `find .` ; do print "processing $f" ; python $f ; done
