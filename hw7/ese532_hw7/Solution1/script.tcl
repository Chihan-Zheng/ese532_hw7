############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
############################################################
open_project ese532_hw7
set_top Filter_horizontal_HW
add_files Filter.cpp
add_files Pipeline.h
add_files -tb testbench.cpp
open_solution "Solution1" -flow_target vitis
set_part {xczu3eg-sbva484-1-i}
create_clock -period 150MHz -name default
#source "./ese532_hw7/Solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
