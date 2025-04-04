**This is the branch for ROS2; use [the ros1 branch](https://github.com/isri-aist/NMPC/tree/ros1) for ROS1.**

# [NMPC](https://github.com/isri-aist/NMPC)
Non-linear model predictive control (NMPC) library

[![CI-standalone](https://github.com/isri-aist/NMPC/actions/workflows/ci-standalone.yaml/badge.svg)](https://github.com/isri-aist/NMPC/actions/workflows/ci-standalone.yaml)
[![CI-colcon](https://github.com/isri-aist/NMPC/actions/workflows/ci-colcon.yaml/badge.svg)](https://github.com/isri-aist/NMPC/actions/workflows/ci-colcon.yaml)

This repository provides ROS2 packages for the following NMPC methods

## [nmpc_ddp](./nmpc_ddp)

[![Documentation](https://img.shields.io/badge/doxygen-online-brightgreen?logo=read-the-docs&style=flat)](https://isri-aist.github.io/NMPC/nmpc_ddp/index.html)

NMPC with differential dynamic drogramming (DDP)

## [nmpc_fmpc](./nmpc_fmpc)

[![Documentation](https://img.shields.io/badge/doxygen-online-brightgreen?logo=read-the-docs&style=flat)](https://isri-aist.github.io/NMPC/nmpc_fmpc/index.html)

FMPC: Fast NMPC combining the direct multiple shooting (DMS) method, the primal-dual interior point (PDIP) method, and Riccati recursion (RR)

## [nmpc_cgmres](./nmpc_cgmres)

[![Documentation](https://img.shields.io/badge/doxygen-online-brightgreen?logo=read-the-docs&style=flat)](https://isri-aist.github.io/NMPC/nmpc_cgmres/index.html)

NMPC with continuation/GMRES method (C/GMRES)
