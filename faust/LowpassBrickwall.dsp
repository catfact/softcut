declare name "BrickwallLowpass";

import("stdfaust.lib");

process = fi.lowpass(6, 16000);