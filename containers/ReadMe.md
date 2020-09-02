<!-- cFS Containers ReadMe.md -->

## gcc-cmake Containers

The gcc-cmake image can be used to build and run cFS and it's doxygen documentation.

### Building and running the gcc-cmake image

When building the image make sure to give it a name with the `tag` option

```bash
docker build -t gcc-cmake PATH_TO_DOCKERFILE_DIRECTORY
```

```bash
docker run -it --sysctl fs.mqueue.msg_max=3000  -v PATH_TO_CFS_DIRECTORY:/cfs gcc-cmake
```

## Running and building cFS

The gcc-cmake container is meant for mounting your host's cfs directory but building and testing inside the container.
Once you launch the container navigate to the cfs directory and run the following

```bash
cp -r cfe/cmake/sample_defs sample_defs
cp -r cfe/cmake/Makefile.sample Makefile
export SIMULATION=native
make install
```

The above command sequence will setup cFS to run inside the container by executing the following commands

```bash
cd build/exe/cpu1
./core-cpu1
```
