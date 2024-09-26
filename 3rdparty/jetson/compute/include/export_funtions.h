#ifndef _TRTENG_EXP_FUNTIONS_H
#define _TRTENG_EXP_FUNTIONS_H
#include <stdlib.h>
#include <stdint.h>

#ifdef __GNUC__
#define TRTENG_API  extern
#else
#ifdef TRTENG_EXPORTS
#define TRTENG_API __declspec(dllexport)
#else
#define TRTENG_API __declspec(dllimport)
#endif
#endif


extern "C"{
	struct ErrInfo
	{
		int code;
		char errmsg[256 - 4];
	};

	struct trtnet_t;

	TRTENG_API int set_device(int gpuid, ErrInfo* ei);

	TRTENG_API trtnet_t* load_net_from_file(const char* fpath, ErrInfo* ei);

	TRTENG_API void release_net(trtnet_t* net);

	TRTENG_API int net_num_inputs(trtnet_t* hdl);

	TRTENG_API int net_num_outputs(trtnet_t* hdl);

	TRTENG_API const char* net_input_layer_name(trtnet_t* net, int idx);

	TRTENG_API const char* net_output_layer_name(trtnet_t* net, int idx);

	TRTENG_API int net_get_input_idx_by_name(trtnet_t* net, const char* layer_name);

	TRTENG_API int net_get_output_idx_by_name(trtnet_t* net, const char* layer_name);

	struct LayerDims
	{
		int n, c, h, w;
	};

	TRTENG_API int net_input_layer_dims(trtnet_t* hdl, int idx, LayerDims* pd);

	TRTENG_API int net_output_layer_dims(trtnet_t* hdl, int idx, LayerDims* pd);

    TRTENG_API int net_max_batch_size(trtnet_t* hdl);

	struct NetInoutLayerData
	{
		int layer_idx, size;
        size_t offset;
		void* data;
	};

    //for one shot can be called inside multi thread
	TRTENG_API int net_do_inference(trtnet_t* hdl, int nBatch, NetInoutLayerData* pInLdr, int nInLdr, NetInoutLayerData* pOutLdr, int nOutLdr, ErrInfo* pinfo);

    /* for multi shot, the three api must be called successively in single thread
     * net_data_input: prepare input data
     * net_do_inference: do inference
     * net_data_output: output result
    */
    TRTENG_API int net_data_input(trtnet_t* hdl, NetInoutLayerData* pInLdr, int nInLdr, ErrInfo* ei);
    TRTENG_API int net_do_inference_single(trtnet_t* hdl, int nBatch, ErrInfo* ei);
    TRTENG_API int net_data_output(trtnet_t* hdl, NetInoutLayerData* pOutLdr, int nOutLdr, ErrInfo* ei);

	//extension

	TRTENG_API int net_set_parameter(trtnet_t* net, int paramid, void* p1, void* p2);

	TRTENG_API int net_get_parameter(trtnet_t* net, int paramid, void* p1, void* p2);

}


#endif