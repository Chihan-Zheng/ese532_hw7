# This script segment is generated automatically by AutoPilot

# Memory (RAM/ROM)  definition:
set ID 9
set hasByteEnable 0
set MemName Filter_HW_Filter_vertical_HW_Input_local_0
set CoreName ap_simcore_mem
set PortList { 2 0 }
set DataWd 8
set AddrRange 474
set AddrWd 9
set impl_style auto
set TrueReset 0
set HasInitializer 0
set IsROM 0
set ROMData {}
set NumOfStage 2
set MaxLatency -1
set DelayBudget 1.352
set ClkPeriod 6.667
set RegisteredInput 0
if {${::AESL::PGuard_simmodel_gen}} {
if {[info proc ap_gen_simcore_mem] == "ap_gen_simcore_mem"} {
    eval "ap_gen_simcore_mem { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
} else {
    puts "@W \[IMPL-102\] Cannot find ap_gen_simcore_mem, check your platform lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
  ::AP::rtl_comp_handler $MemName
}


set CoreName RAM
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_RAM] == "::AESL_LIB_VIRTEX::xil_gen_RAM"} {
    eval "::AESL_LIB_VIRTEX::xil_gen_RAM { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
  } else {
    puts "@W \[IMPL-104\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_RAM, check your platform lib"
  }
}


# clear list
if {${::AESL::PGuard_autoexp_gen}} {
    cg_default_interface_gen_dc_begin
    cg_default_interface_gen_bundle_begin
    AESL_LIB_XILADAPTER::native_axis_begin
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 10 \
    name aximm2 \
    type other \
    dir O \
    reset_level 1 \
    sync_rst true \
    corename dc_aximm2 \
    op interface \
    ports { m_axi_aximm2_AWVALID { O 1 bit } m_axi_aximm2_AWREADY { I 1 bit } m_axi_aximm2_AWADDR { O 64 vector } m_axi_aximm2_AWID { O 1 vector } m_axi_aximm2_AWLEN { O 32 vector } m_axi_aximm2_AWSIZE { O 3 vector } m_axi_aximm2_AWBURST { O 2 vector } m_axi_aximm2_AWLOCK { O 2 vector } m_axi_aximm2_AWCACHE { O 4 vector } m_axi_aximm2_AWPROT { O 3 vector } m_axi_aximm2_AWQOS { O 4 vector } m_axi_aximm2_AWREGION { O 4 vector } m_axi_aximm2_AWUSER { O 1 vector } m_axi_aximm2_WVALID { O 1 bit } m_axi_aximm2_WREADY { I 1 bit } m_axi_aximm2_WDATA { O 16 vector } m_axi_aximm2_WSTRB { O 2 vector } m_axi_aximm2_WLAST { O 1 bit } m_axi_aximm2_WID { O 1 vector } m_axi_aximm2_WUSER { O 1 vector } m_axi_aximm2_ARVALID { O 1 bit } m_axi_aximm2_ARREADY { I 1 bit } m_axi_aximm2_ARADDR { O 64 vector } m_axi_aximm2_ARID { O 1 vector } m_axi_aximm2_ARLEN { O 32 vector } m_axi_aximm2_ARSIZE { O 3 vector } m_axi_aximm2_ARBURST { O 2 vector } m_axi_aximm2_ARLOCK { O 2 vector } m_axi_aximm2_ARCACHE { O 4 vector } m_axi_aximm2_ARPROT { O 3 vector } m_axi_aximm2_ARQOS { O 4 vector } m_axi_aximm2_ARREGION { O 4 vector } m_axi_aximm2_ARUSER { O 1 vector } m_axi_aximm2_RVALID { I 1 bit } m_axi_aximm2_RREADY { O 1 bit } m_axi_aximm2_RDATA { I 16 vector } m_axi_aximm2_RLAST { I 1 bit } m_axi_aximm2_RID { I 1 vector } m_axi_aximm2_RUSER { I 1 vector } m_axi_aximm2_RRESP { I 2 vector } m_axi_aximm2_BVALID { I 1 bit } m_axi_aximm2_BREADY { O 1 bit } m_axi_aximm2_BRESP { I 2 vector } m_axi_aximm2_BID { I 1 vector } m_axi_aximm2_BUSER { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 11 \
    name Output_r \
    type fifo \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_Output_r \
    op interface \
    ports { Output_r_dout { I 64 vector } Output_r_empty_n { I 1 bit } Output_r_read { O 1 bit } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 12 \
    name tempStream \
    type fifo \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_tempStream \
    op interface \
    ports { tempStream_dout { I 8 vector } tempStream_empty_n { I 1 bit } tempStream_read { O 1 bit } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id -1 \
    name ap_ctrl \
    type ap_ctrl \
    reset_level 1 \
    sync_rst true \
    corename ap_ctrl \
    op interface \
    ports { ap_start { I 1 bit } ap_ready { O 1 bit } ap_done { O 1 bit } ap_idle { O 1 bit } ap_continue { I 1 bit } } \
} "
}


# Adapter definition:
set PortName ap_clk
set DataWd 1 
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc cg_default_interface_gen_clock] == "cg_default_interface_gen_clock"} {
eval "cg_default_interface_gen_clock { \
    id -2 \
    name ${PortName} \
    reset_level 1 \
    sync_rst true \
    corename apif_ap_clk \
    data_wd ${DataWd} \
    op interface \
}"
} else {
puts "@W \[IMPL-113\] Cannot find bus interface model in the library. Ignored generation of bus interface for '${PortName}'"
}
}


# Adapter definition:
set PortName ap_rst
set DataWd 1 
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc cg_default_interface_gen_reset] == "cg_default_interface_gen_reset"} {
eval "cg_default_interface_gen_reset { \
    id -3 \
    name ${PortName} \
    reset_level 1 \
    sync_rst true \
    corename apif_ap_rst \
    data_wd ${DataWd} \
    op interface \
}"
} else {
puts "@W \[IMPL-114\] Cannot find bus interface model in the library. Ignored generation of bus interface for '${PortName}'"
}
}



# merge
if {${::AESL::PGuard_autoexp_gen}} {
    cg_default_interface_gen_dc_end
    cg_default_interface_gen_bundle_end
    AESL_LIB_XILADAPTER::native_axis_end
}


