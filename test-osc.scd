n = NetAddr("127.0.0.1", 9999);

n.sendMsg("/set/rate", 1.0);
n.sendMsg("/set/loopStart", 1.0);
n.sendMsg("/set/loopEnd", 2.0);
n.sendMsg("/set/loopFlag", 1.0);
n.sendMsg("/set/fadeTime", 0.2);
n.sendMsg("/set/recLevel", 1.0);
n.sendMsg("/set/preLevel", 0.5);
n.sendMsg("/set/recFlag", 1.0);
n.sendMsg("/set/recOffset", -10);
n.sendMsg("/set/position", 1.0);
n.sendMsg("/quit");

n.sendMsg("/set/rate", -2.0);