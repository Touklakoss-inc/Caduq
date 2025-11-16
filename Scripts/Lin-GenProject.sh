#!/bin/bash
cd ../vendor/bin/linux/premake
premake5 gmake
rm -r bin bin-int
bear -- make
