#include <string>
#include <chrono>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
 
#include "app.h"
#include "opencv2/opencv.hpp"
#include "cnstream_logging.hpp"
 
 
//using namespace infer_server;
 
 
int Transform::CreatePool(BufSurfaceCreateParams *params, uint32_t block_count) {
    std::unique_lock<std::mutex> lk(mutex_);
    if (!pool_.CreatePool(params, block_count)) {
        pool_created_ = true;
        return 0;
    }
    LOGE(SOURCE) << "[Transform] CreatePool(): Create pool failed.";
    return -1;
}
 
void Transform::DestroyPool() {
    std::unique_lock<std::mutex> lk(mutex_);
    pool_.DestroyPool(5000);
}
 
void Transform::OnBufInfo(int width, int height, BufSurfaceColorFormat fmt) {
    memset(&create_params_, 0, sizeof(BufSurfaceCreateParams));
    create_params_.width = width;
    create_params_.height = height;
    create_params_.device_id = param_.device_id;
    create_params_.batch_size = 1;
    create_params_.color_format = fmt;
    create_params_.mem_type = BUF_MEMORY_HOST;
    return;
}
 
BufSurfWrapperPtr Transform::GetBufSurface(int timeout_ms) {
    if (pool_created_) {
        std::unique_lock<std::mutex> lk(mutex_);
        return pool_.GetBufSurfaceWrapper(timeout_ms);
    }
 
    if (param_.bufpool_size > 0) {
        LOGI(SOURCE) << "[Transform] GetBufSurface(): Create pool";
        if (CreatePool(&create_params_, param_.bufpool_size) < 0) {
            LOGE(SOURCE) << "[Transform] GetBufSurface(): Create pool failed";
            return nullptr;
        }
 
        std::unique_lock<std::mutex> lk(mutex_);
        return pool_.GetBufSurfaceWrapper(timeout_ms);
    }
 
    BufSurface *surf = nullptr;
    if (BufSurfaceCreate(&surf, &create_params_) < 0) {
        LOGE(SOURCE) << "[Transform] GetBufSurface() Create BufSurface failed.";
        return nullptr;
    }
    return std::make_shared<BufSurfaceWrapper>(surf);
}
 
 