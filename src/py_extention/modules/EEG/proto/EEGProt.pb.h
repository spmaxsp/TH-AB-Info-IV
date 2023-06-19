// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: EEGProt.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_EEGProt_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_EEGProt_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_EEGProt_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_EEGProt_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_EEGProt_2eproto;
namespace EEGProt {
class DataPacket;
struct DataPacketDefaultTypeInternal;
extern DataPacketDefaultTypeInternal _DataPacket_default_instance_;
class SendCommand;
struct SendCommandDefaultTypeInternal;
extern SendCommandDefaultTypeInternal _SendCommand_default_instance_;
}  // namespace EEGProt
PROTOBUF_NAMESPACE_OPEN
template<> ::EEGProt::DataPacket* Arena::CreateMaybeMessage<::EEGProt::DataPacket>(Arena*);
template<> ::EEGProt::SendCommand* Arena::CreateMaybeMessage<::EEGProt::SendCommand>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace EEGProt {

enum Command : int {
  COMMAND_NOOP = 0,
  COMMAND_START_STREAM = 1,
  COMMAND_STOP_STREAM = 2,
  COMMAND_LOAD_PROFILE = 3,
  COMMAND_GET_STATE = 4,
  Command_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  Command_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool Command_IsValid(int value);
constexpr Command Command_MIN = COMMAND_NOOP;
constexpr Command Command_MAX = COMMAND_GET_STATE;
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
  STATE_WAITING = 2,
  STATE_READY = 3,
  State_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  State_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool State_IsValid(int value);
constexpr State State_MIN = STATE_IDLE;
constexpr State State_MAX = STATE_READY;
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
enum Type : int {
  TYPE_NONE = 0,
  TYPE_DATA = 1,
  TYPE_STATE_ONLY = 2,
  Type_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  Type_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool Type_IsValid(int value);
constexpr Type Type_MIN = TYPE_NONE;
constexpr Type Type_MAX = TYPE_STATE_ONLY;
constexpr int Type_ARRAYSIZE = Type_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Type_descriptor();
template<typename T>
inline const std::string& Type_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Type>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Type_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Type_descriptor(), enum_t_value);
}
inline bool Type_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, Type* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Type>(
    Type_descriptor(), name, value);
}
// ===================================================================

class DataPacket final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:EEGProt.DataPacket) */ {
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
    return "EEGProt.DataPacket";
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
    kDataFieldNumber = 3,
    kStateFieldNumber = 1,
    kTypeFieldNumber = 4,
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

  // string data = 3;
  void clear_data();
  const std::string& data() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_data(ArgT0&& arg0, ArgT... args);
  std::string* mutable_data();
  PROTOBUF_NODISCARD std::string* release_data();
  void set_allocated_data(std::string* data);
  private:
  const std::string& _internal_data() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_data(const std::string& value);
  std::string* _internal_mutable_data();
  public:

  // .EEGProt.State state = 1;
  void clear_state();
  ::EEGProt::State state() const;
  void set_state(::EEGProt::State value);
  private:
  ::EEGProt::State _internal_state() const;
  void _internal_set_state(::EEGProt::State value);
  public:

  // .EEGProt.Type type = 4;
  void clear_type();
  ::EEGProt::Type type() const;
  void set_type(::EEGProt::Type value);
  private:
  ::EEGProt::Type _internal_type() const;
  void _internal_set_type(::EEGProt::Type value);
  public:

  // @@protoc_insertion_point(class_scope:EEGProt.DataPacket)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr timestamp_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_;
    int state_;
    int type_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_EEGProt_2eproto;
};
// -------------------------------------------------------------------

