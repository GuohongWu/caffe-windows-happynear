#ifndef CAFFE_MHE_LOSS_LAYER_HPP_
#define CAFFE_MHE_LOSS_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#include "caffe/layers/loss_layer.hpp"

namespace caffe {

	// implement MHE loss presented in the paper:
	// Learning towards Minimum Hyperspherical Energy, 2018.

	/*
		We assume that bottom[0] datas are L2-normalized.
	*/

	template <typename Dtype>
	class MHELossLayer : public LossLayer<Dtype> {
	public:
		explicit MHELossLayer(const LayerParameter& param)
			: LossLayer<Dtype>(param) {}
		virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
			const vector<Blob<Dtype>*>& top);
		virtual void Reshape(const vector<Blob<Dtype>*>& bottom, 
			const vector<Blob<Dtype>*>& top);

		virtual inline int ExactNumBottomBlobs() const { return 2; }
		virtual inline int ExactNumTopBlobs() const { return -1; }
		virtual inline int MinTopBlobs() const { return 1; }
		virtual inline int MaxTopBlobs() const { return 2; }
		virtual inline const char* type() const { return "MHELoss"; }

	protected:
		virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			const vector<Blob<Dtype>*>& top);
		virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			const vector<Blob<Dtype>*>& top);

		virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
			const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
		virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
			const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

		bool use_lambda_curve;
		Dtype lambda_m;       // if use_lambda_curve=true, lambda_m = lambda_max * [1 - (1 + gamma * iter)^-power]
		Dtype lambda_max;
		Dtype gamma;
		Dtype power;
		Dtype iter;
		Blob<Dtype> dot_prod_;
		Blob<Dtype> loss_temp_;  // temp for gpu_forward
	};

}

#endif
