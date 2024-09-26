# MemoryPool
C++内存池的高级实现，包含代码详解、CMake构建工程、应用实例。

代码详细解释在博客：https://cumtchw.blog.csdn.net/article/details/142184528?spm=1001.2014.3001.5502

上面的代码原本是https://github.com/Cambricon/CNStream 中的内存池相关的代码，我先是把整个的CNStream移植到了NVIDIA Jetson Orin上面，然后又把里面的内存池相关代码单独摘出来，至于案例，我就用个opencv做一下图片转换，然后里面用一下这个内存池代码，只是为了演示一下内存池的使用，

目前在NVIDIA Jetson Orin上跑通，jetpack版本是5.1.3. CUDA是11.4.
