//
// Created by egordm on 8-12-18.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <structures/signal_container.h>
#include <audiofile/reading.h>
#include <utils/litsignal_math.h>
#include <utils/pulse_utils.h>
#include <utils/window_utils.h>
#include "test_helpers.h"
#include <processing/phase_vocoder_algorithm.h>
#include <audiofile/simplified.h>
#include <analysis/stft_algorithm.h>
#include <analysis/istft_algorithm.h>

using namespace litsignal::structures;
using namespace litsignal::analysis;
using namespace litsignal::algorithm;
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
    int sample_rate = signal.getSampleRate();
    vec window = window::hanning(window_size);
    std::cout << window.t() << std::endl;
    vec X = signal.get_data_vec().col(0);


    clock_t begin = clock();

    S = calculate_stft(X, feature_rate, t, f, sample_rate, window, hop_size);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Calculated STFT in secs: " << elapsed_secs << std::endl;
}

TEST_F(LitSignalAnalysisTests, SFFT_Correctness_Test) {
    int freq = 10; //600 / 60; // 60 bpm
    int sample_rate = freq * 2; // samples per second. Least needed to store the signal

    vec sine, cosine;
    pulse_utils::generate_pulse(cosine, sine, freq, sample_rate * 10, sample_rate);

    vec t, f;
    int feature_rate;


    int window_size = sample_rate;
    int hop_size = window_size / 2;
    vec window = window::hanning(window_size);
    //vec window = vec(window_size, fill::ones);
    vec X = cosine;

    cx_mat S = calculate_stft(X, feature_rate, t, f, sample_rate, window, hop_size);
    S = S(span::all, span(1, S.n_cols - 2));
    //vec O = calculate_istft(S, window, hop_size);
    //std::cout << O.t() << std::endl;

    mat Sa = abs(S);
    //std::cout << Sa << std::endl;

    ASSERT_DOUBLE_EQ(mean(Sa(Sa.n_rows - 1, span(1, S.n_cols - 2))), sum(window)); // Remove first and last cols
    ASSERT_TRUE(compare_vec(f, regspace(0, sample_rate/2)));
    vec tcomp =regspace<vec>(0, Sa.n_cols - 1) * (window_size / 2 / (double) sample_rate);
    ASSERT_TRUE(compare_vec(t, tcomp));
}

TEST_F(LitSignalAnalysisTests, PhaseVocoder_Performance_Test) {
    vec X = signal.get_data_vec().col(0);
    int sample_rate = signal.getSampleRate();

    clock_t begin = clock();

    auto newSignal = litsignal::processing::calculate_pv(X, sample_rate, 1.8f);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Calculated PhaseVocoder in secs: " << elapsed_secs << std::endl;

    auto container = SignalContainer(newSignal, sample_rate);
    auto ret = litaudiofile::simplified::write_audio("data/output/dst_vocode.wav", &container);
    if(!ret) std::cout << "FAIL!" << std::endl;
}