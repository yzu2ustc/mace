//
// Copyright (c) 2017 XiaoMi All rights reserved.
//

#ifndef MACE_CORE_MACE_H_
#define MACE_CORE_MACE_H_
#include <cstdint>
#include <vector>
#include <string>
#include "mace/core/logging.h"

namespace mace {

enum NetMode {
  INIT = 0,
  NORMAL = 1
};

enum DeviceType {
  CPU = 0,
  NEON = 1,
  OPENCL = 2
};

enum DataType {
  DT_INVALID = 0,
  DT_FLOAT = 1,
  DT_DOUBLE = 2,
  DT_INT32 = 3,
  DT_UINT8 = 4,
  DT_INT16 = 5,
  DT_INT8 = 6,
  DT_STRING = 7,
  DT_INT64 = 8,
  DT_UINT16 = 9,
  DT_BOOL = 10,
  DT_HALF = 19,
  DT_UINT32 = 22
};

class TensorProto {
 public:
  TensorProto(const std::string &name,
              unsigned char *data,
              const std::vector<int64_t> &dims,
              const DataType data_type = DT_FLOAT,
              uint32_t node_id = 0);
  TensorProto(const std::string &name,
              unsigned char *data,
              const std::vector<int64_t> &dims,
              const int data_type,
              uint32_t node_id = 0);

  const std::string &name() const;
  unsigned char *data() const;
  const int data_size() const;
  const std::vector<int64_t> &dims() const;
  DataType data_type() const;
  uint32_t node_id() const;

 private:
  std::string name_;
  unsigned char *data_;
  int data_size_;
  std::vector<int64_t> dims_;
  DataType data_type_;
  uint32_t node_id_;
};

class Argument {
 public:
  Argument();
  void CopyFrom(const Argument &from) ;
 public:
  const std::string &name() const;
  void set_name(const std::string& value);
  bool has_f() const;
  float f() const ;
  void set_f(float value) ;
  bool has_i() const ;
  int64_t i() const ;
  void set_i(int64_t value);
  bool has_s() const ;
  std::string s() const ;
  void set_s(const std::string& value) ;
  const std::vector<float> &floats() const ;
  void add_floats(float value) ;
  void set_floats(const std::vector<float> &value);
  const std::vector<int64_t> &ints() const ;
  void add_ints(int64_t value) ;
  void set_ints(const std::vector<int64_t> &value);
  const std::vector<std::string> &strings() const ;
  void add_strings(const ::std::string& value) ;
  void set_strings(const std::vector<std::string> &value);

 private:
  void set_has_f() ;
  void set_has_i() ;
  void set_has_s() ;

 private:
  std::string name_;
  float f_;
  int64_t i_;
  std::string s_;
  std::vector<float> floats_;
  std::vector<int64_t > ints_;
  std::vector<std::string> strings_;
  uint32_t has_bits_;
};

class NodeInput {
 public:
  void CopyFrom(const NodeInput &from) {
    node_id_ = from.node_id();
    output_port_ = from.output_port();
  }
 public:
  int node_id() const {
    return node_id_;
  }
  int output_port() const {
    return output_port_;
  }
 private:
  int node_id_;
  int output_port_;
};

class OutputShape {
 public:
  void CopyFrom(const OutputShape &from) {
    auto from_dims = from.dims();
    dims_.resize(from_dims.size());
    std::copy(from_dims.begin(), from_dims.end(), dims_.begin());
  }
 public:
  const std::vector<int64_t> &dims() const {
    return dims_;
  }
 private:
  std::vector<int64_t> dims_;
};

class OperatorDef {
 public:
  void CopyFrom(const OperatorDef &from);

 public:
  const std::string &name() const;
  void set_name(const std::string &name_);
  bool has_name() const;
  const std::string &type() const;
  void set_type(const std::string &type_);
  bool has_type() const;
  int mem_id() const;
  void set_mem_id(const int mem_id);
  bool has_mem_id() const;
  uint32_t node_id() const;
  uint32_t op_id() const;
  uint32_t padding() const;
  const std::vector<NodeInput> &node_input() const;
  const std::vector<int> &out_max_byte_size() const;
  const std::vector<std::string> &input() const;
  const std::string& input(int index) const;
  std::string* add_input();
  void add_input(const ::std::string& value);
  void add_input(::std::string&& value);
  void set_input(const std::vector<std::string> &value);
  const std::vector<std::string> &output() const;
  const std::string& output(int index) const;
  std::string* add_output();
  void add_output(const ::std::string& value);
  void add_output(::std::string&& value);
  void set_output(const std::vector<std::string> &value);
  const std::vector<Argument> &arg() const;
  Argument* add_arg();
  const std::vector<OutputShape> &output_shape() const;
  void set_output_shape(const std::vector<OutputShape> &value);
  const std::vector<DataType> &output_type() const;
  void set_output_type(const std::vector<DataType> &value);

