% Copyright 2018 Toyota Research Institute.  All rights reserved.
% 
% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this software and associated documentation files (the "Software"), to deal
% in the Software without restriction, including without limitation the rights
% to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
% copies of the Software, and to permit persons to whom the Software is
% furnished to do so, subject to the following conditions:
% 
% The above copyright notice and this permission notice shall be included in
% all copies or substantial portions of the Software.
% 
% THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
% IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
% FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
% AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
% LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
% OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
% SOFTWARE.

%% An example on how to use libgp_mex, the mex wrapper for libgp.
% This script provides a very simple example as to how to use libgp_mex. 
% Trains a two-data point GP using libgp, and outputs the result.
% If you wish to compare the output result to gpml (if you have it 
% installed), feel free to uncomment the last section.

clc
close all
clear all

%% Compilation (need to do this before running this script)

% libgp_mex         % creates mexed libgp
% startup           % initializes gpml

%% Initialize

% Adds the paths in case it wasn't added.
dir = fileparts(mfilename('fullpath'));
addpath([dir, '/../matlab/']);
addpath([dir, '/../build/']);
gpmex = mex_interface(str2fun([dir '/../build/libgp_mex']));

% Specify input parameters.
cov = 'CovSum( CovSEiso, CovNoise)';
log_ell = 1.2;
log_sigma_f = 4.1;
log_lik = -2;

input_dim = 4;
num_train = 2;
num_test =  2;

X_train = [1, 2, 3, 4;
           4, 5, 6, 7];
X_test = X_train;
Y = [10; 22];

disp(['Number of training points: ', num2str(num_train), ', Number of test points: ', num2str(num_test)]);

%% Run libgp

gpmex.initialize(input_dim, cov, [log_ell, log_sigma_f, log_lik]);
gpmex.train( X_train', Y );
[libgp_pred, libgp_var] = gpmex.predictWithVariance(X_test');       
disp('libgp prediction:');
disp(libgp_pred')
disp('libgp variance:');
disp(libgp_var')

%% Run gpml: Uncomment below if you wish to compare the output of gpml using the same parameters

% cf_options = struct('meanfcn', [], 'likfcn', @likGauss, 'inffcn', @infGaussLik, ...
%                   'hyp', struct('mean', [], 'cov', [log_ell, log_sigma_f, log_lik], 'lik', log_lik));
% covfcn = {'covSum', {'covSEiso', 'covNoise'}};
% [gpml_pred, gpml_sd, ~, ~] = gp(cf_options.hyp, cf_options.inffcn, cf_options.meanfcn, covfcn, cf_options.likfcn, X_train, Y, X_test);
% disp('gpml prediction:');
% disp(gpml_pred)
% disp('gpml stddev:');
% disp(gpml_sd)
