#include "gtest/gtest.h"

#include "config.hpp"
#include "ffmpegpp.hpp"


///////////////////////////////////////////////// FormatContext Open Test
TEST(TYPE_TEST, INPUT_CONTEXT_OPEN_1) {
    ff::FFAVInputContext inputContext;

    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    inputContext.close();
}

TEST(TYPE_TEST, INPUT_CONTEXT_DESTROY_TEST_1) {
    ff::FFAVInputContext inputContext;
}

TEST(TYPE_TEST, INPUT_CONTEXT_DESTROY_TEST_2) {
    ff::FFAVInputContext inputContext;

    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
}

TEST(TYPE_TEST, INPUT_CONTEXT_FAILED) {
    ff::FFAVInputContext inputContext;

    // Failed
    ff::AVError error = inputContext.open("./tasdfadfadfasdf123.mp4");
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::AV_ERROR);
}





///////////////////////////////////////////////// FormatContext Read Frame Test
TEST(TYPE_TEST, INPUT_CONTEXT_READ_FRAME_1) {
    ff::FFAVInputContext inputContext;

    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    ff::FFAVPacket packet;
    error = inputContext.readFrame(&packet);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
}

TEST(TYPE_TEST, INPUT_CONTEXT_READ_FRAME_2) {
    ff::FFAVInputContext inputContext;

    ff::AVError errro = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(errro.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    int readCount = 0;
    for (auto& iter : inputContext) {
       readCount++;
    }

    std::cout << "read count: " << readCount << std::endl;
}

TEST(TYPE_TEST, INPUT_CONTEXT_READ_FRAME_3) {
    ff::FFAVInputContext inputContext;

    ff::AVError error = inputContext.open(Config::SAMPLE_MP4);
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    // read를 EOF 까지 호출
    int readCount = 0;
    ff::FFAVPacket packet;
    while (inputContext.readFrame(&packet).getType() != ff::AV_ERROR_TYPE::AV_EOF) {
        readCount++;
        ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
    }
    std::cout << "read count: " << readCount << std::endl;
}