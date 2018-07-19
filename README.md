## libgp_mex: A wrapper for mblum's libgp
Author: Xuning Yang <xuning.yang@tri.global>

This package contains a mex wrapper for the [libgp](https://github.com/mblum/libgp) package, so that you can use it in MATLAB.

### Install
Clone this repository with all the submodules:

    git clone --recurse-submodules git@address/to/libgp_mex.git

Follow the standard CMake method of building:

    mkdir build; cd build
    cmake ..
    make

You can also build it in MATLAB by running `matlab/mex_libgp.m`

Run `matlab/addpath_libgp.m` to add the library paths to MATLAB.

### Testing the build

Run `test/run_libgp.m`.

To compare the timing of libgp to [gpml](http://www.gaussianprocess.org/gpml/code/matlab/doc/), install gpml and run `test/timing.m`.

### Usage
An example is given in `test/run_libgp.m`.

The mex interface exposes the following function handles:

    gpmex.initialize(input_dim, cov, hyp)

where `input_dim` is the input dimension, `cov` is the covariance function specified in string, `hyp` is a vector of the hyperparameters associated with cov.

    gpmex.train(X_train, y_train)

where `X_train` is a matrix of size [input_dim, num_samples], and `y_train` is a vector of length num_samples.

    [y_pred, y_var] = gpmex.predictWithVariance(X_test)

where `X_test` is a matrix of size [input_dim, num_samples]

    y_pred = gpmex.predict(X_test)
same as above, but does not compute the variance (faster than above).

    gpmex.clear()
clears all training data.

    gpmex.getNumSamples()
outputs the number of training samples currently in the gp.

### Misc
Tested with Ubuntu 16.04 and MATLAB R2016b.

In certain occasions the system libstdc++ is not matched with the MATLAB version's, with an error message similar to:

    /usr/local/MATLAB/<version>/bin/glnxa64/../../sys/os/glnxa64/libstdc++.so.6:
    version `GLIBCXX_3.4.21' not found (required by /path/to/mex_libgp/build/mex_libgp.mexa64).

To fix, make a symbolic link to the system libstdc++.so:

    mv /usr/local/MATLAB/<version>/sys/os/glnxa64/libstdc++.so.6 /usr/local/MATLAB/<version>/sys/os/glnxa64/libstdc++.so.6_old
    sudo ln -s /usr/lib/x86_64-linux-gnu/libstdc++.so.6 /usr/local/MATLAB/<version>/sys/os/glnxa64/libstdc++.so.6