class SendCommand final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:EEGProt.SendCommand) */ {
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
    return "EEGProt.SendCommand";
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
    kDataFieldNumber = 2,
    kCommandFieldNumber = 1,
  };
  // optional string data = 2;
  bool has_data() const;
  private:
  bool _internal_has_data() const;
  public:
  void clear_data();
  const std::string& data() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_data(ArgT0&& arg0, ArgT... args);
  std::string* mutable_data();
  PROTOBUF_NODISCARD std::string* release_data();
  void set_allocated_data(std::string* data);
  private:
  const std::string& _internal_data() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_data(const std::string& value);
  std::string* _internal_mutable_data();
  public:

  // .EEGProt.Command command = 1;
  void clear_command();
  ::EEGProt::Command command() const;
  void set_command(::EEGProt::Command value);
  private:
  ::EEGProt::Command _internal_command() const;
  void _internal_set_command(::EEGProt::Command value);
  public:

  // @@protoc_insertion_point(class_scope:EEGProt.SendCommand)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_;
    int command_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_EEGProt_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// DataPacket

// .EEGProt.State state = 1;
inline void DataPacket::clear_state() {
  _impl_.state_ = 0;
}
inline ::EEGProt::State DataPacket::_internal_state() const {
  return static_cast< ::EEGProt::State >(_impl_.state_);
}
inline ::EEGProt::State DataPacket::state() const {
  // @@protoc_insertion_point(field_get:EEGProt.DataPacket.state)
  return _internal_state();
}
inline void DataPacket::_internal_set_state(::EEGProt::State value) {
  
  _impl_.state_ = value;
}
inline void DataPacket::set_state(::EEGProt::State value) {
  _internal_set_state(value);
  // @@protoc_insertion_point(field_set:EEGProt.DataPacket.state)
}

// string timestamp = 2;
inline void DataPacket::clear_timestamp() {
  _impl_.timestamp_.ClearToEmpty();
}
inline const std::string& DataPacket::timestamp() const {
  // @@protoc_insertion_point(field_get:EEGProt.DataPacket.timestamp)
  return _internal_timestamp();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void DataPacket::set_timestamp(ArgT0&& arg0, ArgT... args) {
 
 _impl_.timestamp_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:EEGProt.DataPacket.timestamp)
}
inline std::string* DataPacket::mutable_timestamp() {
  std::string* _s = _internal_mutable_timestamp();
  // @@protoc_insertion_point(field_mutable:EEGProt.DataPacket.timestamp)
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
  // @@protoc_insertion_point(field_release:EEGProt.DataPacket.timestamp)
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
  // @@protoc_insertion_point(field_set_allocated:EEGProt.DataPacket.timestamp)
}

// string data = 3;
inline void DataPacket::clear_data() {
  _impl_.data_.ClearToEmpty();
}
inline const std::string& DataPacket::data() const {
  // @@protoc_insertion_point(field_get:EEGProt.DataPacket.data)
  return _internal_data();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void DataPacket::set_data(ArgT0&& arg0, ArgT... args) {
 
 _impl_.data_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:EEGProt.DataPacket.data)
}
inline std::string* DataPacket::mutable_data() {
  std::string* _s = _internal_mutable_data();
  // @@protoc_insertion_point(field_mutable:EEGProt.DataPacket.data)
  return _s;
}
inline const std::string& DataPacket::_internal_data() const {
  return _impl_.data_.Get();
}
inline void DataPacket::_internal_set_data(const std::string& value) {
  
  _impl_.data_.Set(value, GetArenaForAllocation());
}
inline std::string* DataPacket::_internal_mutable_data() {
  
  return _impl_.data_.Mutable(GetArenaForAllocation());
}
inline std::string* DataPacket::release_data() {
  // @@protoc_insertion_point(field_release:EEGProt.DataPacket.data)
  return _impl_.data_.Release();
}
inline void DataPacket::set_allocated_data(std::string* data) {
  if (data != nullptr) {
    
  } else {
    
  }
  _impl_.data_.SetAllocated(data, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.data_.IsDefault()) {
    _impl_.data_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:EEGProt.DataPacket.data)
}

