// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ShimmersensorProt.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_ShimmersensorProt_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_ShimmersensorProt_2eproto

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
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_ShimmersensorProt_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_ShimmersensorProt_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_ShimmersensorProt_2eproto;
namespace ShimmersensorProt {
class DataPacket;
struct DataPacketDefaultTypeInternal;
extern DataPacketDefaultTypeInternal _DataPacket_default_instance_;
class SendCommand;
struct SendCommandDefaultTypeInternal;
extern SendCommandDefaultTypeInternal _SendCommand_default_instance_;
}  // namespace ShimmersensorProt
PROTOBUF_NAMESPACE_OPEN
template<> ::ShimmersensorProt::DataPacket* Arena::CreateMaybeMessage<::ShimmersensorProt::DataPacket>(Arena*);
template<> ::ShimmersensorProt::SendCommand* Arena::CreateMaybeMessage<::ShimmersensorProt::SendCommand>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace ShimmersensorProt {

enum Command : int {
  COMMAND_NOOP = 0,
  COMMAND_START_STREAM = 1,
  COMMAND_STOP_STREAM = 2,
  Command_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  Command_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool Command_IsValid(int value);
constexpr Command Command_MIN = COMMAND_NOOP;
constexpr Command Command_MAX = COMMAND_STOP_STREAM;
constexpr int Command_ARRAYSIZE = Command_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Command_descriptor();
template<typename T>
inline const std::string& Command_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Command>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Command_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Command_descriptor(), enum_t_value);
}
inline bool Command_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, Command* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Command>(
    Command_descriptor(), name, value);
}
enum State : int {
  STATE_IDLE = 0,
  STATE_STREAMING = 1,
  STATE_ERROR = 2,
  State_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  State_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool State_IsValid(int value);
constexpr State State_MIN = STATE_IDLE;
constexpr State State_MAX = STATE_ERROR;
constexpr int State_ARRAYSIZE = State_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* State_descriptor();
template<typename T>
inline const std::string& State_Name(T enum_t_value) {
  static_assert(::std::is_same<T, State>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function State_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    State_descriptor(), enum_t_value);
}
inline bool State_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, State* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<State>(
    State_descriptor(), name, value);
}
// ===================================================================

class DataPacket final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:ShimmersensorProt.DataPacket) */ {
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
    return "ShimmersensorProt.DataPacket";
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
    kTimestampFieldNumber = 2,
    kStateFieldNumber = 1,
    kAccelLnXFieldNumber = 3,
    kAccelLnYFieldNumber = 4,
    kAccelLnZFieldNumber = 5,
  };
  // string timestamp = 2;
  void clear_timestamp();
  const std::string& timestamp() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_timestamp(ArgT0&& arg0, ArgT... args);
  std::string* mutable_timestamp();
  PROTOBUF_NODISCARD std::string* release_timestamp();
  void set_allocated_timestamp(std::string* timestamp);
  private:
  const std::string& _internal_timestamp() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_timestamp(const std::string& value);
  std::string* _internal_mutable_timestamp();
  public:

  // .ShimmersensorProt.State state = 1;
  void clear_state();
  ::ShimmersensorProt::State state() const;
  void set_state(::ShimmersensorProt::State value);
  private:
  ::ShimmersensorProt::State _internal_state() const;
  void _internal_set_state(::ShimmersensorProt::State value);
  public:

  // int32 accel_ln_x = 3;
  void clear_accel_ln_x();
  int32_t accel_ln_x() const;
  void set_accel_ln_x(int32_t value);
  private:
  int32_t _internal_accel_ln_x() const;
  void _internal_set_accel_ln_x(int32_t value);
  public:

  // int32 accel_ln_y = 4;
  void clear_accel_ln_y();
  int32_t accel_ln_y() const;
  void set_accel_ln_y(int32_t value);
  private:
  int32_t _internal_accel_ln_y() const;
  void _internal_set_accel_ln_y(int32_t value);
  public:

  // int32 accel_ln_z = 5;
  void clear_accel_ln_z();
  int32_t accel_ln_z() const;
  void set_accel_ln_z(int32_t value);
  private:
  int32_t _internal_accel_ln_z() const;
  void _internal_set_accel_ln_z(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:ShimmersensorProt.DataPacket)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr timestamp_;
    int state_;
    int32_t accel_ln_x_;
    int32_t accel_ln_y_;
    int32_t accel_ln_z_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_ShimmersensorProt_2eproto;
};
// -------------------------------------------------------------------

