// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: PyModuleProt.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_PyModuleProt_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_PyModuleProt_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_PyModuleProt_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_PyModuleProt_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_PyModuleProt_2eproto;
namespace PyModuleProt {
class DataPacket;
struct DataPacketDefaultTypeInternal;
extern DataPacketDefaultTypeInternal _DataPacket_default_instance_;
}  // namespace PyModuleProt
PROTOBUF_NAMESPACE_OPEN
template<> ::PyModuleProt::DataPacket* Arena::CreateMaybeMessage<::PyModuleProt::DataPacket>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace PyModuleProt {

// ===================================================================

class DataPacket final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:PyModuleProt.DataPacket) */ {
 public:
  inline DataPacket() : DataPacket(nullptr) {}
  ~DataPacket() override;
  explicit PROTOBUF_CONSTEXPR DataPacket(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  DataPacket(const DataPacket& from);
  DataPacket(DataPacket&& from) noexcept
    : DataPacket() {
    *this = ::std::move(from);
  }

  inline DataPacket& operator=(const DataPacket& from) {
    CopyFrom(from);
    return *this;
  }
  inline DataPacket& operator=(DataPacket&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const DataPacket& default_instance() {
    return *internal_default_instance();
  }
  static inline const DataPacket* internal_default_instance() {
    return reinterpret_cast<const DataPacket*>(
               &_DataPacket_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(DataPacket& a, DataPacket& b) {
    a.Swap(&b);
  }
  inline void Swap(DataPacket* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(DataPacket* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  DataPacket* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<DataPacket>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const DataPacket& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const DataPacket& from) {
    DataPacket::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(DataPacket* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "PyModuleProt.DataPacket";
  }
  protected:
  explicit DataPacket(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kSomeSensorFieldNumber = 1,
  };
  // int32 some_sensor = 1;
  void clear_some_sensor();
  int32_t some_sensor() const;
  void set_some_sensor(int32_t value);
  private:
  int32_t _internal_some_sensor() const;
  void _internal_set_some_sensor(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:PyModuleProt.DataPacket)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    int32_t some_sensor_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_PyModuleProt_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// DataPacket

// int32 some_sensor = 1;
inline void DataPacket::clear_some_sensor() {
  _impl_.some_sensor_ = 0;
}
inline int32_t DataPacket::_internal_some_sensor() const {
  return _impl_.some_sensor_;
}
inline int32_t DataPacket::some_sensor() const {
  // @@protoc_insertion_point(field_get:PyModuleProt.DataPacket.some_sensor)
  return _internal_some_sensor();
}
inline void DataPacket::_internal_set_some_sensor(int32_t value) {
  
  _impl_.some_sensor_ = value;
}
inline void DataPacket::set_some_sensor(int32_t value) {
  _internal_set_some_sensor(value);
  // @@protoc_insertion_point(field_set:PyModuleProt.DataPacket.some_sensor)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace PyModuleProt

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_PyModuleProt_2eproto