// .EEGProt.Type type = 4;
inline void DataPacket::clear_type() {
  _impl_.type_ = 0;
}
inline ::EEGProt::Type DataPacket::_internal_type() const {
  return static_cast< ::EEGProt::Type >(_impl_.type_);
}
inline ::EEGProt::Type DataPacket::type() const {
  // @@protoc_insertion_point(field_get:EEGProt.DataPacket.type)
  return _internal_type();
}
inline void DataPacket::_internal_set_type(::EEGProt::Type value) {
  
  _impl_.type_ = value;
}
inline void DataPacket::set_type(::EEGProt::Type value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:EEGProt.DataPacket.type)
}

// -------------------------------------------------------------------

// SendCommand

// .EEGProt.Command command = 1;
inline void SendCommand::clear_command() {
  _impl_.command_ = 0;
}
inline ::EEGProt::Command SendCommand::_internal_command() const {
  return static_cast< ::EEGProt::Command >(_impl_.command_);
}
inline ::EEGProt::Command SendCommand::command() const {
  // @@protoc_insertion_point(field_get:EEGProt.SendCommand.command)
  return _internal_command();
}
inline void SendCommand::_internal_set_command(::EEGProt::Command value) {
  
  _impl_.command_ = value;
}
inline void SendCommand::set_command(::EEGProt::Command value) {
  _internal_set_command(value);
  // @@protoc_insertion_point(field_set:EEGProt.SendCommand.command)
}

// optional string data = 2;
inline bool SendCommand::_internal_has_data() const {
  bool value = (_impl_._has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool SendCommand::has_data() const {
  return _internal_has_data();
}
inline void SendCommand::clear_data() {
  _impl_.data_.ClearToEmpty();
  _impl_._has_bits_[0] &= ~0x00000001u;
}
inline const std::string& SendCommand::data() const {
  // @@protoc_insertion_point(field_get:EEGProt.SendCommand.data)
  return _internal_data();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void SendCommand::set_data(ArgT0&& arg0, ArgT... args) {
 _impl_._has_bits_[0] |= 0x00000001u;
 _impl_.data_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:EEGProt.SendCommand.data)
}
inline std::string* SendCommand::mutable_data() {
  std::string* _s = _internal_mutable_data();
  // @@protoc_insertion_point(field_mutable:EEGProt.SendCommand.data)
  return _s;
}
inline const std::string& SendCommand::_internal_data() const {
  return _impl_.data_.Get();
}
inline void SendCommand::_internal_set_data(const std::string& value) {
  _impl_._has_bits_[0] |= 0x00000001u;
  _impl_.data_.Set(value, GetArenaForAllocation());
}
inline std::string* SendCommand::_internal_mutable_data() {
  _impl_._has_bits_[0] |= 0x00000001u;
  return _impl_.data_.Mutable(GetArenaForAllocation());
}
inline std::string* SendCommand::release_data() {
  // @@protoc_insertion_point(field_release:EEGProt.SendCommand.data)
  if (!_internal_has_data()) {
    return nullptr;
  }
  _impl_._has_bits_[0] &= ~0x00000001u;
  auto* p = _impl_.data_.Release();
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.data_.IsDefault()) {
    _impl_.data_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  return p;
}
inline void SendCommand::set_allocated_data(std::string* data) {
  if (data != nullptr) {
    _impl_._has_bits_[0] |= 0x00000001u;
  } else {
    _impl_._has_bits_[0] &= ~0x00000001u;
  }
  _impl_.data_.SetAllocated(data, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.data_.IsDefault()) {
    _impl_.data_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:EEGProt.SendCommand.data)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace EEGProt

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::EEGProt::Command> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::EEGProt::Command>() {
  return ::EEGProt::Command_descriptor();
}
template <> struct is_proto_enum< ::EEGProt::State> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::EEGProt::State>() {
  return ::EEGProt::State_descriptor();
}
template <> struct is_proto_enum< ::EEGProt::Type> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::EEGProt::Type>() {
  return ::EEGProt::Type_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_EEGProt_2eproto
