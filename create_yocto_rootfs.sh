#!/bin/bash

status=1

subdir=$1
[ -z ${subdir} ] && { echo "subdirectory must be specified"; exit 1; }

container=$2
[ -z ${container} ] && { echo "container must be specified"; exit 1; }

docker pull ${container}
cont_id=$(docker create ${container} /bin/false)

[ -z ${cont_id} ] && exit 1

if mkdir -p ${subdir}
then
    docker export ${cont_id} | tar x -C ${subdir} images
    status=$?
fi 

docker rm ${cont_id}

if [ ${status} -eq 0 ]
then 
    ln -sv images/$(basename ${subdir}/images/*) ${subdir}/content
    status=$?
fi

exit ${status}
