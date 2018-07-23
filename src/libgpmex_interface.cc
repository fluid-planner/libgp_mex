#include <libgpmex_interface.h>

#define TRY(func, code) if (func != 0) { return code; }

namespace mu = mex_utils;

LibgpMexInterface::LibgpMexInterface() {}
LibgpMexInterface::~LibgpMexInterface() {}

LibgpMexInterface::ReturnStatus
LibgpMexInterface::FunctionCall(int nlhs, mxArray *plhs[], int nrhs,
                                const mxArray *prhs[]) {

  int fcnlen = 128;
  char fcnname[fcnlen];
  mxGetString(prhs[0], fcnname, fcnlen);

  if (!strcmp(fcnname, "initialize"))
    return CallInitialize(nlhs, plhs, nrhs, prhs);
  else if (!strcmp(fcnname, "train"))
    return CallTrain(nlhs, plhs, nrhs, prhs);
  else if (!strcmp(fcnname, "predict"))
    return CallPredict(nlhs, plhs, nrhs, prhs);
  else if (!strcmp(fcnname, "predictWithVariance"))
    return CallPredictWithVariance(nlhs, plhs, nrhs, prhs);
  else if (!strcmp(fcnname, "clear"))
    return CallClear(nlhs, plhs, nrhs, prhs);
  else if (!strcmp(fcnname, "getNumSamples"))
    return CallGetNumSamples(nlhs, plhs, nrhs, prhs);
  else {
    char warn_msg[128];
    sprintf(warn_msg, "Function '%s' not found", fcnname);
    mexWarnMsgTxt(warn_msg);
    return ERROR_UNHANDLED;
  }

  return ERROR_FAILED;
}

LibgpMexInterface::ReturnStatus
LibgpMexInterface::CallInitialize(int nlhs, mxArray *plhs[], int nrhs,
                                  const mxArray *prhs[]) {
  TRY(mu::CheckInput(nrhs, 5, "initialize"), ERROR_INPUT);
  TRY(mu::CheckInputType(prhs[2], mu::SCALAR), ERROR_INPUT);
  TRY(mu::CheckInputType(prhs[3], mu::STRING), ERROR_INPUT);

  unsigned int dim = *mxGetPr(prhs[2]);
  std::string cov_kernel = mu::GetStdString(prhs[3]);

  std::vector<double> hyp_params;
  if (mu::MxArray2Vec(prhs[4], &hyp_params) != SUCCESS)
    return ERROR_MATRIX;

  libgp_.Initialize(dim, cov_kernel, hyp_params);
  return SUCCESS;
}

LibgpMexInterface::ReturnStatus
LibgpMexInterface::CallTrain(int nlhs, mxArray *plhs[], int nrhs,
                             const mxArray *prhs[]) {
  TRY(mu::CheckInput(nrhs, 4, "train"), ERROR_INPUT);

  // Check the X matrix is of the correct size.
  int nrows = mxGetM(prhs[2]);
  int ncols = mxGetN(prhs[2]);

  // Check the Y vector is a 1d vector.
  int num_samples = std::max(mxGetM(prhs[3]), mxGetN(prhs[3]));
  assert(std::min(mxGetM(prhs[3]), mxGetN(prhs[3])) == 1);

  int input_dim = libgp_.GetInputDim();

  if (nrows != input_dim) {
    char warn_msg[128];
    sprintf(warn_msg,
            "Data dimension %i does not match the specified data dimension %i.",
            nrows, input_dim);
    mexWarnMsgTxt(warn_msg);
    return ERROR_MATRIX_SIZE;
  }

  if (ncols != num_samples) {
    char warn_msg[128];
    sprintf(warn_msg, "Number of train data X (%i) does not match the number "
                      "of train data Y (%i).",
            ncols, num_samples);
    mexWarnMsgTxt(warn_msg);
    return ERROR_MATRIX_SIZE;
  }

  std::vector<double> x;
  if (mu::MxArray2Mat(prhs[2], &x) != SUCCESS)
    return ERROR_MATRIX;

  std::vector<double> y;
  if (mu::MxArray2Vec(prhs[3], &y) != SUCCESS)
    return ERROR_MATRIX;

  libgp_.Train(x, y);
  return SUCCESS;
}

