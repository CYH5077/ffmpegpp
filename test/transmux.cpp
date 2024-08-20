#include "ffmpegpp.hpp"
#include "gtest/gtest.h"

TEST(TRANSMUX, TRANSMUX) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open("sample.mp4");  // GPU Decode
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    auto streams = inputContext.getDecodeStreamList();

    ff::FFAVOutputContext outputContext;
    error = outputContext.open("transmux.ts");
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

    outputContext.copyStreams(streams);
    error = outputContext.writeHeader();
    ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);

     while (true) {
        ff::FFAVPacket packet;
         error = inputContext.demux(&packet);
         if (error.getType() != ff::AV_ERROR_TYPE::AV_EOF) {
             ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
             error = outputContext.writePacket(packet);
             ASSERT_EQ(error.getType(), ff::AV_ERROR_TYPE::SUCCESS);
         } else {
             break;
         }
     }
    

    outputContext.close();
    inputContext.close();
}
