//
// Created by egordm on 8-12-18.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <structures/signal_container.h>
#include <audiofile/reading.h>
#include <analysis/fourier_analysis.h>
#include <utils/litsignal_math.h>
#include <analysis/stft_processor.h>

using namespace litsignal::structures;
using namespace litsignal::analysis;
using namespace litaudiofile;

class LitSignalAnalysisTests : public ::testing::Test {
protected:
    SignalContainer signal;

    void SetUp() override {
        AudioReader reader(&signal, "data/hangar.mp3");
        if(!reader.read()) throw std::runtime_error("Could load the test file.");
    }
};

TEST_F(LitSignalAnalysisTests, SFFT_Test) {
    int feature_rate;
    vec t, f;
    cx_mat S;

    int window_size = static_cast<int>(1024 * signal.getSampleRate() / 22050.);
    int hop_size = window_size / 2;
    vec window = lit::math::my_hanning(window_size);
    vec X = signal.get_data_vec().col(0);

    clock_t begin = clock();

    fourier_analysis::STFTProcessor stft_processor(X, S, feature_rate, t, f, signal.getSampleRate(), window, hop_size);
    stft_processor.process();

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Calculated STFT in secs: " << elapsed_secs << std::endl;

    int i = 0;
}