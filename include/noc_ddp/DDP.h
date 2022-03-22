/* Author: Masaki Murooka */

#pragma once

#include <memory>
#include <vector>

#include <Eigen/Dense>

namespace NOC
{
/** \brief DDP problem.
    \tparam StateDim state dimension
    \tparam InputDim input dimension
 */
template<int StateDim, int InputDim>
class DDPProblem
{
public:
  /** \brief Type of vector of state dimension. */
  using StateDimVector = Eigen::Matrix<double, StateDim, 1>;

  /** \brief Type of vector of input dimension. */
  using InputDimVector = Eigen::Matrix<double, InputDim, 1>;

  /** \brief Type of matrix of state x state dimension. */
  using StateStateDimMatrix = Eigen::Matrix<double, StateDim, StateDim>;

  /** \brief Type of matrix of input x input dimension. */
  using InputInputDimMatrix = Eigen::Matrix<double, InputDim, InputDim>;

  /** \brief Type of matrix of state x input dimension. */
  using StateInputDimMatrix = Eigen::Matrix<double, StateDim, InputDim>;

  /** \brief Type of matrix of input x state dimension. */
  using InputStateDimMatrix = Eigen::Matrix<double, InputDim, StateDim>;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  /** \brief Constructor. */
  DDPProblem(int state_dim = StateDim, int input_dim = InputDim);

  /** \brief Gets the state dimension. */
  inline int stateDim() const
  {
    return state_dim_;
  }

  /** \brief Gets the input dimension. */
  inline int inputDim() const
  {
    return input_dim_;
  }

  /** \brief Calculate discrete state equation.
      \param x current state (x[k])
      \param u current input (u[k])
      \returns next state (x[k+1])
   */
  virtual StateDimVector stateEq(const StateDimVector & x, const InputDimVector & u) const = 0;

  /** \brief Calculate running cost.
      \param x current state (x[k])
      \param u current input (u[k])
      \returns running cost (L[k])
   */
  virtual double runningCost(const StateDimVector & x, const InputDimVector & u) const = 0;

  /** \brief Calculate terminal cost.
      \param x current state (x[k])
      \returns terminal cost (phi[k])
   */
  virtual double terminalCost(const StateDimVector & x) const = 0;

  /** \brief Calculate first-order derivatives of discrete state equation.
      \param x state
      \param u input
      \param state_eq_deriv_x first-order derivative of state equation w.r.t. state
      \param state_eq_deriv_u first-order derivative of state equation w.r.t. input
  */
  virtual void calcStateqDeriv(const StateDimVector & x,
                               const InputDimVector & u,
                               Eigen::Ref<StateStateDimMatrix> state_eq_deriv_x,
                               Eigen::Ref<StateInputDimMatrix> state_eq_deriv_u) const = 0;

  /** \brief Calculate first-order and second-order derivatives of discrete state equation.
      \param x state
      \param u input
      \param state_eq_deriv_x first-order derivative of state equation w.r.t. state
      \param state_eq_deriv_u first-order derivative of state equation w.r.t. input
      \param state_eq_deriv_xx second-order derivative of state equation w.r.t. state
      \param state_eq_deriv_uu second-order derivative of state equation w.r.t. input
      \param state_eq_deriv_xu second-order derivative of state equation w.r.t. state and input
  */
  virtual void calcStateqDeriv(const StateDimVector & x,
                               const InputDimVector & u,
                               Eigen::Ref<StateStateDimMatrix> state_eq_deriv_x,
                               Eigen::Ref<StateInputDimMatrix> state_eq_deriv_u,
                               std::vector<StateStateDimMatrix> & state_eq_deriv_xx,
                               std::vector<InputInputDimMatrix> & state_eq_deriv_uu,
                               std::vector<StateInputDimMatrix> & state_eq_deriv_xu) const = 0;

  /** \brief Calculate first-order derivatives of running cost.
      \param x state
      \param u input
      \param running_cost_deriv_x first-order derivative of running cost w.r.t. state
      \param running_cost_deriv_u first-order derivative of running cost w.r.t. input
  */
  virtual void calcRunningCostDeriv(const StateDimVector & x,
                                    const InputDimVector & u,
                                    Eigen::Ref<StateDimVector> running_cost_deriv_x,
                                    Eigen::Ref<InputDimVector> running_cost_deriv_u) const = 0;

