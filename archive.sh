#!/bin/bash

git archive HEAD --prefix=artyFX_git/ --format=zip > artyFX_git_`date +"%d-%m-%Y"`_`git log --pretty=format:"%h" | head -n1`.zip
