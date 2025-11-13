#!/bin/bash
cd ..
premake5 gmake
rm -r bin bin-int
bear -- make
