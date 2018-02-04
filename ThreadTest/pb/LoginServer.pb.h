// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: LoginServer.proto

#ifndef PROTOBUF_LoginServer_2eproto__INCLUDED
#define PROTOBUF_LoginServer_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf_LoginServer_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
void InitDefaultsLoginReqImpl();
void InitDefaultsLoginReq();
inline void InitDefaults() {
  InitDefaultsLoginReq();
}
}  // namespace protobuf_LoginServer_2eproto
namespace lm {
class LoginReq;
class LoginReqDefaultTypeInternal;
extern LoginReqDefaultTypeInternal _LoginReq_default_instance_;
}  // namespace lm
namespace lm {

// ===================================================================

class LoginReq : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:lm.LoginReq) */ {
 public:
  LoginReq();
  virtual ~LoginReq();

  LoginReq(const LoginReq& from);

  inline LoginReq& operator=(const LoginReq& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  LoginReq(LoginReq&& from) noexcept
    : LoginReq() {
    *this = ::std::move(from);
  }

  inline LoginReq& operator=(LoginReq&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const LoginReq& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const LoginReq* internal_default_instance() {
    return reinterpret_cast<const LoginReq*>(
               &_LoginReq_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(LoginReq* other);
  friend void swap(LoginReq& a, LoginReq& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline LoginReq* New() const PROTOBUF_FINAL { return New(NULL); }

  LoginReq* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const LoginReq& from);
  void MergeFrom(const LoginReq& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(LoginReq* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string username = 1;
  bool has_username() const;
  void clear_username();
  static const int kUsernameFieldNumber = 1;
  const ::std::string& username() const;
  void set_username(const ::std::string& value);
  #if LANG_CXX11
  void set_username(::std::string&& value);
  #endif
  void set_username(const char* value);
  void set_username(const char* value, size_t size);
  ::std::string* mutable_username();
  ::std::string* release_username();
  void set_allocated_username(::std::string* username);

  // required string passwd = 2;
  bool has_passwd() const;
  void clear_passwd();
  static const int kPasswdFieldNumber = 2;
  const ::std::string& passwd() const;
  void set_passwd(const ::std::string& value);
  #if LANG_CXX11
  void set_passwd(::std::string&& value);
  #endif
  void set_passwd(const char* value);
  void set_passwd(const char* value, size_t size);
  ::std::string* mutable_passwd();
  ::std::string* release_passwd();
  void set_allocated_passwd(::std::string* passwd);

  // @@protoc_insertion_point(class_scope:lm.LoginReq)
 private:
  void set_has_username();
  void clear_has_username();
  void set_has_passwd();
  void clear_has_passwd();

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr username_;
  ::google::protobuf::internal::ArenaStringPtr passwd_;
  friend struct ::protobuf_LoginServer_2eproto::TableStruct;
  friend void ::protobuf_LoginServer_2eproto::InitDefaultsLoginReqImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// LoginReq

// required string username = 1;
inline bool LoginReq::has_username() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void LoginReq::set_has_username() {
  _has_bits_[0] |= 0x00000001u;
}
inline void LoginReq::clear_has_username() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void LoginReq::clear_username() {
  username_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_username();
}
inline const ::std::string& LoginReq::username() const {
  // @@protoc_insertion_point(field_get:lm.LoginReq.username)
  return username_.GetNoArena();
}
inline void LoginReq::set_username(const ::std::string& value) {
  set_has_username();
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:lm.LoginReq.username)
}
#if LANG_CXX11
inline void LoginReq::set_username(::std::string&& value) {
  set_has_username();
  username_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:lm.LoginReq.username)
}
#endif
inline void LoginReq::set_username(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_username();
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:lm.LoginReq.username)
}
inline void LoginReq::set_username(const char* value, size_t size) {
  set_has_username();
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:lm.LoginReq.username)
}
inline ::std::string* LoginReq::mutable_username() {
  set_has_username();
  // @@protoc_insertion_point(field_mutable:lm.LoginReq.username)
  return username_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* LoginReq::release_username() {
  // @@protoc_insertion_point(field_release:lm.LoginReq.username)
  clear_has_username();
  return username_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void LoginReq::set_allocated_username(::std::string* username) {
  if (username != NULL) {
    set_has_username();
  } else {
    clear_has_username();
  }
  username_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), username);
  // @@protoc_insertion_point(field_set_allocated:lm.LoginReq.username)
}

// required string passwd = 2;
inline bool LoginReq::has_passwd() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void LoginReq::set_has_passwd() {
  _has_bits_[0] |= 0x00000002u;
}
inline void LoginReq::clear_has_passwd() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void LoginReq::clear_passwd() {
  passwd_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_passwd();
}
inline const ::std::string& LoginReq::passwd() const {
  // @@protoc_insertion_point(field_get:lm.LoginReq.passwd)
  return passwd_.GetNoArena();
}
inline void LoginReq::set_passwd(const ::std::string& value) {
  set_has_passwd();
  passwd_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:lm.LoginReq.passwd)
}
#if LANG_CXX11
inline void LoginReq::set_passwd(::std::string&& value) {
  set_has_passwd();
  passwd_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:lm.LoginReq.passwd)
}
#endif
inline void LoginReq::set_passwd(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_passwd();
  passwd_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:lm.LoginReq.passwd)
}
inline void LoginReq::set_passwd(const char* value, size_t size) {
  set_has_passwd();
  passwd_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:lm.LoginReq.passwd)
}
inline ::std::string* LoginReq::mutable_passwd() {
  set_has_passwd();
  // @@protoc_insertion_point(field_mutable:lm.LoginReq.passwd)
  return passwd_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* LoginReq::release_passwd() {
  // @@protoc_insertion_point(field_release:lm.LoginReq.passwd)
  clear_has_passwd();
  return passwd_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void LoginReq::set_allocated_passwd(::std::string* passwd) {
  if (passwd != NULL) {
    set_has_passwd();
  } else {
    clear_has_passwd();
  }
  passwd_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), passwd);
  // @@protoc_insertion_point(field_set_allocated:lm.LoginReq.passwd)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace lm

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_LoginServer_2eproto__INCLUDED