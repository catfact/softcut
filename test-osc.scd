n = NetAddr("localhost", 9999);

// change this to norns address for remote test
//n = NetAddr("192.168.1.11", 9999);

// echo
(
n.sendMsg("/set/loopStart", 1.0);
n.sendMsg("/set/loopEnd", 2.0);
n.sendMsg("/set/loopFlag", 1.0);
n.sendMsg("/set/rate", 1.0);

n.sendMsg("/set/recLevel", 0.5);
n.sendMsg("/set/preLevel", 0.5);
n.sendMsg("/set/recFlag", 1);
n.sendMsg("/set/recOffset", 50);
n.sendMsg("/set/position", 1.0);
)

/// thingsss

n.sendMsg("/set/rate", -0.42);
n.sendMsg("/set/rate", 5/8 + 0.01);
n.sendMsg("/set/rate", 5/8);
n.sendMsg("/set/rate", -3/2);


n.sendMsg("/set/rate", 5/8);
n.sendMsg("/set/rate", 8/5);
n.sendMsg("/set/rate", -1);



n.sendMsg("/set/rate", -6);
n.sendMsg("/set/rate", 3/2);


// short loop
n.sendMsg("/set/loopEnd", 1.2);
n.sendMsg("/set/fadeTime", 0.1);


n.sendMsg("/set/recLevel", 0.8);
n.sendMsg("/set/preLevel", 0.9);

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

/*
r.stop;
q.stop;

n.sendMsg("/quit");
*/