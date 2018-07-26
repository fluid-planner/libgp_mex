%% Timing comparison between gpml (matlab) vs. libgp_mex (mexed c++)
% Generates a timing plot for various numbers of training data for gpml and
% libgp. Must have gpml installed.

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

num_train_vec = [1, 10, 100, 500, 1000, 1500, 2000, 3000, 4000, 5000];
num_test_vec = [1, 10, 100, 1000];
input_dim = 6;

%% Run timing analysis

T_libgp = [];
T_gpml = [];

for i = 1:length(num_train_vec)
    for j = 1:length(num_test_vec)

        num_train = num_train_vec(i);
        num_test =  num_test_vec(j);
        disp(['Number of training points: ', num2str(num_train), ', Number of test points: ', num2str(num_test)]);

        X_train = randi(10,num_train,input_dim);
        X_test = randi(10,num_test,input_dim);
        Y = randi(50, num_train, 1);

        % Run libgp.
        gpmex.initialize(input_dim, cov, [log_ell, log_sigma_f, log_lik]); % Initializing libgp_mex object      
        ts = tic;
        gpmex.train( X_train', Y);                                         % Train libgp.
        t_libgp_train = toc(ts);
        ts = tic;
        libgp_pred = gpmex.predict(X_test');                               % Predict without variance (faster)
        % [libgp_pred, libgp_var] = gpmex.predictWithVariance(X_test');     % Predict with variance (slower)
        t_libgp_predict = toc(ts);

        % Run gpml.
        cf_options = struct('meanfcn', [], 'likfcn', @likGauss, 'inffcn', @infGaussLik, ...
                            'hyp', struct('mean', [], 'cov', [log_ell, log_sigma_f, log_lik], 'lik', log_lik));
        covfcn = {'covSum', {'covSEiso', 'covNoise'}};
        ts = tic;
        [gpml_pred, gpml_sd, ~, ~] = gp(cf_options.hyp, cf_options.inffcn, cf_options.meanfcn, covfcn, cf_options.likfcn, X_train, Y, X_test);
        t_gpml = toc(ts);

        % Display results and add to the timing matrix.
        disp(['Timing: libgp train+test: ', num2str(t_libgp_train), ' + ', num2str(t_libgp_predict), ' = ', num2str(t_libgp_train + t_libgp_predict)]);
        disp(['Timing: gpml  train+test: ', num2str(t_gpml)]);

        T_libgp(i, j) = t_libgp_train + t_libgp_predict;
        T_gpml(i, j) = t_gpml;

    end
end

%% Plot

co = get(gca,'colororder');
figure(1); hold on; grid on;
title({['Timing comparison between gpml (matlab) and libgp (mexed c++) for input dimension ', num2str(input_dim)],...
    'dashed line: libgp. solid line: gpml'});
xlabel('Number of training points'); ylabel('Time for train + test (s)');

lgnd = {};
handles = [];
for j = 1:length(num_test_vec)
    h = plot(num_train_vec, T_libgp(:, j), '.--', 'Color', co(j, :));
    plot(num_train_vec, T_gpml(:, j), '.-', 'Color', co(j, :));
    handles(end+1) = h;
    lgnd{end+1} = [num2str(num_test_vec(j)), ' test pt(s)'];
end
legend(handles, lgnd);