class SendCommand final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:ShimmersensorProt.SendCommand) */ {
 public:
  inline SendCommand() : SendCommand(nullptr) {}
  ~SendCommand() override;
  explicit PROTOBUF_CONSTEXPR SendCommand(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  SendCommand(const SendCommand& from);
  SendCommand(SendCommand&& from) noexcept
    : SendCommand() {
    *this = ::std::move(from);
  }

  inline SendCommand& operator=(const SendCommand& from) {
    CopyFrom(from);
    return *this;
  }
  inline SendCommand& operator=(SendCommand&& from) noexcept {
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
  static const SendCommand& default_instance() {
    return *internal_default_instance();
  }
  static inline const SendCommand* internal_default_instance() {
    return reinterpret_cast<const SendCommand*>(
               &_SendCommand_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(SendCommand& a, SendCommand& b) {
    a.Swap(&b);
  }
  inline void Swap(SendCommand* other) {
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
  void UnsafeArenaSwap(SendCommand* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  SendCommand* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<SendCommand>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const SendCommand& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const SendCommand& from) {
    SendCommand::MergeImpl(*this, from);
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
  void InternalSwap(SendCommand* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "ShimmersensorProt.SendCommand";
  }
  protected:
  explicit SendCommand(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kCommandFieldNumber = 1,
    kValueFieldNumber = 2,
  };
  // .ShimmersensorProt.Command command = 1;
  void clear_command();
  ::ShimmersensorProt::Command command() const;
  void set_command(::ShimmersensorProt::Command value);
  private:
  ::ShimmersensorProt::Command _internal_command() const;
  void _internal_set_command(::ShimmersensorProt::Command value);
  public:

  // optional int32 value = 2;
  bool has_value() const;
  private:
  bool _internal_has_value() const;
  public:
  void clear_value();
  int32_t value() const;
  void set_value(int32_t value);
  private:
  int32_t _internal_value() const;
  void _internal_set_value(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:ShimmersensorProt.SendCommand)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
    int command_;
    int32_t value_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_ShimmersensorProt_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// DataPacket

// .ShimmersensorProt.State state = 1;
inline void DataPacket::clear_state() {
  _impl_.state_ = 0;
}
inline ::ShimmersensorProt::State DataPacket::_internal_state() const {
  return static_cast< ::ShimmersensorProt::State >(_impl_.state_);
}
inline ::ShimmersensorProt::State DataPacket::state() const {
  // @@protoc_insertion_point(field_get:ShimmersensorProt.DataPacket.state)
  return _internal_state();
}
inline void DataPacket::_internal_set_state(::ShimmersensorProt::State value) {
  
  _impl_.state_ = value;
}
inline void DataPacket::set_state(::ShimmersensorProt::State value) {
  _internal_set_state(value);
  // @@protoc_insertion_point(field_set:ShimmersensorProt.DataPacket.state)
}

// string timestamp = 2;
inline void DataPacket::clear_timestamp() {
  _impl_.timestamp_.ClearToEmpty();
}
inline const std::string& DataPacket::timestamp() const {
  // @@protoc_insertion_point(field_get:ShimmersensorProt.DataPacket.timestamp)
  return _internal_timestamp();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void DataPacket::set_timestamp(ArgT0&& arg0, ArgT... args) {
 
 _impl_.timestamp_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:ShimmersensorProt.DataPacket.timestamp)
}
inline std::string* DataPacket::mutable_timestamp() {
  std::string* _s = _internal_mutable_timestamp();
  // @@protoc_insertion_point(field_mutable:ShimmersensorProt.DataPacket.timestamp)
  return _s;
}
inline const std::string& DataPacket::_internal_timestamp() const {
  return _impl_.timestamp_.Get();
}
inline void DataPacket::_internal_set_timestamp(const std::string& value) {
  
  _impl_.timestamp_.Set(value, GetArenaForAllocation());
}
inline std::string* DataPacket::_internal_mutable_timestamp() {
  
  return _impl_.timestamp_.Mutable(GetArenaForAllocation());
}
inline std::string* DataPacket::release_timestamp() {
  // @@protoc_insertion_point(field_release:ShimmersensorProt.DataPacket.timestamp)
  return _impl_.timestamp_.Release();
}
inline void DataPacket::set_allocated_timestamp(std::string* timestamp) {
  if (timestamp != nullptr) {
    
  } else {
    
  }
  _impl_.timestamp_.SetAllocated(timestamp, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.timestamp_.IsDefault()) {
    _impl_.timestamp_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:ShimmersensorProt.DataPacket.timestamp)
}

// int32 accel_ln_x = 3;
inline void DataPacket::clear_accel_ln_x() {
  _impl_.accel_ln_x_ = 0;
}
inline int32_t DataPacket::_internal_accel_ln_x() const {
  return _impl_.accel_ln_x_;
}
inline int32_t DataPacket::accel_ln_x() const {
  // @@protoc_insertion_point(field_get:ShimmersensorProt.DataPacket.accel_ln_x)
  return _internal_accel_ln_x();
}
inline void DataPacket::_internal_set_accel_ln_x(int32_t value) {
  
  _impl_.accel_ln_x_ = value;
}
inline void DataPacket::set_accel_ln_x(int32_t value) {
  _internal_set_accel_ln_x(value);
  // @@protoc_insertion_point(field_set:ShimmersensorProt.DataPacket.accel_ln_x)
}

// int32 accel_ln_y = 4;
inline void DataPacket::clear_accel_ln_y() {
  _impl_.accel_ln_y_ = 0;
}
inline int32_t DataPacket::_internal_accel_ln_y() const {
  return _impl_.accel_ln_y_;
}
inline int32_t DataPacket::accel_ln_y() const {
  // @@protoc_insertion_point(field_get:ShimmersensorProt.DataPacket.accel_ln_y)
  return _internal_accel_ln_y();
}
inline void DataPacket::_internal_set_accel_ln_y(int32_t value) {
  
  _impl_.accel_ln_y_ = value;
}
inline void DataPacket::set_accel_ln_y(int32_t value) {
  _internal_set_accel_ln_y(value);
  // @@protoc_insertion_point(field_set:ShimmersensorProt.DataPacket.accel_ln_y)
}

// int32 accel_ln_z = 5;
inline void DataPacket::clear_accel_ln_z() {
  _impl_.accel_ln_z_ = 0;
}
inline int32_t DataPacket::_internal_accel_ln_z() const {
  return _impl_.accel_ln_z_;
}
inline int32_t DataPacket::accel_ln_z() const {
  // @@protoc_insertion_point(field_get:ShimmersensorProt.DataPacket.accel_ln_z)
  return _internal_accel_ln_z();
}
inline void DataPacket::_internal_set_accel_ln_z(int32_t value) {
  
  _impl_.accel_ln_z_ = value;
}
inline void DataPacket::set_accel_ln_z(int32_t value) {
  _internal_set_accel_ln_z(value);
  // @@protoc_insertion_point(field_set:ShimmersensorProt.DataPacket.accel_ln_z)
}

// -------------------------------------------------------------------

// SendCommand

// .ShimmersensorProt.Command command = 1;
inline void SendCommand::clear_command() {
  _impl_.command_ = 0;
}
inline ::ShimmersensorProt::Command SendCommand::_internal_command() const {
  return static_cast< ::ShimmersensorProt::Command >(_impl_.command_);
}
inline ::ShimmersensorProt::Command SendCommand::command() const {
  // @@protoc_insertion_point(field_get:ShimmersensorProt.SendCommand.command)
  return _internal_command();
}
inline void SendCommand::_internal_set_command(::ShimmersensorProt::Command value) {
  
  _impl_.command_ = value;
}
inline void SendCommand::set_command(::ShimmersensorProt::Command value) {
  _internal_set_command(value);
  // @@protoc_insertion_point(field_set:ShimmersensorProt.SendCommand.command)
}

// optional int32 value = 2;
inline bool SendCommand::_internal_has_value() const {
  bool value = (_impl_._has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool SendCommand::has_value() const {
  return _internal_has_value();
}
inline void SendCommand::clear_value() {
  _impl_.value_ = 0;
  _impl_._has_bits_[0] &= ~0x00000001u;
}
inline int32_t SendCommand::_internal_value() const {
  return _impl_.value_;
}
inline int32_t SendCommand::value() const {
  // @@protoc_insertion_point(field_get:ShimmersensorProt.SendCommand.value)
  return _internal_value();
}
inline void SendCommand::_internal_set_value(int32_t value) {
  _impl_._has_bits_[0] |= 0x00000001u;
  _impl_.value_ = value;
}
inline void SendCommand::set_value(int32_t value) {
  _internal_set_value(value);
  // @@protoc_insertion_point(field_set:ShimmersensorProt.SendCommand.value)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace ShimmersensorProt

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::ShimmersensorProt::Command> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ShimmersensorProt::Command>() {
  return ::ShimmersensorProt::Command_descriptor();
}
template <> struct is_proto_enum< ::ShimmersensorProt::State> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ShimmersensorProt::State>() {
  return ::ShimmersensorProt::State_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_ShimmersensorProt_2eproto
