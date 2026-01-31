<img src="rsc/caduq_logo8_final.png" alt="Caduq Logo" width="300" height="300">

# Caduq

Caduq is a (baby) cad surface modeling software written in C++. Its end goal is to provide an opensource, lightwheight and easy to use cad surface modeling software. The first use cases will be the modeling of tubular frame for race car design. Today no opensource tool exist for doing so in an efficient way.

## Project status

Caduq is in a very very (very) early stage of developpement. So far, we can't even create objects in the GUI...

## Contributing
We wont accept any pull request but any suggestions would be welcome !

## Building from source

To build Caduq from source, first clone the repo and update all its dependencies:

```bash
git clone https://github.com/Touklakoss-inc/Caduq.git
git submodule fetch
git submodule upgrade
```
We use premake to create all the makefiles so at this stage the building process will be slightly different if you are on linux or windows. Don't worry, we give you a premake executable, so you won't need to install it.

### For Linux

If it is not, make the premake executable, like so:
```bash
chmod +x ./vendor/bin/linux/premake/premake5
```
And then launch it with the gmake parameter, in order to make all the necessary makefiles:
```bash
./vendor/bin/linux/premake/premake5 gmake
```
Now you can build caduq with the following command (you can omit the -j if you don't want to build on multiple cores):
```bash
make -j
```
When it is finished, you can launch caduq:
```bash
./bin/Debug-linux-x86_64/Caduq/Caduq
```

### For Windows

Execute `Scripts/Win-GenProject.bat` to generate the files for your IDE. By default, the IDE is Visual Studio 2022. You can change that in the `.bat` file, though we only tested the project with VS22.

## License
[GPL-3.0](https://choosealicense.com/licenses/gpl-3.0/)
