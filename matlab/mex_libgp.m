clc
close all
clear all
path1 = ['-I' fullfile(pwd,'../include')];
path2 = ['-I' fullfile(pwd,'../external')];
path3 = ['-I' fullfile(pwd,'../libgp/include')];
path4 = ['-I' fullfile(pwd,'../libgp_interface/include')];
path5 = ['-I' fullfile('/usr/include/eigen3')];

mex('-output', '../build/libgp_mex', 'COPTIMFLAGS="-Ofast"', path1, path2, path3, path4, path5,...
  '../src/libgpmex_interface.cc',...
  '../libgp/src/cov.cc',...
  '../libgp/src/cov_factory.cc',...
  '../libgp/src/cov_linear_ard.cc',...
  '../libgp/src/cov_linear_one.cc',...
  '../libgp/src/cov_matern3_iso.cc',...
  '../libgp/src/cov_matern5_iso.cc',...
  '../libgp/src/cov_noise.cc',...
  '../libgp/src/cov_rq_iso.cc',...
  '../libgp/src/cov_periodic_matern3_iso.cc',...
  '../libgp/src/cov_periodic.cc',...
  '../libgp/src/cov_se_ard.cc',...
  '../libgp/src/cov_se_iso.cc',...
  '../libgp/src/cov_sum.cc',...
  '../libgp/src/cov_prod.cc',...
  '../libgp/src/gp.cc',...
  '../libgp/src/gp_utils.cc',...
  '../libgp/src/sampleset.cc',...
  '../libgp/src/rprop.cc',...
  '../libgp/src/input_dim_filter.cc',...
  '../libgp/src/cg.cc',...
  '../libgp_interface/src/libgp_interface.cc');
