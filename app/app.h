#ifndef __APP_H__
#define __APP_H__
 
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>
#include "buf_surface_util.hpp"
 
using namespace infer_server;
 
class IUserPool {
public:
    virtual ~IUserPool() {}
    virtual void OnBufInfo(int width, int height, BufSurfaceColorFormat fmt) = 0;
    virtual int CreatePool(BufSurfaceCreateParams *params, uint32_t block_count) = 0;
    virtual void DestroyPool() = 0;
    virtual infer_server::BufSurfWrapperPtr GetBufSurface(int timeout_ms) = 0;
};
 
/*!
 * @struct DataSourceParam
 *
 * @brief The DataSourceParam is a structure describing the parameters of a DataSource module.
 */
struct DataSourceParam {
  uint32_t interval = 1;  /*!< The interval of outputting one frame. It outputs one frame every n (interval_) frames. */
  int device_id = 0;      /*!< The device ordinal. */
  uint32_t bufpool_size = 16;    /*!< The size of the buffer pool to store output frames. */
};
 
 
 
class Transform : public IUserPool {
public:
    explicit Transform() {}
    ~Transform() 
    {
        DestroyPool();
    }
 
private:
    DataSourceParam param_;
    BufSurfaceCreateParams create_params_;
 
public:
 
    // IUserPool
    int CreatePool(BufSurfaceCreateParams *params, uint32_t block_count);
    void DestroyPool() override;
    void OnBufInfo(int width, int height, BufSurfaceColorFormat fmt);
    BufSurfWrapperPtr GetBufSurface(int timeout_ms) override;
 
private:
 
private:
    BufPool pool_;
    bool pool_created_ = false;
    std::mutex mutex_;
};  // class 
 
 
#endif