LibgpMexInterface::ReturnStatus
LibgpMexInterface::CallPredictWithVariance(int nlhs, mxArray *plhs[], int nrhs,
                                           const mxArray *prhs[]) {

  TRY(mu::CheckInput(nrhs, 3, "predictWithVariance"), ERROR_INPUT);

  // Check the X matrix is of the correct size.
  int nrows = mxGetM(prhs[2]);
  int ncols = mxGetN(prhs[2]);

  int input_dim = libgp_.GetInputDim();
  int num_samples = ncols;

  if (nrows != input_dim) {
    char warn_msg[128];
    sprintf(warn_msg,
            "Data dimension %i does not match the specified data dimension %i.",
            nrows, input_dim);
    mexWarnMsgTxt(warn_msg);
    return ERROR_MATRIX_SIZE;
  }

  std::vector<double> x;
  if (mu::MxArray2Mat(prhs[2], &x) != SUCCESS)
    return ERROR_MATRIX;

  std::vector<double> y_pred;
  std::vector<double> y_var;

  libgp_.Predict(num_samples, x, &y_pred, &y_var);

  plhs[0] = mu::Vec2MxArray(y_pred);
  plhs[1] = mu::Vec2MxArray(y_var);

  return SUCCESS;
}

LibgpMexInterface::ReturnStatus
LibgpMexInterface::CallPredict(int nlhs, mxArray *plhs[], int nrhs,
                               const mxArray *prhs[]) {

  TRY(mu::CheckInput(nrhs, 3, "predict"), ERROR_INPUT);

  // Check the X matrix is of the correct size.
  int nrows = mxGetM(prhs[2]);
  int ncols = mxGetN(prhs[2]);

  int input_dim = libgp_.GetInputDim();
  int num_samples = ncols;

  if (nrows != input_dim) {
    char warn_msg[128];
    sprintf(warn_msg,
            "Data dimension %i does not match the specified data dimension %i.",
            nrows, input_dim);
    mexWarnMsgTxt(warn_msg);
    return ERROR_MATRIX_SIZE;
  }

  std::vector<double> x;
  if (mu::MxArray2Mat(prhs[2], &x) != SUCCESS)
    return ERROR_MATRIX;

  std::vector<double> y_pred;
  libgp_.Predict(num_samples, x, &y_pred);

  plhs[0] = mu::Vec2MxArray(y_pred);

  return SUCCESS;
}

LibgpMexInterface::ReturnStatus
LibgpMexInterface::CallClear(int nlhs, mxArray *plhs[], int nrhs,
                             const mxArray *prhs[]) {
  TRY(mu::CheckInput(nrhs, 2, "clear"), ERROR_INPUT);
  libgp_.Clear();
  return SUCCESS;
}

LibgpMexInterface::ReturnStatus
LibgpMexInterface::CallGetNumSamples(int nlhs, mxArray *plhs[], int nrhs,
                                     const mxArray *prhs[]) {
  TRY(mu::CheckInput(nrhs, 2, "getNumSamples"), ERROR_INPUT);
  libgp_.GetNumSamples();
  return SUCCESS;
}

// The mex function that is invoked by MATLAB, as explained by:
// https://www.mathworks.com/help/matlab/apiref/mexfunction.html
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

  int fcnlen = 128;
  char fcn[fcnlen];

  // Verify the input contains at least one input argument.
  if (nrhs < 1 || mxGetString(prhs[0], fcn, fcnlen))
    mexErrMsgTxt("First input should be a command string less than 128 "
                 "characters long.");

  // Create a new class instance that serves as long-term storage.
  if (!strcmp("new", fcn)) {
    if (nlhs != 1)
      mexErrMsgTxt("new: at least one output required: <object_handle>");

    // Return a handle to a new C++ instance.
    plhs[0] = convertPtr2Mat<LibgpMexInterface>(new LibgpMexInterface);
    return;
  }

  // Delete the C++ object.
  if (!strcmp("delete", fcn)) {
    destroyObject<LibgpMexInterface>(prhs[1]);

    // Warn if other commands were ignored.
    if (nlhs != 0 || nrhs != 2)
      mexWarnMsgTxt("Delete: Unexpected arguments ignored.");
    return;
  }

  // Get the class instance pointer from the second input.
  LibgpMexInterface *libgp_utils = convertMat2Ptr<LibgpMexInterface>(prhs[1]);

  // Now hand it off to the class object which will do the real work.
  int out;
  switch (out = libgp_utils->FunctionCall(nlhs, plhs, nrhs, prhs)) {
  case LibgpMexInterface::EXIT: {
    mexWarnMsgTxt("Exiting.");
    return;
  }
  case LibgpMexInterface::ERROR_INPUT: {
    mexWarnMsgTxt("Incorrect Input type.");
    return;
  }
  case LibgpMexInterface::ERROR_FAILED: {
    mexWarnMsgTxt("Input Failed.");
    return;
  }
  case LibgpMexInterface::ERROR_UNHANDLED: {
    mexWarnMsgTxt("Unhandled Input.");
    return;
  }
  case LibgpMexInterface::ERROR_MATRIX: {
    mexWarnMsgTxt("Cannot convert input into matrix/vector.");
    return;
  }
  case LibgpMexInterface::ERROR_MATRIX_SIZE: {
    mexWarnMsgTxt("Matrix size incorrect.");
    return;
  }
  default:
    return;
  }
}
