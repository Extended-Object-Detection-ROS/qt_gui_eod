# QT GUI for Exteneded Object Detection
[![GitHub issues](https://img.shields.io/github/issues/Extended-Object-Detection-ROS/qt_gui_eod.svg)](https://github.com/Extended-Object-Detection-ROS/qt_gui_eod/issues) [![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause) ![version](https://img.shields.io/badge/version-1.0.0-blue) ![OpenCV](https://img.shields.io/badge/opencv-4.2.0-blue) ![QT](https://img.shields.io/badge/QT-5.15.2-blue)

Allows:
 - See and save results of object detection on static images
 - Edit and save object bases (XML-files with object descriptions)
 - Choose subset of objects for detection
 
![empty gui](doc/eod_gui_empty.png)

![filled gui](doc/eod_gui_filled.png)


# Install 
(tested on Linux Kubuntu 20.04 LTS with installed OpenCV 4.2.0)

1. Clone this repository locally (last release)
```bash
git clone --recurse-submodules https://github.com/Extended-Object-Detection-ROS/qt_gui_eod -b r1.0.0
```
2. (Optional) Install igraph for Complex Objects detection
```bash
mkdir tmp
cd tmp
curl -O https://igraph.org/nightly/get/c/igraph-0.7.1.tar.gz
tar -xzf igraph-0.7.1.tar.gz
rm igraph-0.7.1.tar.gz

sudo mv igraph-0.7.1 /usr/local/src/
cd /usr/local/src/igraph-0.7.1
./configure
make
make check
sudo make install
```
In CMakeLists.txt at 19 line change on:
```cmake
set(igraph ON)
```
3. Open CMakeLists.txt in qt-creator, build and run program

