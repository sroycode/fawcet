## Fawcet

Extracting approximate point to point cost using osrm

## Code is WIP

This code is work in progress and is to change frequently.

## What is it

Right now, provide a bbox and cost between all geohash pairs of given precision in it will 
be extracted. If cost is distance i.e. your osrm config uses distance it gets same

## Creating Development Environment

You need to install these tools using standard utils.
- cmake 3+,
- clang++/gcc5 ( supporting C++14)
- autoconf automake etc ( for thirdparty tools )
- glog and gflags.
- libosrm

## Compiling

You need to have libosrm , follow the instructions on https://github.com/Project-OSRM/osrm-backend

Create a build directory

```
mkdir build
cd build
# Debug
cmake -DCMAKE_BUILD_TYPE=Debug .. && make
# Release
cmake .. && make
```

## Running

This can run with ch or mld files, bbox uses (left,bottom,right,top) 
I usually use the excellent tool at http://norbertrenner.de/osm/bbox.html

```
./fawcet_main -btype bbox -prec 5 -data 77.469,12.813,77.789,13.085 --mldfile mld_filename.orsm 
./fawcet_main -btype bbox -prec 5 -data 77.469,12.813,77.789,13.085 --chfile ch_filename.orsm 
```

## Acknowledgements

This project uses code from

- https://github.com/Project-OSRM/osrm-backend
- https://github.com/simplegeo/libgeohash ( geohash )

## Sorry

... for the sparse documentation, I will make a more detailed description when time permits.

