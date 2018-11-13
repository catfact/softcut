n = NetAddr("localhost", 9999);

// change this to norns address for remote test
//n = NetAddr("192.168.1.11", 9999);

// echo
(
n.sendMsg("/set/ampLeft", 0, 1.0);

n.sendMsg("/set/ampRight", 0, 0.4);

n.sendMsg("/set/loopStart", 0, 1.0);
n.sendMsg("/set/loopEnd", 0, 2.0);
n.sendMsg("/set/loopFlag", 0, 1.0);
n.sendMsg("/set/rate", 0, 1.0);

n.sendMsg("/set/recLevel", 0, 1.0);
n.sendMsg("/set/preLevel", 0, 0.7);

n.sendMsg("/set/recFlag", 0, 1);

n.sendMsg("/set/recOffset", 0, 50);
n.sendMsg("/set/position", 0, 1.0);

n.sendMsg("/set/fadeTime", 0, 0.125);
)

// voice 2
(
n.sendMsg("/set/ampLeft", 1, 0.5);

n.sendMsg("/set/ampRight", 1, 1.0);

n.sendMsg("/set/loopStart", 1, 2.0);
n.sendMsg("/set/loopEnd", 1, 5.5);
n.sendMsg("/set/loopFlag", 1, 1.0);
n.sendMsg("/set/rate", 1, 1.2);

n.sendMsg("/set/fadeTime", 1, 0.2);
n.sendMsg("/set/recLevel", 1, 0.5);
n.sendMsg("/set/preLevel", 1, 0.2);

n.sendMsg("/set/recFlag", 1, 1);

n.sendMsg("/set/recOffset", 1, 50);
n.sendMsg("/set/position", 1, 2.0);
)

/// thingsss
n.sendMsg("/set/rate", 0, 0.47);
n.sendMsg("/set/rate", 0, -0.47);

n.sendMsg("/set/rate", 0, 5/8);
n.sendMsg("/set/rate", 0, 5/8 + 0.01);
n.sendMsg("/set/rate", 0, -3/2);
n.sendMsg("/set/rate", 0, 3/8);
n.sendMsg("/set/rate", 0, 8/5);

n.sendMsg("/set/rate", 0, -1);

n.sendMsg("/set/rate", 0, -6);
n.sendMsg("/set/rate", 0, 8);
n.sendMsg("/set/rate", 0, 5.5);
n.sendMsg("/set/rate", 0, -3/2);
n.sendMsg("/set/rate", 0, 1/8);
n.sendMsg("/set/rate", 0, -1/8);


n.sendMsg("/set/rate", 0, -4.7);


// short loop
n.sendMsg("/set/loopEnd", 0, 1.25);
n.sendMsg("/set/fadeTime", 0, 0.06);

n.sendMsg("/set/recLevel", 0, 1.0);
n.sendMsg("/set/preLevel", 0, 0.9);
n.sendMsg("/set/preLevel", 0, 0.0);

n.sendMsg("/set/recLevel", 0, 0);
n.sendMsg("/set/preLevel", 0, 1.0);
n.sendMsg("/set/position", 0, 0.0);

// long loop, long fade
n.sendMsg("/set/loopEnd", 0, 5.0);
n.sendMsg("/set/fadeTime", 0, 2.0);


/// resonaturr
n.sendMsg("/set/loopStart", 0, 1.0);
n.sendMsg("/set/loopEnd", 0, 1.0 + 1/110);
n.sendMsg("/set/fadeTime", 0, 1/220);
n.sendMsg("/set/preLevel", 0, 0.977);

// randomize rate
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


		n.sendMsg("/set/rate", 0, rate);
	}
}.play;
)

// randomize loop duration too rarrh
(
~mindur = 1.0;
q = Routine {
	var dur, fade;
	inf.do {
		(~mindur + (1.rand)).wait;
		dur = ~mindur + ~mindur.rand;
		fade = dur * (0.1 + 0.5.rand);
		postln([dur, fade]);
		if(fade > dur, { postln("!!!!!!!!!!!!!!!"); });
		n.sendMsg("/set/loopEnd", 0, 1 + dur);
		n.sendMsg("/set/fadeTime", 0, fade);
	}
}.play;
)


// randomly cut to position
(
~mindur = 0.5;
p = Routine {
    var pos;
    inf.do {
		(~mindur + ~mindur.rand).wait;
		pos = 1.2 + 0.4.rand;
		postln("pos " ++pos);
		n.sendMsg("/set/position", 0, pos);
    }
}.play;
)

// real flappy
~mindur = 0.05;

// less flappy
~mindur = 0.25;

// freeze
n.sendMsg("/set/recFlag", 0, 0);

// input filters
n.sendMsg("/set/filterFc", 0, 880 * 4);
n.sendMsg("/set/filterRq", 0, 0.7);
n.sendMsg("/set/filterLp", 0, 0.8);
n.sendMsg("/set/filterBp", 0, 0.0);

// different!
n.sendMsg("/set/filterFc", 0, 550);
n.sendMsg("/set/filterLp", 0, 0.0);
n.sendMsg("/set/filterHp", 0, 0.8);
n.sendMsg("/set/filterRq", 0, 0.02);

n.sendMsg("/set/filterLp", 0, 0.8);
n.sendMsg("/set/filterHp", 0, 0.0);

n.sendMsg("/set/filterFcMod", 0, 0);
n.sendMsg("/set/filterDry", 0, 0);
n.sendMsg("/set/filterFc", 0, 330);

n.sendMsg("/set/filterFcMod", 0, 0.5);
n.sendMsg("/set/filterFcMod", 0, 1.0);

/*
r.stop;
q.stop;
p.stop;


n.sendMsg("/quit");
*/