  /** \brief Calculate first-order and second-order derivatives of running cost.
      \param x state
      \param u input
      \param running_cost_deriv_x first-order derivative of running cost w.r.t. state
      \param running_cost_deriv_u first-order derivative of running cost w.r.t. input
      \param running_cost_deriv_xx second-order derivative of running cost w.r.t. state
      \param running_cost_deriv_uu second-order derivative of running cost w.r.t. input
      \param running_cost_deriv_xu second-order derivative of running cost w.r.t. state and input
  */
  virtual void calcRunningCostDeriv(const StateDimVector & x,
                                    const InputDimVector & u,
                                    Eigen::Ref<StateDimVector> running_cost_deriv_x,
                                    Eigen::Ref<InputDimVector> running_cost_deriv_u,
                                    Eigen::Ref<StateStateDimMatrix> running_cost_deriv_xx,
                                    Eigen::Ref<InputInputDimMatrix> running_cost_deriv_uu,
                                    Eigen::Ref<StateInputDimMatrix> running_cost_deriv_xu) const = 0;

  /** \brief Calculate first-order derivatives of terminal cost.
      \param x state
      \param terminal_cost_deriv_x first-order derivative of terminal cost w.r.t. state
  */
  virtual void calcTerminalCostDeriv(const StateDimVector & x,
                                     Eigen::Ref<StateDimVector> terminal_cost_deriv_x) const = 0;

  /** \brief Calculate first-order and second-order derivatives of terminal cost.
      \param x state
      \param terminal_cost_deriv_x first-order derivative of terminal cost w.r.t. state
      \param terminal_cost_deriv_xx second-order derivative of terminal cost w.r.t. state
  */
  virtual void calcTerminalCostDeriv(const StateDimVector & x,
                                     Eigen::Ref<StateDimVector> terminal_cost_deriv_x,
                                     Eigen::Ref<StateStateDimMatrix> terminal_cost_deriv_xx) const = 0;

protected:
  //! State dimension
  const int state_dim_ = 0;

  //! Input dimension
  const int input_dim_ = 0;
};

/** \brief DDP solver.
    \tparam StateDim state dimension
    \tparam InputDim input dimension

    See the following for a detailed algorithm.
      - Y Tassa, T Erez, E Todorov. Synthesis and stabilization of complex behaviors through online trajectory
   optimization. IROS2012.
      - Y Tassa, N Mansard, E Todorov. Control-limited differential dynamic programming. ICRA2014.
      - https://www.mathworks.com/matlabcentral/fileexchange/52069-ilqg-ddp-trajectory-optimization
 */
template<int StateDim, int InputDim>
class DDPSolver
{
public:
  /** \brief Type of vector of state dimension. */
  using StateDimVector = Eigen::Matrix<double, StateDim, 1>;

  /** \brief Type of vector of input dimension. */
  using InputDimVector = Eigen::Matrix<double, InputDim, 1>;

  /** \brief Type of matrix of state x state dimension. */
  using StateStateDimMatrix = Eigen::Matrix<double, StateDim, StateDim>;

  /** \brief Type of matrix of input x input dimension. */
  using InputInputDimMatrix = Eigen::Matrix<double, InputDim, InputDim>;

  /** \brief Type of matrix of state x input dimension. */
  using StateInputDimMatrix = Eigen::Matrix<double, StateDim, InputDim>;

  /** \brief Type of matrix of input x state dimension. */
  using InputStateDimMatrix = Eigen::Matrix<double, InputDim, StateDim>;

public:
  /*! \brief Configuration. */
  struct Configuration
  {
    //! Whether to enable verbose print
    bool verbose_print = true;

    //! Whether to use second-order derivatives of state equation
    bool use_state_eq_second_derivative = false;

    //! Maximum iteration of optimization loop
    int max_iter = 500;

    //! Number of steps in horizon
    int horizon_steps = 100;

    //! Regularization type (1: Quu + lambda * I, 2: Vxx + lambda * I)
    int reg_type = 1;

    //! Initial regularization coefficient
    double initial_lambda = 1e-6;

    //! Initial scaling factor of regularization coefficient
    double initial_dlambda = 1.0;

    //! Increasing/decreasing factor of regularization coefficient scaling
    double lambda_factor = 1.6;

    //! Minimum regularization coefficient
    double lambda_min = 1e-6;

    //! Maximum regularization coefficient
    double lambda_max = 1e10;

    //! Termination threshold of relative norm of k
    double k_rel_norm_thre = 1e-4;

    //! Termination threshold of regularization coefficient
    double lambda_thre = 1e-5;

    //! List of alpha (scaling factor of k)
    Eigen::VectorXd alpha_list = Eigen::VectorXd::LinSpaced(11, 0, -3);

    //! Allowable threshold of cost update ratio
    double cost_update_ratio_thre = 0;

    //! Termination threshold of cost update
    double cost_update_thre = 1e-7;
  };

  /*! \brief Derivatives of DDP problem. */
  struct Derivative
  {
    /** \brief Constructor. */
    Derivative() {}

    /** \brief Constructor.
        \param state_dim state dimension
     */
    Derivative(int state_dim)
    {
      setStateDim(state_dim);
    }

