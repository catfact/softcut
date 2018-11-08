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
n.sendMsg("/set/recOffset", -10);
n.sendMsg("/set/position", 1.0);
)

n.sendMsg("/set/rate", 1/8);
n.sendMsg("/set/rate", 5/8 + 0.01);
n.sendMsg("/set/rate", 3/2);


n.sendMsg("/set/recLevel", 0.5);
n.sendMsg("/set/preLevel", 1.0);


// flappy
(
r = Routine {
	var rate;
	inf.do {
		(1 + (1.rand)).wait;
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


/*
r.stop;

n.sendMsg("/quit");
*/