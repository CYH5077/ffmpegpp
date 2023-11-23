#include "gtest/gtest.h"

#include "TEST_DEFINE.hpp"

#include "AVResult.hpp"
#include "InputFormat.hpp"

TEST(InputFomat_TEST, InputFormat_TEST) {
    av::AVResult result;

    av::InputFormat inputFormat;
    inputFormat.open(TEST::MP4_FILE, result);
    ASSERT_TRUE(result.isSuccess());

    inputFormat.printDump();

    inputFormat.close();
}