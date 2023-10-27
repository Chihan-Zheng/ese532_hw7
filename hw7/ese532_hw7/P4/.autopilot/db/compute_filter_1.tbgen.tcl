set moduleName compute_filter_1
set isTopModule 0
set isCombinational 0
set isDatapathOnly 0
set isPipelined 1
set pipeline_type dataflow
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {compute_filter.1}
set C_modelType { void 0 }
set C_modelArgList {
	{ inStream int 8 regular {fifo 0 volatile } {global 0}  }
	{ outStream int 8 regular {fifo 1 volatile } {global 1}  }
}
set C_modelArgMapList {[ 
	{ "Name" : "inStream", "interface" : "fifo", "bitwidth" : 8, "direction" : "READONLY", "extern" : 0} , 
 	{ "Name" : "outStream", "interface" : "fifo", "bitwidth" : 8, "direction" : "WRITEONLY", "extern" : 0} ]}
# RTL Port declarations: 
set portNum 13
set portList { 
	{ inStream_dout sc_in sc_lv 8 signal 0 } 
	{ inStream_empty_n sc_in sc_logic 1 signal 0 } 
	{ inStream_read sc_out sc_logic 1 signal 0 } 
	{ outStream_din sc_out sc_lv 8 signal 1 } 
	{ outStream_full_n sc_in sc_logic 1 signal 1 } 
	{ outStream_write sc_out sc_logic 1 signal 1 } 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_continue sc_in sc_logic 1 continue -1 } 
}
set NewPortList {[ 
	{ "name": "inStream_dout", "direction": "in", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "inStream", "role": "dout" }} , 
 	{ "name": "inStream_empty_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inStream", "role": "empty_n" }} , 
 	{ "name": "inStream_read", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "inStream", "role": "read" }} , 
 	{ "name": "outStream_din", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "outStream", "role": "din" }} , 
 	{ "name": "outStream_full_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "outStream", "role": "full_n" }} , 
 	{ "name": "outStream_write", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "outStream", "role": "write" }} , 
 	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_continue", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "continue", "bundle":{"name": "ap_continue", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "3", "12", "13"],
		"CDFG" : "compute_filter_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "1", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "Dataflow", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "1",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "134747", "EstimateLatencyMax" : "386178",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "1",
		"InDataflowNetwork" : "1",
		"HasNonBlockingOperation" : "0",
		"InputProcess" : [
			{"ID" : "1", "Name" : "Filter_horizontal_HW_1_U0"}],
		"OutputProcess" : [
			{"ID" : "3", "Name" : "Filter_vertical_HW_1_U0"}],
		"Port" : [
			{"Name" : "inStream", "Type" : "Fifo", "Direction" : "I", "DependentProc" : "0", "DependentChan" : "0", "DependentChanDepth" : "2", "DependentChanType" : "0",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "Filter_horizontal_HW_1_U0", "Port" : "inStream"}]},
			{"Name" : "tempStream", "Type" : "Fifo", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "Filter_horizontal_HW_1_U0", "Port" : "tempStream"},
					{"ID" : "3", "SubInstance" : "Filter_vertical_HW_1_U0", "Port" : "tempStream"}]},
			{"Name" : "outStream", "Type" : "Fifo", "Direction" : "O", "DependentProc" : "0", "DependentChan" : "0", "DependentChanDepth" : "2", "DependentChanType" : "0",
				"SubConnect" : [
					{"ID" : "3", "SubInstance" : "Filter_vertical_HW_1_U0", "Port" : "outStream"}]}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Filter_horizontal_HW_1_U0", "Parent" : "0", "Child" : ["2"],
		"CDFG" : "Filter_horizontal_HW_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "1", "ap_idle" : "1", "real_start" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "130951", "EstimateLatencyMax" : "130951",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "1",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "inStream", "Type" : "Fifo", "Direction" : "I", "DependentProc" : "0", "DependentChan" : "0", "DependentChanDepth" : "2", "DependentChanType" : "0",
				"BlockSignal" : [
					{"Name" : "inStream_blk_n", "Type" : "RtlSignal"}]},
			{"Name" : "tempStream", "Type" : "Fifo", "Direction" : "O", "DependentProc" : "3", "DependentChan" : "12", "DependentChanDepth" : "2", "DependentChanType" : "0",
				"BlockSignal" : [
					{"Name" : "tempStream_blk_n", "Type" : "RtlSignal"}]}]},
	{"ID" : "2", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.Filter_horizontal_HW_1_U0.mac_muladd_8ns_8ns_16ns_16_4_1_U10", "Parent" : "1"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Filter_vertical_HW_1_U0", "Parent" : "0", "Child" : ["4", "5", "6", "7", "8", "9", "10", "11"],
		"CDFG" : "Filter_vertical_HW_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "1", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "4277", "EstimateLatencyMax" : "255708",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "1",
		"HasNonBlockingOperation" : "0",
		"StartSource" : "1",
		"StartFifo" : "start_for_Filter_vertical_HW_1_U0_U",
		"Port" : [
			{"Name" : "tempStream", "Type" : "Fifo", "Direction" : "I", "DependentProc" : "1", "DependentChan" : "12", "DependentChanDepth" : "2", "DependentChanType" : "0",
				"BlockSignal" : [
					{"Name" : "tempStream_blk_n", "Type" : "RtlSignal"}]},
			{"Name" : "outStream", "Type" : "Fifo", "Direction" : "O", "DependentProc" : "0", "DependentChan" : "0", "DependentChanDepth" : "2", "DependentChanType" : "0",
				"BlockSignal" : [
					{"Name" : "outStream_blk_n", "Type" : "RtlSignal"}]}]},
	{"ID" : "4", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.Filter_vertical_HW_1_U0.Input_local_0_U", "Parent" : "3"},
	{"ID" : "5", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.Filter_vertical_HW_1_U0.Input_local_1_U", "Parent" : "3"},
	{"ID" : "6", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.Filter_vertical_HW_1_U0.Input_local_2_U", "Parent" : "3"},
	{"ID" : "7", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.Filter_vertical_HW_1_U0.Input_local_3_U", "Parent" : "3"},
	{"ID" : "8", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.Filter_vertical_HW_1_U0.Input_local_4_U", "Parent" : "3"},
	{"ID" : "9", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.Filter_vertical_HW_1_U0.Input_local_5_U", "Parent" : "3"},
	{"ID" : "10", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.Filter_vertical_HW_1_U0.Input_local_6_U", "Parent" : "3"},
	{"ID" : "11", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.Filter_vertical_HW_1_U0.mac_muladd_8ns_8ns_16ns_16_4_1_U14", "Parent" : "3"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.tempStream_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.start_for_Filter_vertical_HW_1_U0_U", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	compute_filter_1 {
		inStream {Type I LastRead 8 FirstWrite -1}
		tempStream {Type IO LastRead -1 FirstWrite -1}
		outStream {Type O LastRead -1 FirstWrite 8}}
	Filter_horizontal_HW_1 {
		inStream {Type I LastRead 8 FirstWrite -1}
		tempStream {Type O LastRead -1 FirstWrite 11}}
	Filter_vertical_HW_1 {
		tempStream {Type I LastRead 7 FirstWrite -1}
		outStream {Type O LastRead -1 FirstWrite 8}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "134747", "Max" : "386178"}
	, {"Name" : "Interval", "Min" : "130952", "Max" : "255709"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	inStream { ap_fifo {  { inStream_dout fifo_data 0 8 }  { inStream_empty_n fifo_status 0 1 }  { inStream_read fifo_update 1 1 } } }
	outStream { ap_fifo {  { outStream_din fifo_data 1 8 }  { outStream_full_n fifo_status 0 1 }  { outStream_write fifo_update 1 1 } } }
}
