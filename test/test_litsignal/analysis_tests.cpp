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
#include <utils/pulse_utils.h>
#include "test_helpers.h"

using namespace litsignal::structures;
using namespace litsignal::analysis;
using namespace litsignal;
using namespace litaudiofile;

class LitSignalAnalysisTests : public ::testing::Test {
protected:
    SignalContainer signal;

    void SetUp() override {
        AudioReader reader(&signal, "data/hangar.mp3");
        if(!reader.read()) throw std::runtime_error("Could load the test file.");
    }
};

TEST_F(LitSignalAnalysisTests, SFFT_Performace_Test) {
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
}

TEST_F(LitSignalAnalysisTests, SFFT_Correctness_Test) {
    int freq = 10; //600 / 60; // 60 bpm
    int sample_rate = freq * 2; // samples per second. Least needed to store the signal

    vec sine, cosine;
    pulse_utils::generate_pulse(sine, cosine, freq, sample_rate * 10, sample_rate);

    vec t, f;
    int feature_rate;
    cx_mat S;

    int window_size = sample_rate;
    int hop_size = window_size / 2;
    vec window = lit::math::my_hanning(window_size);
    vec X = sine;

    fourier_analysis::STFTProcessor stft_processor(X, S, feature_rate, t, f, sample_rate, window, hop_size);
    stft_processor.process();

    mat Sa = abs(S);
    ASSERT_DOUBLE_EQ(mean(Sa(Sa.n_rows - 1, span(1, S.n_cols - 1))), sum(window));
    ASSERT_TRUE(compare_vec(f, regspace(0, sample_rate/2)));
    vec tcomp =regspace<vec>(0, Sa.n_cols - 1) * (window_size / 2 / (double) sample_rate);
    ASSERT_TRUE(compare_vec(t, tcomp));
}