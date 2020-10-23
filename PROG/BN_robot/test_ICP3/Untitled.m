% Generate model points
clear all;
clc;
a = readtable('graphXY0V.csv');
b = table2array(a);
c = b(1,:);
model(1,:) = c(~isinf(c));
c = b(2,:);
model(2,:) = c(~isinf(c));
% Generate data points
a = readtable('graphXY1V.csv');
b = table2array(a);
c = b(1,:);
data(1,:) = c(~isinf(c));
c = b(2,:);
data(2,:) = c(~isinf(c));
% A plot. Model points and data points in start positions
figure(1)
plot(model(1,:),model(2,:),'r.',data(1,:),data(2,:),'b.'), axis equal
% Running the ICP-algorithm. Least squares criterion
[RotMat,TransVec,dataOut]=icp(model,data,100,5,3,1e-10);
RotMat
TransVec
% A plot. Model points and data points in transformed positions
figure(2)
plot(model(1,:),model(2,:),'r.',dataOut(1,:),dataOut(2,:),'b.'), axis equal