 private:
  void set_has_name();
  void set_has_type();
  void set_has_mem_id();

 private:
  std::string name_;
  std::string type_;

  std::vector<std::string> input_;
  std::vector<std::string> output_;
  std::vector<Argument> arg_;
  std::vector<OutputShape> output_shape_;
  std::vector<DataType> output_type_;

  int mem_id_;

  // nnlib
  uint32_t node_id_;
  uint32_t op_id_;
  uint32_t padding_;
  std::vector<NodeInput> node_input_;
  std::vector<int> out_max_byte_size_;

  uint32_t has_bits_;
};

class MemoryBlock {
 public:
  MemoryBlock(int mem_id, uint32_t x, uint32_t y);
 public:
  int mem_id() const;
  uint32_t x() const;
  uint32_t y() const;
 private:
  int mem_id_;
  uint32_t x_;
  uint32_t y_;
};

class MemoryArena {
 public:
  inline const std::vector<MemoryBlock> &mem_block() const {
    return mem_block_;
  }
  inline std::vector<MemoryBlock> &mutable_mem_block() {
    return mem_block_;
  }
  inline int mem_block_size() const {
    return mem_block_.size();
  }
 private:
  std::vector<MemoryBlock> mem_block_;

};

// for hexagon mace-nnlib
class InputInfo {
 public:
  const std::string &name() const {
    return name_;
  }
  int32_t node_id() const {
    return node_id_;
  }
  int32_t max_byte_size() const {
    return max_byte_size_;
  }
  DataType data_type() const {
    return data_type_;
  }
  const std::vector<int32_t> &dims() const {
    return dims_;
  }
 private:
  std::string name_;
  int32_t node_id_;
  int32_t max_byte_size_; // only support 32-bit len
  DataType data_type_;
  std::vector<int32_t> dims_;
};

class OutputInfo {
 public:
  const std::string &name() const {
    return name_;
  }
  int32_t node_id() const {
    return node_id_;
  }
  int32_t max_byte_size() const {
    return max_byte_size_;
  }
  DataType data_type() const {
    return data_type_;
  }
  const std::vector<int32_t> &dims() const {
    return dims_;
  }
 private:
  std::string name_;
  int32_t node_id_;
  int32_t max_byte_size_; // only support 32-bit len
  DataType data_type_;
  std::vector<int32_t> dims_;
};

class NetDef {
 public:
  NetDef();
  int op_size() const;

  const OperatorDef &op(const int idx) const;
 public:
  const std::string &name() const;
  bool has_name() const;
  void set_name(const std::string& value);
  const std::string &version() const;
  bool has_version() const;
  void set_version(const std::string& value);

  const std::vector<OperatorDef> &op() const;
  OperatorDef* add_op();
  std::vector<OperatorDef> &mutable_op();
  const std::vector<Argument> &arg() const;
  Argument *add_arg();
  std::vector<Argument> &mutable_arg();
  const std::vector<TensorProto> &tensors() const;
  std::vector<TensorProto> &mutable_tensors();
  const MemoryArena &mem_arena() const;
  bool has_mem_arena() const;
  MemoryArena &mutable_mem_arena();
  const std::vector<InputInfo> &input_info() const;
  const std::vector<OutputInfo> &output_info() const;

 private:
  void set_has_name();
  void set_has_version();
  void set_has_mem_arena();

 private:
  std::string name_;
  std::string version_;
  std::vector<OperatorDef> op_;
  std::vector<Argument> arg_;
  std::vector<TensorProto> tensors_;

  // for mem optimization
  MemoryArena mem_arena_;

  // for hexagon mace-nnlib
  std::vector<InputInfo> input_info_;
  std::vector<OutputInfo> output_info_;

  uint32_t has_bits_;
};

} //  namespace mace
#endif //  MACE_CORE_MACE_H_
