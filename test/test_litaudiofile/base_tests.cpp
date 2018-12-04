//
// Created by egordm on 2-12-18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <audiofile/writing.h>
#include <audiofile/reading.h>

using namespace litaudiofile;
using namespace litaudio::structures;

class LitAudioFileBaseTests : public ::testing::Test {

};

TEST_F(LitAudioFileBaseTests, ReadWriteConvertResample_Test) {
    auto src = new AudioContainer<uint16_t>(AV_SAMPLE_FMT_S16);
    auto reader = AudioReader<uint16_t>(src, "data/hangar.mp3");
    ASSERT_TRUE(reader.read());

    auto dst = src->clone<uint16_t>();
    dst->setSampleRate(src->getSampleRate() / 2);
    auto converter = processing::AudioConverter(src, dst);
    ASSERT_TRUE(converter.convert());

    auto writer = AudioWriter(dst, "data/output/dst.mp3");
    ASSERT_TRUE(writer.write());

}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}