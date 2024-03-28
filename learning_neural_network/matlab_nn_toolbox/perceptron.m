% 2 input perceptron
clear;
% input training data
P = [-0.5 -0.5 +0.3 +0.0; -0.5 +0.5 -0.5 +1.0];
% expectation training data
T = [1 1 0 0];

plotpv(P, T)
% initialize the network 
net = newp([-1 1; -1 1],1);

% train the network
net = train(net, P,T);

% plot the separation line
plotpc(net.IW{1},net.b{1})

% classify a unkown vector
p = [-0.5; 0.5];
a = sim(net,p)

% this should be classified as 0
p = [0.3; -0.5];
a = sim(net,p)

% 3 input perceptron
clear;
% input training data
P = [-1 +1 -1 +1 -1 +1 -1 +1; -1 -1 +1 +1 -1 -1 +1 +1; -1 -1 -1 -1 +1 +1 +1 +1];
% expectation training data
T = [0 1 0 0 1 1 0 1];

plotpv(P,T)

% initialize the network
net = newp([-1 1; -1 1; -1 1],1);

% train the network
net.trainParam.goal = 0.01;
net = train(net,P,T);

% plot initial network
plotpv(P,T)
plotpc(net.IW{1},net.b{1})

% try a new vector should be classified as 0
p = [0.7; 1.2; -0.2];
a = sim(net,p)

% try a new vector should be classified as 0
p = [-1; +1; -1];
a = sim(net,p)

% 2 neuron perceptron

% input training data
P = [+0.1 +0.7 +0.8 +0.8 +1.0 +0.3 +0.0 -0.3 -0.5 -1.5; +1.2 +1.8 +1.6 +0.6 +0.8 +0.5 +0.2 +0.8 -1.5 -1.3];

% expectation training data
T = [1 1 1 0 0 1 1 1 0 0; 0 0 0 0 0 1 1 1 1 1];

plotpv(P,T)

% initialize the network
net = newp([-2 2; -2 2],2);

% train the network
net.trainParam.goal = 0.01;
net = train(net,P,T);


% this should be classified as 1 0
p = [0.1; 1.2];

a = sim(net, p)

% try a new vector this should classified as 1 0
p = [0.7; 1.2];
a = sim(net, p)

% try a new vector it should be 0 1
p = [1.7; 1.2];
a = sim(net,p)

% try a new vector it should be 0 1 but is 1 1 for [-1.5; -1.2] is correct
p = [-1.7; -1.2];
a = sim(net,p)

% 2 layer perceptron (not working as expected)
clear;

% input training data
P = [-0.5 -0.5 +0.3 -0.1 -0.8; -0.5 +0.5 -0.5 +1.0 +0.0];

% expectation training data
T = [1 1 0 0 0];


% Preprocessing layer

net1 = newp([-1 1; -1 1],20);
net1.layers(1).initFcn='initwb'
net1.inputWeights{1,1}.initFcn = 'rands'
net1.layerWeights{1,1}.initFcn = 'rands'
net1.biases{1}.initFcn = 'rands'
net1 = init(net1);

% Learning layer
net = newp(repmat([-1 1], 20, 1), 1);

% train first layer
A1 = sim(net1, P);

% train the second layer
net.trainParam.goal = 0.01;
net = train(net,A1,T);

% try
p = [0.7; 1.2];
a1 = sim(net1, p); % Preprocess the vector
a2 = sim(net,a1) % Classify the vector
