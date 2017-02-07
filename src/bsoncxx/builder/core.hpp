// Copyright 2014 MongoDB Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <memory>
#include <stdexcept>
#include <type_traits>

#include <bsoncxx/array/value.hpp>
#include <bsoncxx/array/view.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/types.hpp>

#include <bsoncxx/config/prelude.hpp>

namespace bsoncxx {
BSONCXX_INLINE_NAMESPACE_BEGIN
namespace builder {

///
/// A low-level interface for constructing BSON documents and arrays.
///
/// @remark
///   Generally it is recommended to use the classes in builder::basic or builder::stream instead of
///   using this class directly. However, developers who wish to write their own abstractions may
///   find this class useful.
///
class BSONCXX_API core {
   public:
    class BSONCXX_PRIVATE impl;

    ///
    /// Constructs an empty BSON datum.
    ///
    /// @param is_array
    ///   True if the top-level BSON datum should be an array.
    ///
    explicit core(bool is_array);

    core(core&& rhs) noexcept;
    core& operator=(core&& rhs) noexcept;

    ~core();

    ///
    /// Appends a key passed as a non-owning stdx::string_view.
    ///
    /// @remark
    ///   Use key_owned() unless you know what you are doing.
    ///
    /// @warning
    ///   The caller must ensure that the lifetime of the backing string extends until the next
    ///   value is appended.
    ///
    /// @param key
    ///   A null-terminated array of characters.
    ///
    /// @throws bsoncxx::exception if the current BSON datum is an array or if the previous value
    /// appended to the builder was also a key.
    ///
    void key_view(stdx::string_view key);

    ///
    /// Appends a key passed as an STL string.  Transfers ownership of the key to this class.
    ///
    /// @param key
    ///   A string key.
    ///
    /// @throws bsoncxx::exception if the current BSON datum is an array or if the previous value
    /// appended to the builder was a key.
    ///
    void key_owned(std::string key);

    ///
    /// Opens a sub-document within this BSON datum.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void open_document();

    ///
    /// Opens a sub-array within this BSON datum.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void open_array();

    ///
    /// Closes the current sub-document within this BSON datum.
    ///
    /// @throws bsoncxx::exception if the current BSON datum is not an open sub-document.
    ///
    void close_document();

    ///
    /// Closes the current sub-array within this BSON datum.
    ///
    /// @throws bsoncxx::exception if the current BSON datum is not an open sub-array.
    ///
    void close_array();

    ///
    /// Appends the keys from a BSON document into this BSON datum.
    ///
    /// @note
    ///   If this BSON datum is a document, the original keys from `view` are kept.  Otherwise (if
    ///   this BSON datum is an array), the original keys from `view` are discarded.
    ///
    /// @note
    ///   This can be used with an array::view as well by converting it to a document::view first.
    ///
    void concatenate(const document::view& view);

    ///
    /// Appends a BSON double.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_double& value);

    ///
    /// Append a BSON UTF-8 string.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_utf8& value);

    ///
    /// Appends a BSON document.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_document& value);

    ///
    /// Appends a BSON array.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_array& value);

    ///
    /// Appends a BSON binary datum.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_binary& value);

    ///
    /// Appends a BSON undefined.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_undefined& value);

    ///
    /// Appends a BSON ObjectId.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_oid& value);

    ///
    /// Appends a BSON boolean.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_bool& value);

    ///
    /// Appends a BSON date.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_date& value);

    ///
    /// Appends a BSON null.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_null& value);

    ///
    /// Appends a BSON regex.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_regex& value);

    ///
    /// Appends a BSON DBPointer.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_dbpointer& value);

    ///
    /// Appends a BSON JavaScript code.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_code& value);

    ///
    /// Appends a BSON symbol.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_symbol& value);

    ///
    /// Appends a BSON JavaScript code with scope.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_codewscope& value);

    ///
    /// Appends a BSON 32-bit signed integer.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_int32& value);

    ///
    /// Appends a BSON replication timestamp.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_timestamp& value);

