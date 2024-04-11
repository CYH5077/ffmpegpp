#include "superess/impl/DnnSuperResImpl.hpp"
#include "superess/impl/CVMatImpl.hpp"

#include "opencv2/opencv.hpp"
#include "opencv2/dnn_superres.hpp"

namespace ff::dnn {
    DnnSuperResImplPtr DnnSuperResImpl::create() {
        return std::make_shared<DnnSuperResImpl>();
    }

    DnnSuperResImpl::DnnSuperResImpl() {

    }

    DnnSuperResImpl::~DnnSuperResImpl() {

    }

    AVError DnnSuperResImpl::readModel(const std::string &modelPath) {

        this->dnnSuperResImpl.readModel(modelPath);

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    void DnnSuperResImpl::enableCuda(bool isEnable) {
        if (isEnable) {
            this->dnnSuperResImpl.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
            this->dnnSuperResImpl.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
        } else {
            this->dnnSuperResImpl.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
            this->dnnSuperResImpl.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        }
    }

    void DnnSuperResImpl::setModel(const std::string &modelName, int scale) {
        this->dnnSuperResImpl.setModel(modelName, scale);
        this->scale = scale;
    }

    void DnnSuperResImpl::upsample(CVMatImplPtr srcImage, CVMatImplPtr dstImage) {
        cv::Mat img = srcImage->getRaw();

        // 64 x 64 이미지로 나누기
        std::vector<cv::Mat> chunks;
        int chunk_size = 64;
        for (int y = 0; y < img.rows; y += chunk_size) {
            for (int x = 0; x < img.cols; x += chunk_size) {
                cv::Rect roi(x, y, std::min(chunk_size, img.cols - x), std::min(chunk_size, img.rows - y));
                chunks.push_back(img(roi));
            }
        }

        // 각 청크를 EDSR x2 모델을 통과시키기
        std::vector<cv::Mat> upscaledChunks;
        for (auto& chunk : chunks) {
            cv::Mat upscaled_chunk;
            this->dnnSuperResImpl.upsample(chunk, upscaled_chunk);
            upscaledChunks.push_back(upscaled_chunk);
        }

        // 업스케일된 청크들을 하나의 이미지로 합치기
        cv::Mat result;
        int upscaled_chunk_size = chunk_size * this->scale; // EDSR x2 모델은 크기를 2배로 만듦
        int result_cols = img.cols * this->scale;
        int result_rows = img.rows * this->scale;
        result.create(result_rows, result_cols, img.type());

        for (int i = 0; i < upscaledChunks.size(); ++i) {
            int row = (i / (result_cols / upscaled_chunk_size)) * upscaled_chunk_size;
            int col = (i % (result_cols / upscaled_chunk_size)) * upscaled_chunk_size;
            upscaledChunks[i].copyTo(result(cv::Rect(col, row, upscaledChunks[i].cols, upscaledChunks[i].rows)));
        }

        dstImage->setMat(result);
    }

};