//
// Created by egordm on 2-12-18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <audiofile/writing.h>
#include <audiofile/reading.h>
#include <audiofile/tag_reader.h>

using namespace litaudiofile;
using namespace litaudio::structures;

class LitAudioFileBaseTests : public ::testing::Test {

};

TEST_F(LitAudioFileBaseTests, ReadWriteConvertResample_Test) {
    auto src = new AbstractAudioContainer(new structures::AudioBufferDeinterleaved<uint8_t>(-1, 0, av_get_bytes_per_sample(AV_SAMPLE_FMT_S16P)), AV_SAMPLE_FMT_S16P);
    auto reader = AudioReader(src, "data/hangar.mp3");
    ASSERT_TRUE(reader.read());

    auto dst = new AbstractAudioContainer(new structures::AudioBufferDeinterleaved<uint8_t>(-1, 0, av_get_bytes_per_sample(AV_SAMPLE_FMT_S16P)), AV_SAMPLE_FMT_S16P);
    dst->copyFormat(src);
    dst->setSampleRate(src->getSampleRate() / 2);
    auto converter = processing::AudioConverter(src, dst);
    ASSERT_TRUE(converter.convert());

    auto writer = AudioWriter(dst, "data/output/dst.mp3");
    ASSERT_TRUE(writer.write());

}

TEST_F(LitAudioFileBaseTests, ReadTag_Test) {
    TagReader reader("data/test.flac");
    auto tags = reader.getTags();

    std::string artist(tags->artist().toCString());
    std::string title(tags->title().toCString());
    std::string album(tags->album().toCString());

    std::cout << artist << std::endl << title << std::endl << album << std::endl;

    ASSERT_TRUE(artist.compare("Test1") == 0);
    ASSERT_TRUE(title.compare("Test2") == 0);
    ASSERT_TRUE(album.compare("Test3") == 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}