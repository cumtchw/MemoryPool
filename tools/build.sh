#!/bin/bash

root=$(pwd)

function build_release() {
	cd ${root}/../build
	
	echo "Delete cmake cache..."
	#delete all cache
	rm -rf *
	echo "Deleted cmake cache."
	echo
	echo "<------------------------------------------>"

	#cmake & make
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make clean
	make -j
}

function build_debug() {
	cd ${root}/../build

	echo "Delete cmake cache..."
	#delete all cache
	rm -rf *
	echo "Deleted cmake cache."
	echo
	echo "<------------------------------------------>"
	#cmake & make
	cmake -DCMAKE_BUILD_TYPE=Debug ..
	make -j
}

case "$1" in
	-h|--help)
		echo "Usage: $0 [option...], default build type is release."
		echo "-h, --help     for help information"
		echo "-d, --debug    build debug"
		;;
	-d|--debug)
		echo
		echo "<----------start building Debug---------->"
		echo
		build_debug
		;;
	*)		
		echo
		echo "<----------start building Release---------->"
		echo
		build_release
esac

exit 0 