#include "gtest/gtest.h"

#include "ffmpegpp.hpp"

TEST(CUDA, CUDA_SUPPORT) {
    ASSERT_TRUE(av::isCudaVideoEncodingDecodingAvailable());

    av::HWDecoderList hwDecoderList;
    av::getCudaVideoDecoderList(&hwDecoderList);
    for (auto& iter : hwDecoderList) {
        std::cout << "HW Decoder: " << iter << std::endl;
    }

    std::cout << std::endl << std::endl;

    av::HWEncoderList hwEncoderList;
    av::getCudaVideoEncoderList(&hwEncoderList);
    for (auto& iter : hwEncoderList) {
        std::cout << "HW Encoder: " << iter << std::endl;
    }
}