    ///
    /// Appends a BSON 64-bit signed integer.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_int64& value);

    ///
    /// Appends a BSON Decimal128.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_decimal128& value);

    ///
    /// Appends a BSON min-key.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_minkey& value);

    ///
    /// Appends a BSON max-key.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::b_maxkey& value);

    ///
    /// Appends a BSON variant value.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const types::value& value);

    ///
    /// Appends an STL string as a BSON UTF-8 string.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(std::string str);

    ///
    /// Appends a string view as a BSON UTF-8 string.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(stdx::string_view str);

    ///
    /// Appends a char* or const char*.
    ///
    /// We disable all other pointer types to prevent the surprising implicit conversion to bool.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    template <typename T>
    BSONCXX_INLINE void append(T* v) {
        static_assert(std::is_same<typename std::remove_const<T>::type, char>::value,
                      "append is disabled for non-char pointer types");
        append(types::b_utf8{v});
    }

    ///
    /// Appends a native boolean as a BSON boolean.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(bool value);

    ///
    /// Appends a native double as a BSON double.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(double value);

    ///
    /// Appends a native int32_t as a BSON 32-bit signed integer.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(std::int32_t value);

    ///
    /// Appends a native int64_t as a BSON 64-bit signed integer.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(std::int64_t value);

    ///
    /// Appends an oid as a BSON ObjectId.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(const oid& value);

    ///
    /// Appends a decimal128 object as a BSON Decimal128.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(decimal128 value);

    ///
    /// Appends the given document view.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(document::view view);

    ///
    /// Appends the given array view.
    ///
    /// @throws
    ///   bsoncxx::exception if the current BSON datum is a document that is waiting for a key to be
    ///   appended to start a new key/value pair.
    ///
    void append(array::view view);

    ///
    /// Gets a view over the document.
    ///
    /// @return A document::view of the internal BSON.
    ///
    /// @pre
    ///    The top-level BSON datum should be a document that is not waiting for a key to be
    ///    appended to start a new key/value pair, and does contain any open sub-documents or open
    ///    sub-arrays.
    ///
    /// @throws bsoncxx::exception if the precondition is violated.
    ///
    document::view view_document() const;

    ///
    /// Gets a view over the array.
    ///
    /// @return An array::view of the internal BSON.
    ///
    /// @pre
    ///    The top-level BSON datum should be an array that does not contain any open sub-documents
    ///    or open sub-arrays.
    ///
    /// @throws bsoncxx::exception if the precondition is violated.
    ///
    array::view view_array() const;

    ///
    /// Transfers ownership of the underlying document to the caller.
    ///
    /// @return A document::value with ownership of the document.
    ///
    /// @pre
    ///    The top-level BSON datum should be a document that is not waiting for a key to be
    ///    appended to start a new key/value pair, and does not contain any open sub-documents or
    ///    open sub-arrays.
    ///
    /// @throws bsoncxx::exception if the precondition is violated.
    ///
    /// @warning
    ///   After calling extract_document() it is illegal to call any methods on this class, unless
    ///   it is subsequenly moved into.
    ///
    document::value extract_document();

    ///
    /// Transfers ownership of the underlying document to the caller.
    ///
    /// @return A document::value with ownership of the document.
    ///
    /// @pre
    ///    The top-level BSON datum should be an array that does not contain any open sub-documents
    ///    or open sub-arrays.
    ///
    /// @throws bsoncxx::exception if the precondition is violated.
    ///
    /// @warning
    ///   After calling extract_array() it is illegal to call any methods on this class, unless it
    ///   is subsequenly moved into.
    ///
    array::value extract_array();

    ///
    /// Deletes the contents of the underlying BSON datum. After calling clear(), the state of this
    /// class will be the same as it was immediately after construction.
    ///
    void clear();

   private:
    std::unique_ptr<impl> _impl;
};

}  // namespace builder
BSONCXX_INLINE_NAMESPACE_END
}  // namespace bsoncxx

#include <bsoncxx/config/postlude.hpp>
