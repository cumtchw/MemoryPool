#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
 
#include "app.h"
 
int main(int argc, char** argv) {
    const char* filename = "1920.nv12";
    const int width = 1920;
    const int height = 1080;
    const int frameSize = width * height * 3 / 2; // NV12 format: Y plane + UV plane
 
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return -1;
    }
 
    std::vector<uint8_t> buffer(frameSize);
 
    file.read(reinterpret_cast<char*>(buffer.data()), frameSize);
    if (!file) {
        std::cerr << "读取文件失败: " << filename << std::endl;
        return -1;
    }
    file.close();
 

    std::shared_ptr<Transform> transform_ptr = std::make_shared<Transform>();
    transform_ptr->OnBufInfo(width, height, BUF_COLOR_FORMAT_BGR);
    BufSurfWrapperPtr wrapper = transform_ptr->GetBufSurface(1000);
    BufSurface* surf;
    /*************************************************************************************************
    这里用BufSurfaceChown接口，那么BufSurfWrapperPtr的析构函数里面不会销毁surf，因为BufSurfaceChown函数内部把
    surf_ = nullptr;而析构函数内部有个判断if (owner_ && surf_) BufSurfaceDestroy(surf_), surf_ = nullptr;也
    就是当surf_不是null才会BufSurfaceDestroy(surf_)。
    ***************************************************************************************************/
    if (wrapper) {
        surf = wrapper->BufSurfaceChown();
    }
 
 
    cv::Mat bgrImg(height, width, CV_8UC3, surf->surface_list[0].data_ptr);
 
 
    cv::Mat yuvImg(height + height / 2, width, CV_8UC1, buffer.data());
 
    cv::cvtColor(yuvImg, bgrImg, cv::COLOR_YUV2BGR_NV12);
 
    //然后把这个surf进一步传给后续模块使用。然后这样surf又让智能指针BufSurfWrapperPtr给管理了，不需要我们手动的destroy。
    BufSurfWrapperPtr wrapper2 = std::make_shared<BufSurfaceWrapper>(surf);
 
    //实际场景可能是另一个模块用wrapper2去对图片做进一步处理。 这里使用 wrapper2 中的数据保存为 JPG 文件简单模拟下实际场景。
    BufSurface* surf2;
    /*************************************************************************************************
    这里用GetBufSurface接口，那么BufSurfWrapperPtr的析构函数里面就会销毁surf，因为析构函数内部有代码
    if (owner_ && surf_) BufSurfaceDestroy(surf_), surf_ = nullptr;
    ***************************************************************************************************/
    if (wrapper2) {
        surf2 = wrapper2->GetBufSurface();
    }
    cv::Mat finalImg(surf2->surface_list[0].height, surf2->surface_list[0].width, CV_8UC3, surf2->surface_list[0].data_ptr);
 
    if (!cv::imwrite("output.jpg", finalImg)) {
        std::cerr << "保存 JPG 文件失败" << std::endl;
        return -1;
    }
 
    std::cout << "成功读取 NV12 文件并保存为 JPG 文件。" << std::endl;
 
    return 0;
}