    /** \brief Set state dimension.
        \param state_dim state dimension
     */
    void setStateDim(int state_dim)
    {
      Fxx.resize(state_dim);
      Fuu.resize(state_dim);
      Fxu.resize(state_dim);
    }

    //! First-order derivative of state equation w.r.t. state
    StateStateDimMatrix Fx;

    //! First-order derivative of state equation w.r.t. input
    StateInputDimMatrix Fu;

    //! Second-order derivative of state equation w.r.t. state (tensor of rank 3)
    std::vector<StateStateDimMatrix> Fxx;

    //! Second-order derivative of state equation w.r.t. input (tensor of rank 3)
    std::vector<InputInputDimMatrix> Fuu;

    //! Second-order derivative of state equation w.r.t. state and input (tensor of rank 3)
    std::vector<StateInputDimMatrix> Fxu;

    //! First-order derivative of running cost w.r.t. state
    StateDimVector Lx;

    //! First-order derivative of running cost w.r.t. input
    InputDimVector Lu;

    //! Second-order derivative of running cost w.r.t. state
    StateStateDimMatrix Lxx;

    //! Second-order derivative of running cost w.r.t. input
    InputInputDimMatrix Luu;

    //! Second-order derivative of running cost w.r.t. state and input
    StateInputDimMatrix Lxu;
  };

  /*! \brief Data to trace optimization loop. */
  struct TraceData
  {
    //! Iteration of optimization loop
    int iter = 0;

    //! Total cost
    double cost = 0;

    //! Regularization coefficient
    double lambda = 0;

    //! Scaling factor of regularization coefficient
    double dlambda = 0;

    //! Scaling factor of k
    double alpha = 0;

    //! Norm of relative values of k and u
    double k_rel_norm = 0;

    //! Actual update value of cost
    double cost_update_actual = 0;

    //! Expected update value of cost
    double cost_update_expected = 0;

    //! Ratio of actual and expected update values of cost
    double cost_update_ratio = 0;

    //! Duration to calculate derivatives
    double duration_derivative = 0;

    //! Duration to process backward pass
    double duration_backward = 0;

    //! Duration to process forward pass
    double duration_forward = 0;
  };

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  /** \brief Constructor.
      \param problem DDP problem
  */
  DDPSolver(const std::shared_ptr<DDPProblem<StateDim, InputDim>> & problem);

  /** \brief Solve optimization.
      \param current_x current state
      \param initial_u_list initial sequence of input
      \return whether the process is finished successfully
  */
  bool solve(const StateDimVector & current_x, const std::vector<InputDimVector> & initial_u_list);

protected:
  /** \brief Process one iteration.
      \param iter current iteration
      \return 0 for continue, 1 for terminate, -1 for failure
  */
  int procOnce(int iter);

  /** \brief Process backward pass.
      \return whether the process is finished successfully
  */
  bool backwardPass();

  /** \brief Process forward pass.
      \param alpha scaling factor of k
  */
  void forwardPass(double alpha);

public:
  //! Configuration
  Configuration config_;

  //! Non-linear optimal control problem
  std::shared_ptr<DDPProblem<StateDim, InputDim>> problem_;

  //! Sequence of trace data
  std::vector<TraceData> trace_data_list_;

  //! Regularization coefficient
  double lambda_ = 0;

  //! Scaling factor of regularization coefficient
  double dlambda_ = 0;

  //! Sequence of state (x[0], ..., x[N-1], x[N])
  std::vector<StateDimVector> x_list_;

  //! Sequence of input (u[0], ..., u[N-1])
  std::vector<InputDimVector> u_list_;

  //! Sequence of cost (L[0], ..., L[N-1], phi[N])
  Eigen::VectorXd cost_list_;

  //! Sequence of candidate state
  std::vector<StateDimVector> x_candidate_list_;

  //! Sequence of candidate input
  std::vector<InputDimVector> u_candidate_list_;

  //! Sequence of candidate cost
  Eigen::VectorXd cost_candidate_list_;

  //! Sequence of feedforward term for input (k[0], ..., k[N-1])
  std::vector<InputDimVector> k_list_;

  //! Sequence of feedback gain for input w.r.t. state error (K[0], ..., K[N-1])
  std::vector<InputStateDimMatrix> K_list_;

  //! Sequence of derivatives
  std::vector<Derivative> derivative_list_;

  //! First-order derivative of value in last step of horizon
  StateDimVector last_Vx_;

  //! Second-order derivative of value in last step of horizon
  StateStateDimMatrix last_Vxx_;

  //! Expected update of value
  Eigen::Vector2d dV_;
};
} // namespace NOC

#include <noc_ddp/DDP.hpp>
