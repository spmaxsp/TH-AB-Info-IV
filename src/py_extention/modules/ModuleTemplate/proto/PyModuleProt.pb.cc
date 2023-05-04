// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: PyModuleProt.proto

#include "PyModuleProt.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace PyModuleProt {
PROTOBUF_CONSTEXPR DataPacket::DataPacket(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.some_sensor_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct DataPacketDefaultTypeInternal {
  PROTOBUF_CONSTEXPR DataPacketDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~DataPacketDefaultTypeInternal() {}
  union {
    DataPacket _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 DataPacketDefaultTypeInternal _DataPacket_default_instance_;
}  // namespace PyModuleProt
static ::_pb::Metadata file_level_metadata_PyModuleProt_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_PyModuleProt_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_PyModuleProt_2eproto = nullptr;

const uint32_t TableStruct_PyModuleProt_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::PyModuleProt::DataPacket, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::PyModuleProt::DataPacket, _impl_.some_sensor_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::PyModuleProt::DataPacket)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::PyModuleProt::_DataPacket_default_instance_._instance,
};

const char descriptor_table_protodef_PyModuleProt_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\022PyModuleProt.proto\022\014PyModuleProt\"!\n\nDa"
  "taPacket\022\023\n\013some_sensor\030\001 \001(\005b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_PyModuleProt_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_PyModuleProt_2eproto = {
    false, false, 77, descriptor_table_protodef_PyModuleProt_2eproto,
    "PyModuleProt.proto",
    &descriptor_table_PyModuleProt_2eproto_once, nullptr, 0, 1,
    schemas, file_default_instances, TableStruct_PyModuleProt_2eproto::offsets,
    file_level_metadata_PyModuleProt_2eproto, file_level_enum_descriptors_PyModuleProt_2eproto,
    file_level_service_descriptors_PyModuleProt_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_PyModuleProt_2eproto_getter() {
  return &descriptor_table_PyModuleProt_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_PyModuleProt_2eproto(&descriptor_table_PyModuleProt_2eproto);
namespace PyModuleProt {

// ===================================================================

class DataPacket::_Internal {
 public:
};

DataPacket::DataPacket(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:PyModuleProt.DataPacket)
}
DataPacket::DataPacket(const DataPacket& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  DataPacket* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.some_sensor_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.some_sensor_ = from._impl_.some_sensor_;
  // @@protoc_insertion_point(copy_constructor:PyModuleProt.DataPacket)
}

inline void DataPacket::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.some_sensor_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

DataPacket::~DataPacket() {
  // @@protoc_insertion_point(destructor:PyModuleProt.DataPacket)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void DataPacket::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void DataPacket::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void DataPacket::Clear() {
// @@protoc_insertion_point(message_clear_start:PyModuleProt.DataPacket)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.some_sensor_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* DataPacket::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 some_sensor = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.some_sensor_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* DataPacket::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:PyModuleProt.DataPacket)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 some_sensor = 1;
  if (this->_internal_some_sensor() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_some_sensor(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:PyModuleProt.DataPacket)
  return target;
}

size_t DataPacket::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:PyModuleProt.DataPacket)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 some_sensor = 1;
  if (this->_internal_some_sensor() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_some_sensor());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData DataPacket::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    DataPacket::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*DataPacket::GetClassData() const { return &_class_data_; }


void DataPacket::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<DataPacket*>(&to_msg);
  auto& from = static_cast<const DataPacket&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:PyModuleProt.DataPacket)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_some_sensor() != 0) {
    _this->_internal_set_some_sensor(from._internal_some_sensor());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void DataPacket::CopyFrom(const DataPacket& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:PyModuleProt.DataPacket)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DataPacket::IsInitialized() const {
  return true;
}

void DataPacket::InternalSwap(DataPacket* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.some_sensor_, other->_impl_.some_sensor_);
}

::PROTOBUF_NAMESPACE_ID::Metadata DataPacket::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_PyModuleProt_2eproto_getter, &descriptor_table_PyModuleProt_2eproto_once,
      file_level_metadata_PyModuleProt_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace PyModuleProt
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::PyModuleProt::DataPacket*
Arena::CreateMaybeMessage< ::PyModuleProt::DataPacket >(Arena* arena) {
  return Arena::CreateMessageInternal< ::PyModuleProt::DataPacket >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>