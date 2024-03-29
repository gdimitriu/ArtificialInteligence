% perceptron AND
clear;
% input training vector
P =[1 1 0 0; 1 0 1 0];

% expectation vector
T = [1 0 0 0];

% init network
net = newp([0 1; 0 1], 1);

% train network
net = train(net, P, T);

% plot the decision zone
plotpv(P, T)
plotpc(net.IW{1},net.b{1})

%vector test
p =[0; 0];
sim(net,p)

% perceptron OR
clear;
% input training vector
P =[1 1 0 0; 1 0 1 0];

% expectation vector
T = [1 1 1 0];

% init network
net = newp([0 1; 0 1], 1);

% train network
net = train(net, P, T);

% plot the decision zone
plotpv(P, T)
plotpc(net.IW{1},net.b{1})

%vector test
p =[0; 0];
sim(net,p)

% perceptron XOR
clear;
% input training vector
P =[1 1 0 0; 1 0 1 0];

% expectation vector
T = [0 1 1 0];

% init network
net = newp([0 1; 0 1], 1);

% train network
net = train(net, P, T);

% plot the decision zone
plotpv(P, T)
plotpc(net.IW{1},net.b{1})

%vector test
p =[0; 0];
sim(net,p)
