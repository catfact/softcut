//
// Created by ezra on 11/8/18.
//
// lowpass filter, 6th order butterworth
//
// hardcoded for given cutoff and sample rate.

#ifndef SOFTCUT_LOWPASS_BUTTER_6_H
#define SOFTCUT_LOWPASS_BUTTER_6_H

namespace softcut {

	class Lowpass_butter_6 {
		static constexpr float b[7] = { 0.0296, 0.1775, 0.4438, 0.5918, 0.4438, 0.1775, 0.0296 };
		static constexpr float a[7] = { 1.0000, -0.0000, 0.7777, -0.0000, 0.1142, -0.0000, 0.0018 };



		float process(float x) {

	}

}
#endif //SOFTCUT_LOWPASS_BUTTER_6_H
