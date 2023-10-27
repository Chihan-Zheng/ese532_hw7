#include <systemc>
#include <vector>
#include <iostream>
#include "hls_stream.h"
#include "ap_int.h"
#include "ap_fixed.h"
using namespace std;
using namespace sc_dt;
class AESL_RUNTIME_BC {
  public:
    AESL_RUNTIME_BC(const char* name) {
      file_token.open( name);
      if (!file_token.good()) {
        cout << "Failed to open tv file " << name << endl;
        exit (1);
      }
      file_token >> mName;//[[[runtime]]]
    }
    ~AESL_RUNTIME_BC() {
      file_token.close();
    }
    int read_size () {
      int size = 0;
      file_token >> mName;//[[transaction]]
      file_token >> mName;//transaction number
      file_token >> mName;//pop_size
      size = atoi(mName.c_str());
      file_token >> mName;//[[/transaction]]
      return size;
    }
  public:
    fstream file_token;
    string mName;
};
unsigned int ap_apatb_outStream_V_cap_bc;
static AESL_RUNTIME_BC __xlx_outStream_V_size_Reader("../tv/stream_size/stream_size_out_outStream_V.dat");
extern "C" void Filter_horizontal_HW(short*, int, char*);
extern "C" void apatb_Filter_horizontal_HW_hw(volatile void * __xlx_apatb_param_Input, volatile void * __xlx_apatb_param_outStream) {
  // Collect __xlx_Input__tmp_vec
  vector<sc_bv<16> >__xlx_Input__tmp_vec;
  for (int j = 0, e = 1; j != e; ++j) {
    __xlx_Input__tmp_vec.push_back(((short*)__xlx_apatb_param_Input)[j]);
  }
  int __xlx_size_param_Input = 1;
  int __xlx_offset_param_Input = 0;
  int __xlx_offset_byte_param_Input = 0*2;
  short* __xlx_Input__input_buffer= new short[__xlx_Input__tmp_vec.size()];
  for (int i = 0; i < __xlx_Input__tmp_vec.size(); ++i) {
    __xlx_Input__input_buffer[i] = __xlx_Input__tmp_vec[i].range(15, 0).to_uint64();
  }
  //Create input buffer for outStream
  ap_apatb_outStream_V_cap_bc = __xlx_outStream_V_size_Reader.read_size();
  char* __xlx_outStream_input_buffer= new char[ap_apatb_outStream_V_cap_bc];
  // DUT call
  Filter_horizontal_HW(__xlx_Input__input_buffer, __xlx_offset_byte_param_Input, __xlx_outStream_input_buffer);
// print __xlx_apatb_param_Input
  sc_bv<16>*__xlx_Input_output_buffer = new sc_bv<16>[__xlx_size_param_Input];
  for (int i = 0; i < __xlx_size_param_Input; ++i) {
    __xlx_Input_output_buffer[i] = __xlx_Input__input_buffer[i+__xlx_offset_param_Input];
  }
  for (int i = 0; i < __xlx_size_param_Input; ++i) {
    ((short*)__xlx_apatb_param_Input)[i] = __xlx_Input_output_buffer[i].to_uint64();
  }
  for (unsigned i = 0; i <ap_apatb_outStream_V_cap_bc; ++i)
    ((hls::stream<char>*)__xlx_apatb_param_outStream)->write(__xlx_outStream_input_buffer[i]);
}
