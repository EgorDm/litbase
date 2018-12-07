//
// Created by egordm on 2-12-18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <audiofile/writing.h>
#include <audiofile/reading.h>
#include <processing/resample.h>

using namespace litsignal;
using namespace litsignal::processing;
using namespace litaudio::structures;

class LitSignalBaseTests : public ::testing::Test {

};

TEST_F(LitSignalBaseTests, ReadWrite_Test) {
    SignalContainer src;
    AudioReader reader(&src, "data/hangar.mp3");
    ASSERT_TRUE(reader.read());

    AudioWriter writer(&src, "data/output/dst.mp3");
    ASSERT_TRUE(writer.write());
}

TEST_F(LitSignalBaseTests, ReadWriteConvertResample_Test) {
    SignalContainer src;
    AudioReader reader(&src, "data/hangar.mp3");
    ASSERT_TRUE(reader.read());

    SignalContainer dst;
    dst.setSampleRate(src.getSampleRate() / 2);
    SignalResampler resampler(&src, &dst);
    ASSERT_TRUE(resampler.resample());

    AudioWriter writer(&dst, "data/output/dst_signal.mp3");
    ASSERT_TRUE(writer.write());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}