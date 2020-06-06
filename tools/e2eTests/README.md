# Core Flight System - Dockerized End-To-End Testing Tool

A tool to perform dockerized end-to-end tests of cFS.

This tool simulates network-based interactions between the cFS executable and a pseudo-ground system software acting as a test runner.

## Getting Started

### Requirements

* Docker 19 or higher
* Docker Compose compatible with compose file format 2.0 or higher

### Run the test

From the cFS top-level directory, run the following command:

```
docker-compose up --abort-on-container-exit --exit-code-from gsw
```

By default, the test runs with the following options:

* Operating System: Ubuntu 18.04
* Unit tests disabled
* Simulation: native
* Build type: debug
* Deprecated omitted

When the test passes, it exits with a zero exit code. Otherwise, it exits with a non-zero exit code.

Note: `--abort-on-container-exit` makes `cfs` stop when `gsw` (i.e. the test runner) exits, and `--exit-code-from gsw` returns `gsw` exit code (that is: the test status).

### Optional: Customize the cFS Image

The default options to build the images are specified in `docker-compose.yml` (located at the cFS top-level directory), lines 7-12:

```yaml
dockerfile: ./tools/e2eTests/platforms/Ubuntu/18.04/Dockerfile
args:
  - ENABLE_UNIT_TESTS=false
  - SIMULATION=native
  - BUILDTYPE=debug
  - OMIT_DEPRECATED=true
```

To customize these options:

1. First, make `docker-compose.yml` refer to the relevant `Dockerfile` to specify the expected operating system (cFS `dockerfile` configuration option, line 7):

    ```yaml
    cfs:
      build:
        ...
        dockerfile: ./tools/e2eTests/platforms/Ubuntu/18.04/Dockerfile
    ```

    To this end, use one of the following options.

    | Operating System | Path to the corresponding Dockerfile                       |
    |------------------|--------------------------------------------------------|
    | Alpine 3         | `./tools/e2eTests/platforms/Alpine/3/Dockerfile`       |
    | CentOS 7         | `./tools/e2eTests/platforms/CentOS/7/Dockerfile`       |
    | Ubuntu 18.04     | `./tools/e2eTests/platforms/Ubuntu/18.04/Dockerfile`   |
    | Ubuntu 20.04     | `./tools/e2eTests/platforms/Ubuntu/20.04/Dockerfile`   |

2. Second, build the relevant cFS Docker image (tagged `cfs`) by passing the expected option(s) using `--build-arg`:

   ```
    docker-compose build --no-cache --build-arg <key1=value1> [--build-arg <key2=value2> ...] cfs
    ```

	For instance, to enable unit tests and set the build type to release:

    ```
    docker-compose build --no-cache --build-arg ENABLE_UNIT_TESTS=true --build-arg BUILDTYPE=release cfs
    ```
    
3. Finally, run the test:
	
	```
	docker-compose up --abort-on-container-exit --exit-code-from gsw
	```

## Alternative: Direct Use of cFS Containers

It is possible to build cFS images and run the corresponding containers independently of the Docker Compose solution.

### Build the cFS image

To build a cFS image, run the following command from the cFS top-level directory (indeed, the working directory for the build context is expected to be the root of the cFS directory):

```
docker build \
-t <tag of the image> \
-f <path to the relevant Dockerfile> \
--build-arg ENABLE_UNIT_TESTS=false \
--build-arg SIMULATION=native \
--build-arg BUILDTYPE=<build type> \
--build-arg OMIT_DEPRECATED=<boolean value> .
```

Example:

```
docker build \
-t cfs_ubuntu18 \
-f ./tools/e2eTests/platforms/Ubuntu/18.04/Dockerfile \
--build-arg ENABLE_UNIT_TESTS=false \
--build-arg SIMULATION=native \
--build-arg BUILDTYPE=debug \
--build-arg OMIT_DEPRECATED=true .
```

Note that unit tests should preferably be disabled to decouple unit testing from end-to-end testing.

### Run the cFS Container

One the cFS image has been built, it can be run with the following command:

```
docker run --cap-add CAP_SYS_RESOURCE --net=host <tag of the image>
```

Example:

```
docker run --cap-add CAP_SYS_RESOURCE --net=host cfs_ubuntu18
```

The cFS ground system can then interact with the containerized cFS executable on localhost (on Linux and Windows hosts).

Note: `--cap-add CAP_SYS_RESOURCE` is required for queues to be handled properly. Failing to add the `CAP_SYS_RESOURCE` capability would prematurely terminate the execution of the cFS container.
