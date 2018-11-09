n = NetAddr("localhost", 9999);

// change this to norns address for remote test
//n = NetAddr("192.168.1.11", 9999);

// echo
(
n.sendMsg("/set/loopStart", 1.0);
n.sendMsg("/set/loopEnd", 2.0);
n.sendMsg("/set/loopFlag", 1.0);
n.sendMsg("/set/rate", 1.0);

n.sendMsg("/set/recLevel", 1.0);
n.sendMsg("/set/preLevel", 0.5);

n.sendMsg("/set/recFlag", 1);

n.sendMsg("/set/recOffset", 50);
n.sendMsg("/set/position", 1.0);
)

/// thingsss
n.sendMsg("/set/rate", 0.47);
n.sendMsg("/set/rate", -0.47);

n.sendMsg("/set/rate", 5/8);
n.sendMsg("/set/rate", 5/8 + 0.01);
n.sendMsg("/set/rate", -3/2);
n.sendMsg("/set/rate", 3/8);
n.sendMsg("/set/rate", 8/5);

n.sendMsg("/set/rate", -1);

n.sendMsg("/set/rate", -6);
n.sendMsg("/set/rate", 15);
n.sendMsg("/set/rate", 3/2);
n.sendMsg("/set/rate", 1/8);
n.sendMsg("/set/rate", -1/8);


// short loop
n.sendMsg("/set/loopEnd", 1.25);
n.sendMsg("/set/fadeTime", 0.2);


n.sendMsg("/set/recLevel", 1.0);
n.sendMsg("/set/preLevel", 0.9);
n.sendMsg("/set/preLevel", 0.0);

n.sendMsg("/set/recFlag", 0);
n.sendMsg("/set/position", 0.0);

// long loop, long fade
n.sendMsg("/set/loopEnd", 5.0);
n.sendMsg("/set/fadeTime", 2.0);


// flappy
(
~mindur = 1.0;
r = Routine {
	var rate;
	inf.do {
		(~mindur + (1.rand)).wait;
		if(0.5.coin, {
			rate = [-1, 1].choose;
			postln("rate: "++rate);
		}, {
			// if you want to find some worst cases:
			/*
			var rate = 4.0.rand2;
			*/
			var num = 1 + 5.rand;
			var den = 1 + 5.rand;
			var sgn = [-1, 1].choose;
			rate = num/den * sgn;
			postln("rate: " ++ if(sgn<0, {"-"},{""}) ++ num ++ "/" ++ den);
		});


		n.sendMsg("/set/rate", rate);
	}
}.play;
)

// randomize loop duration too rarrh
(
q = Routine {
	var dur;
	inf.do {
		(~mindur + (1.rand)).wait;
		if(0.5.coin, {
			dur = ~mindur + ~mindur.rand;
			postln("dur " ++dur);
			n.sendMsg("/set/loopEnd", 1 + dur);
			n.sendMsg("/set/fadeTime", dur * (0.1  + 0.8.rand));
		});
	}
}.play;
)

// real flappy
~mindur = 0.05;

// less flappy
~mindur = 0.25;

// freeze
n.sendMsg("/set/recFlag", 0);

// input filters
n.sendMsg("/set/filterFc", 880 * 4);
n.sendMsg("/set/filterRq", 0.25);
n.sendMsg("/set/filterLp", 0.8);
n.sendMsg("/set/filterBp", 0.85);
// different!
n.sendMsg("/set/filterFc", 550);
n.sendMsg("/set/filterLp", 0.0);
n.sendMsg("/set/filterHp", 0.8);
n.sendMsg("/set/filterRq", 0.01);

n.sendMsg("/set/filterLp", 0.8);
n.sendMsg("/set/filterHp", 0.0);

n.sendMsg("/set/filterFcMod", 0);
n.sendMsg("/set/filterDry", 0);
n.sendMsg("/set/filterFc", 330);

n.sendMsg("/set/filterFcMod", 0.5);
/*
r.stop;
q.stop;

n.sendMsg("/quit");
*/