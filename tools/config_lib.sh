#!/bin/bash

#创建软连接脚本
#------------------
root=$(pwd)

arch=$1
if [ "$arch" = "x86_64" ]; then
	cd ${root}/../3rdparty
	bash config_lib_x64.sh
	
elif [ "$arch" = "aarch64" ]; then
	cd ${root}/../3rdparty
	bash config_lib_aarch64.sh

else
	echo "[configure library]!!!!!!!!!!!! Architecture ${arch} not supported"
	exit 0
fi
