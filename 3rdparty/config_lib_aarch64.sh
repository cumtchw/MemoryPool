#!/bin/bash

#创建软连接脚本
#------------------
root=$(pwd)
arch=aarch64

#----------------------------------
# jetson: compute cuda
#----------------------------------
cd ${root}/jetson/compute/lib/linux_lib
\cp ../${arch}/* .

#1> Cuda
ln -snf libcublas.so.11.6.6.84 libcublas.so.11
ln -snf libcublas.so.11 libcublas.so

ln -snf libcublasLt.so.11.6.6.84 libcublasLt.so.11
ln -snf libcublasLt.so.11 libcublasLt.so

ln -snf libcudart.so.11.4.298 libcudart.so.11.0
ln -snf libcudart.so.11.0 libcudart.so.11
ln -snf libcudart.so.11 libcudart.so

ln -snf libcurand.so.10.2.5.297 libcurand.so.10
ln -snf libcurand.so.10 libcurand.so

ln -snf libnvrtc.so.11.4.300 libnvrtc.so.11
ln -snf libnvrtc.so.11 libnvrtc.so

ln -snf libnvrtc-builtins.so.11.4.300 libnvrtc-builtins.so.11.4
ln -snf libnvrtc-builtins.so.11.4 libnvrtc-builtins.so.11
ln -snf libnvrtc-builtins.so.11 libnvrtc-builtins.so

ln -snf libcudnn_ops_infer.so.8.6.0 libcudnn_ops_infer.so.8
ln -snf libcudnn_ops_infer.so.8 libcudnn_ops_infer.so

ln -snf libcudnn_cnn_infer.so.8.6.0 libcudnn_cnn_infer.so.8
ln -snf libcudnn_cnn_infer.so.8 libcudnn_cnn_infer.so

ln -snf libcudnn.so.8.6.0 libcudnn.so.8
ln -snf libcudnn.so.8 libcudnn.so


#----------------------------------
# opencv
#----------------------------------
cd ${root}/opencv/lib/linux_lib
\cp ../${arch}/* .

ln -snf libopencv_core.so.4.5.4 libopencv_core.so.4.5
ln -snf libopencv_core.so.4.5 libopencv_core.so

ln -snf libopencv_highgui.so.4.5.4 libopencv_highgui.so.4.5
ln -snf libopencv_highgui.so.4.5 libopencv_highgui.so

ln -snf libopencv_imgproc.so.4.5.4 libopencv_imgproc.so.4.5
ln -snf libopencv_imgproc.so.4.5 libopencv_imgproc.so

ln -snf libopencv_imgcodecs.so.4.5.4 libopencv_imgcodecs.so.4.5
ln -snf libopencv_imgcodecs.so.4.5 libopencv_imgcodecs.so

ln -snf libopencv_video.so.4.5.4 libopencv_video.so.4.5
ln -snf libopencv_video.so.4.5 libopencv_video.so

ln -snf libopencv_videoio.so.4.5.4 libopencv_videoio.so.4.5
ln -snf libopencv_videoio.so.4.5 libopencv_videoio.so

ln -snf libopencv_features2d.so.4.5.4 libopencv_features2d.so.4.5
ln -snf libopencv_features2d.so.4.5 libopencv_features2d.so

ln -snf libopencv_flann.so.4.5.4 libopencv_flann.so.4.5
ln -snf libopencv_flann.so.4.5 libopencv_flann.so

ln -snf libopencv_calib3d.so.4.5.4 libopencv_calib3d.so.4.5
ln -snf libopencv_calib3d.so.4.5 libopencv_calib3d.so

ln -snf libopencv_dnn.so.4.5.4 libopencv_dnn.so.4.5
ln -snf libopencv_dnn.so.4.5 libopencv_dnn.so

#----------------------------------
# gflags
#----------------------------------
cd ${root}/gflags/lib/linux_lib
\cp ../${arch}/* .
ln -snf libgflags.so.2.2 libgflags.so.2
ln -snf libgflags.so.2 libgflags.so

#----------------------------------
# glog
#----------------------------------
cd ${root}/glog/lib/linux_lib
\cp ../${arch}/* .
ln -snf libglog.so.0 libglog.so
