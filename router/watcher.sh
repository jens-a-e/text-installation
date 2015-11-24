#!/bin/bash
fswatch -0 -or -l 0.5 ./root | xargs -0 -n 1 -I{} ./sync.sh