//===-- fbx.h ---------------------------------------------*- mode: C99 -*-===//
//
//                           _____ _____ __ __
//                          |   __| __  |  |  |
//                          |   __| __ -|-   -|
//                          |__|  |_____|__|__|
//
//   The contents of this of this file is released into the public domain.
//
//===----------------------------------------------------------------------===//

#ifndef _FBX_H_
#define _FBX_H_

//  _____     _ _   _
// | __  |_ _|_| |_| |
// | __ -| | | | | . |
// |_____|___|_|_|___|

#if defined(_MSC_VER)
  #define FBX_ON_MSVC  1
  #define FBX_ON_CLANG 0
  #define FBX_ON_GCC   0
#elif defined(__GNUC__)
  #if defined(__clang__)
    #define FBX_ON_MSVC  0
    #define FBX_ON_CLANG 1
    #define FBX_ON_GCC   1
  #else
    // HACK(mtwilliams): We assume that we're being compiled with GCC.
    #define FBX_ON_MSVC  0
    #define FBX_ON_CLANG 0
    #define FBX_ON_GCC   1
  #endif
#endif

#if defined(_WIN32) || defined(_WIN64)
  #define FBX_ON_WINDOWS 1
  #define FBX_ON_MAC 0
  #define FBX_ON_LINUX 0
  #define FBX_ON_ANDROID 0
  #define FBX_ON_IOS 0
#elif defined(__APPLE__)
  #include <TargetConditionals.h>
  #if TARGET_OS_MAC
    #define FBX_ON_WINDOWS 0
    #define FBX_ON_MAC 1
    #define FBX_ON_LINUX 0
    #define FBX_ON_ANDROID 0
    #define FBX_ON_IOS 0
  #elif TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    #define FBX_ON_WINDOWS 0
    #define FBX_ON_MAC 0
    #define FBX_ON_LINUX 0
    #define FBX_ON_ANDROID 0
    #define FBX_ON_IOS 1
  #elif TARGET_OS_TVOS
    #error ("We don't support tvOS!")
  #endif
#elif defined(__linux__)
  #if defined(ANDROID)
    #define FBX_ON_WINDOWS 0
    #define FBX_ON_MAC 0
    #define FBX_ON_LINUX 0
    #define FBX_ON_ANDROID 1
    #define FBX_ON_IOS 0
  #else
    #define FBX_ON_WINDOWS 0
    #define FBX_ON_MAC 0
    #define FBX_ON_LINUX 1
    #define FBX_ON_ANDROID 0
    #define FBX_ON_IOS 0
  #endif
#else
  #error ("We don't support this platform!")
#endif

#if FBX_ON_WINDOWS
  #define FBX_ON_POSIX 0
#elif FBX_ON_MAC || FBX_ON_LINUX
  #define FBX_ON_POSIX 1
#elif FBX_ON_ANDROID || FBX_ON_IOS
  // Ostensibly...
  #define FBX_ON_POSIX 1
#endif

#if defined(_M_IX86) || defined(__i386__)
  #define FBX_ON_X86 1
  #define FBX_ON_X86_64 0
#elif defined(_M_X64) || defined(__x86_64__)
  #define FBX_ON_X86 0
  #define FBX_ON_X86_64 1
#endif

#if FBX_ON_WINDOWS
  #define FBX_EXPORT(ret) ret __stdcall
  #define FBX_CALLBACK(ret) ret __cdecl
#elif FBX_ON_MAC
  #define FBX_EXPORT(ret) ret
  #define FBX_CALLBACK(ret) ret
#elif FBX_ON_LINUX
  #define FBX_EXPORT(ret) ret
  #define FBX_CALLBACK(ret) ret
#elif FBX_ON_ANDROID
  #define FBX_EXPORT(ret) ret
  #define FBX_CALLBACK(ret) ret
#elif FBX_ON_IOS
  #define FBX_EXPORT(ret) ret
  #define FBX_CALLBACK(ret) ret
#endif

#if defined(__cplusplus)
  #define FBX_BEGIN_EXTERN_C extern "C" {
  #define FBX_END_EXTERN_C }
#else
  #define FBX_BEGIN_EXTERN_C
  #define FBX_END_EXTERN_C
#endif

//  _____
// |_   _|_ _ ___ ___ ___
//   | | | | | . | -_|_ -|
//   |_| |_  |  _|___|___|
//       |___|_|

FBX_BEGIN_EXTERN_C

typedef signed char    fbx_int8_t;
typedef unsigned char  fbx_uint8_t;
typedef signed short   fbx_int16_t;
typedef unsigned short fbx_uint16_t;
typedef signed int     fbx_int32_t;
typedef unsigned int   fbx_uint32_t;

#if defined(_MSC_VER)
  typedef signed __int64 fbx_int64_t;
  typedef unsigned __int64 fbx_uint64_t;
#else
  typedef signed long long fbx_int64_t;
  typedef unsigned long long fbx_uint64_t;
#endif

typedef float fbx_real32_t;
typedef double fbx_real64_t;

#if (_MSC_VER >= 1300 && _Wp64)
  #if FBX_ON_X86
    typedef __w64 signed long fbx_intptr_t;
    typedef __w64 unsigned long fbx_uintptr_t;
    typedef __w64 unsigned long fbx_size_t;
  #elif FBX_ON_X86_64
    typedef __w64 signed __int64 fbx_intptr_t;
    typedef __w64 unsigned __int64 fbx_uintptr_t;
    typedef __w64 unsigned __int64 fbx_size_t;
  #endif
#else
  #if FBX_ON_X86
    typedef fbx_int32_t fbx_intptr_t;
    typedef fbx_uint32_t fbx_uintptr_t;
    typedef fbx_uint32_t fbx_size_t;
  #elif FBX_ON_X86_64
    typedef fbx_int64_t fbx_intptr_t;
    typedef fbx_uint64_t fbx_uintptr_t;
    typedef fbx_uint64_t fbx_size_t;
  #endif
#endif

typedef unsigned int fbx_bool_t;

#define FBX_TRUE  ((fbx_bool_t)1)
#define FBX_FALSE ((fbx_bool_t)0)

// This will cause negative subscript errors if the sizes of our types don't
// match our expectations.
#define FBX_CHECK_SIZE_OF_TYPE(expression) \
  typedef char fbx__size_matches_expectation[(expression) ? 1 : -1]

// Ensure fixed width types match our expectations.
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_int8_t)   == 1);
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_uint8_t)  == 1);
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_int16_t)  == 2);
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_uint16_t) == 2);
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_int32_t)  == 4);
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_uint32_t) == 4);
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_int64_t)  == 8);
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_uint64_t) == 8);
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_real32_t) == 4);
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_real64_t) == 8);

// Ensure integer pointer types can actually hold pointers.
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_intptr_t) == sizeof(void *));
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_uintptr_t) == sizeof(void *));

#undef FBX_CHECK_SIZE_OF_TYPE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//  _____     ___     _
// |   __|___|  _|___| |_ _ _
// |__   | .'|  _| -_|  _| | |
// |_____|__,|_| |___|_| |_  |
//                       |___|

// Called when something horrible happened. Hints as to what then bails.
extern void fbx_panic(const char *format, ...);

//  _____
// |     |___ _____ ___ ___ _ _
// | | | | -_|     | . |  _| | |
// |_|_|_|___|_|_|_|___|_| |_  |
//                         |___|

/// Allocates @size bytes of memory aligned to @alignment byte boundary.
/// \return Pointer to allocated memory if successful, or `NULL` if not.
extern void *fbx_allocate(fbx_size_t size,
                          fbx_size_t alignment);


/// Allocates @size bytes of memory aligned to @alignment byte boundary.
/// \warning Panics if allocation fails.
/// \return Pointer to allocated memory.
extern void *fbx_allocate_s(fbx_size_t size,
                            fbx_size_t alignment);

/// Frees memory allocated by `fbx_allocate` or `fbx_allocate_s`.
extern void fbx_free(void *ptr);

// TODO(mtwilliams): Bookmarking. Maintain a linked list of positions that can
// be rewound to. If rewinding to bookmark prior to the last bookmark, remove
// it from the list to let rewinds cascade.

/// A simple bump allocator.
typedef struct fbx_block {
  fbx_uintptr_t base;
  fbx_size_t offset;
  fbx_size_t size;
} fbx_block_t;

/// Allocates @size bytes of memory from @block.
/// \return Pointer to allocated memory if successful, or `NULL` if not.
extern void *fbx_block_allocate(fbx_block_t *block,
                                fbx_size_t size);

/// Allocates @size bytes of memory from @block.
/// \warning Panics if allocation fails.
/// \return Pointer to allocated memory.
extern void *fbx_block_allocate_s(fbx_block_t *block,
                                  fbx_size_t size);

/// Empties @block.
extern void fbx_block_reset(fbx_block_t *block);

#define fbx_copy(source, destination, size_in_bytes) \
  memcpy((void *)(destination), (void *)(source), size_in_bytes)

#define fbx_copy_s(source, destination, size_in_bytes) \
  memmove((void *)(destination), (void *)(source), size_in_bytes)

#define fbx_zero(ptr, size_in_bytes) \
  memset((void *)(ptr), 0, size_in_bytes)

//  _____   _ _____
// |     | / |     |
// |-   -|/ /|  |  |
// |_____|_/ |_____|

/// An abstracted source of data.
typedef struct fbx_stream fbx_stream_t;

/// Wraps a @buffer of @size bytes in a stream.
/// \param @free_on_close Toggles freeing of @buffer after stream is closed.
extern fbx_stream_t *fbx_stream_open_from_memory(const void *buffer,
                                                 fbx_size_t size,
                                                 fbx_bool_t free_on_close);

#ifndef FBX_NO_STANDARD_LIBRARY

/// Wraps @file in a stream.
/// \param @close_after_use Toggles closing of @file after exhaustion.
extern fbx_stream_t *fbx_stream_open_from_file(FILE *file,
                                               fbx_bool_t close_after_use);

/// Opens a stream for the file at @path.
/// \param @path Absolute or relative path to file to stream.
extern fbx_stream_t *fbx_stream_open_from_path(const char *path);

#endif

/// Closes @stream.
extern void fbx_stream_close(fbx_stream_t *stream);

/// Returns absolute position in @stream.
extern fbx_size_t fbx_stream_tell(fbx_stream_t *stream);

/// Moves to absolute @offset in @stream.
/// \return True if seek was successful, false otherwise.
extern fbx_bool_t fbx_stream_seek(fbx_stream_t *stream,
                                  fbx_size_t offset);

/// Moves @offset bytes forward in @stream.
/// \return True if skip was successful, false otherwise.
extern fbx_bool_t fbx_stream_skip(fbx_stream_t *stream,
                                  fbx_size_t offset);

/// Reads up to @count bytes to @buffer.
/// \returns Number of bytes actually read. May be less than @count if @stream
/// is exhausted prior to completion.
extern fbx_size_t fbx_stream_read(fbx_stream_t *stream,
                                  void *buffer,
                                  fbx_size_t count);

/// Returns whether or not the stream is exhausted.
extern fbx_bool_t fbx_stream_exhausted(fbx_stream_t *stream);

//  _____     _   _
// |     |___| |_| |_
// | | | | .'|  _|   |
// |_|_|_|__,|_| |_|_|

/// \def FBX_PI
/// \brief Ratio of a circle's circumference to its diameter.
#define FBX_PI 3.14159265358979323846264338327950288f

/// \def FBX_TAU
/// \brief Twice as good as Pi.
/// \see FBX_PI
#define FBX_TAU (2.f * FBX_PI)

/// \brief Converts degrees to radians.
static fbx_real32_t fbx_degrees_to_radians(const fbx_real32_t degrees) {
  return degrees * (180.f / FBX_PI);
}

/// \brief Converts radians to degrees.
static fbx_real32_t fbx_radians_to_degrees(const fbx_real32_t radians) {
  return radians * (FBX_PI / 180.f);
}

typedef struct fbx_vec2 {
  fbx_real32_t x, y;
} fbx_vec2_t;

typedef struct fbx_vec3 {
  fbx_real32_t x, y, z;
} fbx_vec3_t;

typedef struct fbx_vec4 {
  fbx_real32_t x, y, z, w;
} fbx_vec4_t;

/// Represents a direction (and optionally, a magnitude).
typedef struct fbx_quaternion {
  fbx_real32_t x, y, z, w;
} fbx_quaternion_t;

/// Identity quaternion.
extern const fbx_quaternion_t FBX_IDENTITY_QUATERNION;

/// Represents a linear transformation of three-dimensional space.
///
/// \remark We store matrices in column major order. This means columns are
/// laid our contiguously in memory. We also uses columns vectors when
/// multiplying matrices and vectors, meaning transformations are applied
/// right-to-left.
///
typedef struct fbx_mat4 {
  fbx_real32_t v[4][4];
} fbx_mat4_t;

/// Zero matrix.
extern const fbx_mat4_t FBX_ZERO_MATRIX;

/// Identity matrix.
extern const fbx_mat4_t FBX_IDENTITY_MATRIX;

//  _____     _
// |     |___| |___ ___
// |   --| . | | . |  _|
// |_____|___|_|___|_|

// TODO(mtwilliams): HSL
// TODO(mtwilliams): HSV
// TODO(mtwilliams): YUV

typedef struct fbx_color {
  fbx_real32_t r, g, b, a;
} fbx_color_t;

static fbx_color_t fbx_rgb(fbx_uint8_t r, fbx_uint8_t g, fbx_uint8_t b) {
  return { r * (1.f/255.f), g * (1.f/255.f), b * (1.f/255.f), 1.f };
}

static fbx_color_t fbx_rgba(fbx_uint8_t r, fbx_uint8_t g, fbx_uint8_t b, fbx_uint8_t a) {
  return { r * (1.f/255.f), g * (1.f/255.f), b * (1.f/255.f), a * (1.f/255.f) };
}

static fbx_color_t fbx_rgbf(fbx_real32_t r, fbx_real32_t g, fbx_real32_t b) {
  return { r, g, b, 1.f };
}

static fbx_color_t fbx_rgbaf(fbx_real32_t r, fbx_real32_t g, fbx_real32_t b, fbx_real32_t a) {
  return { r, g, b, a };
}

//  _____ _   _ _ _ _
// |  |  | |_|_| |_| |_ _ _
// |  |  |  _| | | |  _| | |
// |_____|_| |_|_|_|_| |_  |
//                     |___|

/// \def FBX_MIN
/// \brief Chooses the lesser of @a and @b.
#define FBX_MIN(a, b) \
  (((a) < (b)) ? (a) : (b))

/// \def FBX_MAX
/// \brief Chooses the greater of @a and @b.
#define FBX_MAX(a, b) \
  (((a) > (b)) ? (a) : (b))

//  ____      _
// |    \ ___| |_ ___
// |  |  | .'|  _| .'|
// |____/|__,|_| |__,|

// Bindings?
// Custom properties?

typedef struct fbx_material fbx_material_t;
typedef struct fbx_texture fbx_texture_t;

typedef struct fbx_transform fbx_transform_t;

typedef struct fbx_model fbx_model_t;
typedef struct fbx_mesh fbx_mesh_t;
typedef struct fbx_topology fbx_topology_t;
typedef struct fbx_geometry fbx_geometry_t;

typedef struct fbx_camera fbx_camera_t;

typedef struct fbx_light fbx_light_t;

typedef struct fbx_bone fbx_bone_t;
typedef struct fbx_skeleton fbx_skeleton_t;
typedef struct fbx_pose fbx_pose_t;

typedef struct fbx_animation fbx_animation_t;

typedef struct fbx_metadata {
  const char *title;
  const char *subject;
  const char *author;
  const char *comment;
  const char *keywords;
  const char *version;
  const char *revision;
} fbx_metadata_t;

typedef struct fbx_tool_info {
  const char *vendor;
  const char *name;
  const char *version;
} fbx_tool_info_t;

typedef struct fbx_exporter_info {
  const char *name;
} fbx_exporter_info_t;

typedef struct fbx_timestamp {
  fbx_uint32_t year;
  fbx_uint32_t month;
  fbx_uint32_t day;
  fbx_uint32_t hour;
  fbx_uint32_t minute;
  fbx_uint32_t second;
  fbx_uint32_t millisecond;
} fbx_timestamp_t;

typedef struct fbx_basis {
  fbx_vec3_t up;
  fbx_vec3_t forward;
  fbx_vec3_t right;
} fbx_basis_t;

// TODO(mtwilliams): Units per meter?

typedef struct fbx {
  fbx_uint32_t version;

  fbx_metadata_t metadata;

  fbx_tool_info_t tool;
  fbx_exporter_info_t exporter;

  fbx_basis_t basis;
  fbx_vec3_t origin;

  fbx_timestamp_t timestamp;
} fbx_t;

//  _____                   _
// |     |_____ ___ ___ ___| |_
// |-   -|     | . | . |  _|  _|
// |_____|_|_|_|  _|___|_| |_|
//             |_|

// TODO(mtwilliams): Coordinate spaces.
// TODO(mtwilliams): Normal smoothing.
// TODO(mtwilliams): Triangulation.

typedef struct fbx_import_options {
  /// Memory to dedicate to permanent allocations. The permanent pool is only
  /// freed when the imported scene is freed.
  fbx_size_t permanent_memory_pool;

  /// Memory to dedicate to transient allocations. The transient pool is freed
  /// after importing.
  fbx_size_t transient_memory_pool;

  /// Memory to dedicate to string allocations. The string pool is only freed
  /// when the imported scene is freed.
  fbx_size_t strings_memory_pool;
} fbx_import_options_t;

typedef struct fbx_importer fbx_importer_t;

//  _____                 _
// |   __|_ _ ___ ___ ___| |_
// |   __|_'_| . | . |  _|  _|
// |_____|_,_|  _|___|_| |_|
//           |_|

// TODO(mtwilliams): Export functionality.

FBX_END_EXTERN_C

#endif

#ifdef FBX_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <math.h>

#if FBX_ON_WINDOWS || FBX_ON_POSIX
  #include <memory.h>
#endif

FBX_BEGIN_EXTERN_C

//  _____     ___     _
// |   __|___|  _|___| |_ _ _
// |__   | .'|  _| -_|  _| | |
// |_____|__,|_| |___|_| |_  |
//                       |___|

void fbx_panic(const char *format, ...)
{
  va_list ap;

  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);

  abort();
}

//  _____
// |     |___ _____ ___ ___ _ _
// | | | | -_|     | . |  _| | |
// |_|_|_|___|_|_|_|___|_| |_  |
//                         |___|

void *fbx_allocate(fbx_size_t size,
                   fbx_size_t alignment)
{
  void *ptr;

  if (alignment <= 16)
    alignment = 16;

#if FBX_ON_MSVC
  ptr = _aligned_malloc(size, alignment);
#else
  #if FBX_ON_POSIX
  #endif
#endif

  if (ptr)
    // We always zero memory, as it prevents an entire class of errors.
    memset(ptr, 0, size);

  return ptr;
}

void *fbx_allocate_s(fbx_size_t size,
                     fbx_size_t alignment)
{
  void *ptr;

  ptr = fbx_allocate(size, alignment);

  if (ptr == NULL)
    // We get `NULL` if out of memory.
    fbx_panic("Out of memory!\n");

  return ptr;
}

void fbx_free(void *ptr)
{
#if FBX_ON_MSVC
  _aligned_free(ptr);
#else
  #if FBX_ON_POSIX
  #endif
#endif
}

void *fbx_block_allocate(fbx_block_t *block,
                         fbx_size_t size)
{
  void *ptr;

  // Allocations are rounded up to the closest multiple of 16, thereby ensuring
  // 16-byte alignment of every allocation (assuming base is aligned.)
  size = (((size + 15) / 16) * 16);

  if ((block->offset + size) > block->size)
    // Not enough space.
    return NULL;

  ptr = (void *)(block->base + block->offset);

  block->offset += size;

  return ptr;
}

void *fbx_block_allocate_s(fbx_block_t *block,
                           fbx_size_t size)
{
  void *ptr;

  ptr = fbx_block_allocate(block, size);

  if (ptr == NULL)
    // We get `NULL` if out of memory.
    fbx_panic("Out of memory!\n");

  return ptr;
}

void fbx_block_reset(fbx_block_t *block)
{
  block->offset = 0;

  // Rezero. Again, to prevent an entire class of errors.
  memset((void *)block->base, 0, block->size);
}

//  _____   _ _____
// |     | / |     |
// |-   -|/ /|  |  |
// |_____|_/ |_____|

// TODO(mtwilliams): Support large files (hidden by pre-processor definition.)
// REFACTOR(mtwilliams): Support different streams through function pointers.

struct fbx_stream {
  const fbx_uint8_t *buffer;
  fbx_size_t size_of_buffer;
  fbx_bool_t free_on_close;
  fbx_size_t cursor;
};

fbx_stream_t *fbx_stream_open_from_memory(const void *buffer,
                                          fbx_size_t size,
                                          fbx_bool_t free_on_close)
{
  fbx_stream_t *stream = (fbx_stream_t *)fbx_allocate_s(sizeof(fbx_stream_t), 16);

  stream->buffer = (const fbx_uint8_t *)buffer;
  stream->size_of_buffer = size;

  stream->free_on_close = free_on_close;

  stream->cursor = 0;

  return stream;
}

#ifndef FBX_NO_STANDARD_LIBRARY

fbx_stream_t *fbx_stream_open_from_file(FILE *file,
                                        fbx_bool_t close_after_use)
{
  // Determine size of file.
  fseek(file, 0, SEEK_END);
  const fbx_size_t size_of_file = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate enough memory so the entire contents of the file can be read in.
  void *buffer = fbx_allocate_s(size_of_file, 1);

  // Read entire contents into memory.
  fread(buffer, 1, size_of_file, file);

  if (close_after_use)
    // HACK(mtwilliams): Immediately close since we're reading the contents
    // upfront, rather than streaming.
    fclose(file);

  return fbx_stream_open_from_memory(buffer, size_of_file, FBX_TRUE);
}

fbx_stream_t *fbx_stream_open_from_path(const char *path)
{
  if (FILE *file = fopen(path, "rb"))
    return fbx_stream_open_from_file(file, FBX_TRUE);

  // Couldn't open file.
  return NULL;
}

#endif

void fbx_stream_close(fbx_stream_t *stream)
{
  if (stream->free_on_close)
    fbx_free((void *)stream->buffer);

  fbx_free((void *)stream);
}

fbx_size_t fbx_stream_tell(fbx_stream_t *stream)
{
  return stream->cursor;
}

fbx_bool_t fbx_stream_seek(fbx_stream_t *stream,
                           fbx_size_t offset)
{
  if (offset > stream->size_of_buffer)
    // Cannot seek past end of buffer.
    return FBX_FALSE;

  stream->cursor = offset;

  return FBX_TRUE;
}

fbx_bool_t fbx_stream_skip(fbx_stream_t *stream,
                           fbx_size_t offset)
{
  if ((stream->size_of_buffer - stream->cursor) < offset)
    // Cannot skip past end of buffer.
    return FBX_FALSE;

  stream->cursor += offset;

  return FBX_TRUE;
}

fbx_size_t fbx_stream_read(fbx_stream_t *stream,
                           void *buffer,
                           fbx_size_t count)
{
  const fbx_size_t read = FBX_MIN(count, stream->size_of_buffer - stream->cursor);
  memcpy(buffer, (const void *)&stream->buffer[stream->cursor], read);
  stream->cursor += read;
  return read;
}

fbx_bool_t fbx_stream_exhausted(fbx_stream_t *stream)
{
  // Finished if at end of buffer.
  return (stream->cursor == stream->size_of_buffer);
}

// Boolean values are encoded as 1-bit values in the least significant bit of
// of a single byte.

static fbx_bool_t fbx_stream_read_bool(fbx_stream_t *stream, fbx_bool_t *value) {
  return (fbx_stream_read(stream, (void *)value, 1) == 1);
}

// Integer values are little endian.

// BUG(mtwilliams): These don't work on big endian machines.

static fbx_bool_t fbx_stream_read_int8(fbx_stream_t *stream, fbx_int8_t *value) {
  return (fbx_stream_read(stream, (void *)value, 1) == 1);
}

static fbx_bool_t fbx_stream_read_int16(fbx_stream_t *stream, fbx_int16_t *value) {
  return (fbx_stream_read(stream, (void *)value, 2) == 2);
}

static fbx_bool_t fbx_stream_read_int32(fbx_stream_t *stream, fbx_int32_t *value) {
  return (fbx_stream_read(stream, (void *)value, 4) == 4);
}

static fbx_bool_t fbx_stream_read_int64(fbx_stream_t *stream, fbx_int64_t *value) {
  return (fbx_stream_read(stream, (void *)value, 8) == 8);
}

static fbx_bool_t fbx_stream_read_uint8(fbx_stream_t *stream, fbx_uint8_t *value) {
  return (fbx_stream_read(stream, (void *)value, 1) == 1);
}

static fbx_bool_t fbx_stream_read_uint16(fbx_stream_t *stream, fbx_uint16_t *value) {
  return (fbx_stream_read(stream, (void *)value, 2) == 2);
}

static fbx_bool_t fbx_stream_read_uint32(fbx_stream_t *stream, fbx_uint32_t *value) {
  return (fbx_stream_read(stream, (void *)value, 4) == 4);
}

static fbx_bool_t fbx_stream_read_uint64(fbx_stream_t *stream, fbx_uint64_t *value) {
  return (fbx_stream_read(stream, (void *)value, 8) == 8);
}

// Single-precision and double-precision IEEE 754 numbers.

static fbx_bool_t fbx_stream_read_real32(fbx_stream_t *stream, fbx_real32_t *value) {
  return (fbx_stream_read(stream, (void *)value, 4) == 4);
}

static fbx_bool_t fbx_stream_read_real64(fbx_stream_t *stream, fbx_real64_t *value) {
  return (fbx_stream_read(stream, (void *)value, 8) == 8);
}

// HACK(mtwilliams): We're down casting to 32-bit width types when we encounter
// files using the newer format, rather than up casting from 32-bit width types
// when we encounter files using the original format. Th breaks for files
// larger than 4GiB. In practice this should not be an issue, but may bite us
// or someone else the future.

fbx_bool_t fbx_stream_read_uint64_as_uint32(fbx_stream_t *stream, fbx_uint32_t *value) {
  fbx_uint64_t wide;

  // Read into wide type.
  if (!fbx_stream_read_uint64(stream, &wide))
    return FBX_FALSE;

  // Downcast willy nilly.
  *value = wide;

  return FBX_TRUE;
}

// "Safe" variants of reading operations. If reading fails, these will early
// out and cause and unwinding.

#define fbx_stream_seek_s(Stream, Count) \
  if (!fbx_stream_seek(Stream, Count)) { \
    return FBX_FALSE;                    \
  }

#define fbx_stream_skip_s(Stream, Count) \
  if (!fbx_stream_skip(Stream, Count)) { \
    return FBX_FALSE;                    \
  }

#define fbx_stream_read_s(Stream, Buffer, Count)         \
  if (fbx_stream_read(Stream, Buffer, Count) != Count) { \
    return FBX_FALSE;                                    \
  }

#define fbx_stream_read_uint8_s(Stream, Value) \
  if (!fbx_stream_read_uint8(Stream, Value)) { \
    return FBX_FALSE;                          \
  }

#define fbx_stream_read_uint16_s(Stream, Value) \
  if (!fbx_stream_read_uint16(Stream, Value)) { \
    return FBX_FALSE;                           \
  }

#define fbx_stream_read_uint32_s(Stream, Value) \
  if (!fbx_stream_read_uint32(Stream, Value)) { \
    return FBX_FALSE;                           \
  }

#define fbx_stream_read_uint64_s(Stream, Value) \
  if (!fbx_stream_read_uint64(Stream, Value)) { \
    return FBX_FALSE;                           \
  }

#define fbx_stream_read_uint64_as_uint32_s(Stream, Value) \
  if (!fbx_stream_read_uint64_as_uint32(Stream, Value)) { \
    return FBX_FALSE;                                     \
  }

//  ____      _
// |    \ ___| |_ ___
// |  |  | .'|  _| .'|
// |____/|__,|_| |__,|

// TODO(mtwilliams): Templates.
// TODO(mtwilliams): Use templates to set defaults.
// TODO(mtwilliams): Automatically associate properties with nodes.

typedef enum fbx_datum_type {
  FBX_UNKNOWN_DATUM = 0,

  FBX_BOOLEAN_DATUM = 'C',
  FBX_INT16_DATUM = 'Y',
  FBX_INT32_DATUM = 'I',
  FBX_INT64_DATUM = 'L',
  FBX_REAL32_DATUM = 'F',
  FBX_REAL64_DATUM = 'D',

  FBX_ARRAY_OF_BOOLEAN_DATUM = 'b',
  FBX_ARRAY_OF_INT32_DATUM = 'i',
  FBX_ARRAY_OF_INT64_DATUM = 'l',
  FBX_ARRAY_OF_REAL32_DATUM = 'f',
  FBX_ARRAY_OF_REAL64_DATUM = 'd',

  FBX_STRING_DATUM = 'S',
  FBX_BINARY_DATUM = 'R'
} fbx_datum_type_t;

// Reference to data attached to a node.
typedef struct fbx_ref_to_data {
  fbx_uint32_t length;
  union {
    const void *pointer;
    const void *binary;
    const char *string;
  };
} fbx_ref_to_data_t;

typedef struct fbx_datum {
  union {
    fbx_datum_type_t type;
    fbx_uint8_t type_as_byte;
  };

  union {
    fbx_bool_t as_a_boolean;
    fbx_int16_t as_an_int16;
    fbx_int32_t as_an_int32;
    fbx_int64_t as_an_int64;
    fbx_real32_t as_a_real32;
    fbx_real64_t as_a_real64;
    fbx_ref_to_data_t as_a_ref;
  };
} fbx_datum_t;

#define BUMP(Pointer, Count) \
  (void *)(fbx_uintptr_t(Pointer) + (Count))

static fbx_bool_t fbx_extract_a_datum(const void *cursor,
                                      fbx_datum_t *datum,
                                      const void **updated)
{
  datum->type = FBX_UNKNOWN_DATUM;

  // Identify type from prefixed byte.
  datum->type_as_byte = *((fbx_uint8_t *)cursor);

  // Bump cursor to data.
  cursor = BUMP(cursor, 1);

  switch (datum->type) {
    // Primitives.
    case FBX_BOOLEAN_DATUM:
      datum->as_a_boolean = *(fbx_bool_t *)cursor;
      *updated = BUMP(cursor, 1);
      return FBX_TRUE;
    case FBX_INT16_DATUM:
      datum->as_an_int16 = *(fbx_int16_t *)cursor;
      *updated = BUMP(cursor, 2);
      return FBX_TRUE;
    case FBX_INT32_DATUM:
      datum->as_an_int32 = *(fbx_int32_t *)cursor;
      *updated = BUMP(cursor, 4);
      return FBX_TRUE;
    case FBX_INT64_DATUM:
      datum->as_an_int64 = *(fbx_int64_t *)cursor;
      *updated = BUMP(cursor, 8);
      return FBX_TRUE;
    case FBX_REAL32_DATUM:
      datum->as_a_real32 = *(fbx_real32_t *)cursor;
      *updated = BUMP(cursor, 4);
      return FBX_TRUE;
    case FBX_REAL64_DATUM:
      datum->as_a_real64 = *(fbx_real64_t *)cursor;
      *updated = BUMP(cursor, 8);
      return FBX_TRUE;

  #if 0
    // Arrays of primitives
    case FBX_ARRAY_OF_BOOLEAN_DATUM: return fbx_process_an_array(importer, FBX_BOOLEAN_DATUM);
    case FBX_ARRAY_OF_INT32_DATUM: return fbx_process_an_array(importer, FBX_INT32_DATUM);
    case FBX_ARRAY_OF_INT64_DATUM: return fbx_process_an_array(importer, FBX_INT64_DATUM);
    case FBX_ARRAY_OF_REAL32_DATUM: return fbx_process_an_array(importer, FBX_REAL32_DATUM);
    case FBX_ARRAY_OF_REAL64_DATUM: return fbx_process_an_array(importer, FBX_REAL64_DATUM);
  #endif

    // Strings and binaries.
    case FBX_STRING_DATUM:
    case FBX_BINARY_DATUM:
      datum->as_a_ref.length = *(fbx_uint32_t *)cursor;
      datum->as_a_ref.pointer = BUMP(cursor, 4);
      *updated = BUMP(cursor, 4 + datum->as_a_ref.length);
      return FBX_TRUE;
  }

  // Unknown type.
  return FBX_FALSE;
}

// Don't export internal helper macro.
#undef BUMP

static fbx_bool_t fbx_extract_a_datum_typed(fbx_datum_type_t type,
                                            const void *cursor,
                                            const void **updated,
                                            void *extracted)
{
  fbx_datum_t datum;

  if (!fbx_extract_a_datum(cursor, &datum, updated))
    // Failed to extract.
    return FBX_FALSE;

  if (datum.type != type)
    // Type mismatch.
    return FBX_FALSE;

  switch (type) {
    case FBX_BOOLEAN_DATUM:
      *(fbx_bool_t *)extracted = datum.as_a_boolean;
      return FBX_TRUE;
    case FBX_INT16_DATUM:
      *(fbx_int16_t *)extracted = datum.as_an_int16;
      return FBX_TRUE;
    case FBX_INT32_DATUM:
      *(fbx_int32_t *)extracted = datum.as_an_int32;
      return FBX_TRUE;
    case FBX_INT64_DATUM:
      *(fbx_int64_t *)extracted = datum.as_an_int64;
      return FBX_TRUE;
    case FBX_REAL32_DATUM:
      *(fbx_real32_t *)extracted = datum.as_a_real32;
      return FBX_TRUE;
    case FBX_REAL64_DATUM:
      *(fbx_real64_t *)extracted = datum.as_a_real64;
      return FBX_TRUE;

  #if 0
    case FBX_ARRAY_OF_BOOLEAN_DATUM: break;
    case FBX_ARRAY_OF_INT32_DATUM: break;
    case FBX_ARRAY_OF_INT64_DATUM: break;
    case FBX_ARRAY_OF_REAL32_DATUM: break;
    case FBX_ARRAY_OF_REAL64_DATUM: break;
  #endif

    case FBX_STRING_DATUM:
    case FBX_BINARY_DATUM:
      *(fbx_ref_to_data_t *)extracted = datum.as_a_ref;
      return FBX_TRUE;
  }

  return FBX_FALSE;
}

#define fbx_extract_a_datum_s(Type, Cursor, Extracted)                        \
  if (!fbx_extract_a_datum_typed(Type, Cursor, &Cursor, (void *)Extracted)) { \
    return FBX_FALSE;                                                         \
  }

#define fbx_extract_a_datum_from_node_s(Type, Node, Extracted)                \
  do {                                                                        \
    const void *cursor = Node->data;                                                \
    if (!fbx_extract_a_datum_typed(Type, cursor, &cursor, (void *)Extracted)) \
      return FBX_FALSE;                                                       \
  } while (0,0)

// Generated with the help of `scripts/fnv1a.c`.

typedef enum fbx_property_type {
  FBX_COMPOUND_PROPERTY = 0xe79b2efc,
  FBX_ENUM_PROPERTY     = 0x816cb000,
  FBX_BOOLEAN_PROPERTY  = 0xc894953d,
  FBX_INTEGER_PROPERTY  = 0x95e97e5e,
  FBX_FLOAT_PROPERTY    = 0xa6c45d85,
  FBX_DOUBLE_PROPERTY   = 0xa0eb0f08,
  FBX_STRING_PROPERTY   = 0x84006183,
  FBX_TIME_PROPERTY     = 0xc1d8710b,
  FBX_DATETIME_PROPERTY = 0x9bec7490,
  FBX_RGB_PROPERTY      = 0xff873b9b
} fbx_property_type_t;

static fbx_property_type_t fbx_property_type_by_name(const char *name,
                                                     size_t length)
{
  const fbx_uint8_t *start = (const fbx_uint8_t *)name;
  const fbx_uint8_t *end = start + length;

  fbx_uint32_t hash = 2166136261ul;

  for (const fbx_uint8_t *I = start; I != end; ++I)
    hash = (hash ^ *I) * 16777619ul;

  return (fbx_property_type_t)hash;
}

static const char *fbx_property_type_to_name(fbx_property_type_t type) {
  switch (type) {
    case FBX_COMPOUND_PROPERTY: return "Compound";
    case FBX_ENUM_PROPERTY: return "Enum";
    case FBX_BOOLEAN_PROPERTY: return "Boolean";
    case FBX_INTEGER_PROPERTY: return "Integer";
    case FBX_FLOAT_PROPERTY: return "Float";
    case FBX_DOUBLE_PROPERTY: return "Double";
    case FBX_STRING_PROPERTY: return "String";
    case FBX_TIME_PROPERTY: return "Time";
    case FBX_DATETIME_PROPERTY: return "DateTime";
    case FBX_RGB_PROPERTY: return "RGB";
  }

  return NULL;
}

typedef enum fbx_property_flag {
  // Signifies the property is user-defined; an "extension" in some sense.
  FBX_PROPERTY_IS_USER_DEFINED = (1 << 31)
} fbx_property_flag_t;

// Internal representation of properties attached to nodes.
typedef struct fbx_property {
  fbx_property_type_t type;

  const char *name;

  // Sub-type?
  const char *hint;

  fbx_uint32_t flags;

  union {
    fbx_ref_to_data_t as_a_ref;
    fbx_int32_t as_an_integer;
    fbx_real64_t as_a_number;
  } value;
} fbx_property_t;

// Internal representation of nodes encountered in `.fbx` files.
typedef struct fbx_node {
  // Unique identifier assigned during import to aid housekeeping.
  fbx_uint32_t id;

  // Name given to this node. May, on the rare occasion, be `NULL`.
  const char *name;

  // Data associated with this node.
  const void *data;

  // Number of elements comprising `data`.
  fbx_uint32_t num_of_datums;

  // Size of `data` in bytes.
  fbx_uint32_t size_of_data;

  // Properties associated with this node.
  const fbx_property_t *properties;

  // Number of properties associated with this node.
  fbx_uint32_t num_of_properties;

  // Parent node.
  struct fbx_node *parent;

  // First child.
  struct fbx_node *children;

  // Next sibling, i.e. next child of parent.
  struct fbx_node *sibling;
} fbx_node_t;

static const fbx_property_t *fbx_node_property_by_name(fbx_node_t *node,
                                                       const char *name)
{
  for (fbx_uint32_t property = 0; property < node->num_of_properties; ++property)
    if (strcmp(node->properties[property].name, name) == 0)
      return &node->properties[property];
  return NULL;
}

#define fbx_node_property_by_name_s(Node, Name, Property)    \
  if (!(Property = fbx_node_property_by_name(Node, Name))) { \
    return FBX_FALSE;                                        \
  }

static fbx_node_t *fbx_node_child_by_name(fbx_node_t *node,
                                          const char *name)
{
  for (fbx_node_t *child = node->children; child; child = child->sibling)
    if (strcmp(child->name, name) == 0)
      return child;
  return NULL;
}

#define fbx_node_child_by_name_s(Node, Name, Child)    \
  if (!(Child = fbx_node_child_by_name(Node, Name))) { \
    return FBX_FALSE;                                  \
  }

// TODO(mtwilliams): Build hash tables to reduce cost of looking up defaults.

typedef struct fbx_template {
  const char *name;

  const fbx_property_t *properties;
  fbx_uint32_t num_of_properties;

  // Node that defined this type.
  const fbx_node_t *definition;
} fbx_template_t;

//  _____                   _
// |     |_____ ___ ___ ___| |_
// |-   -|     | . | . |  _|  _|
// |_____|_|_|_|  _|___|_| |_|
//             |_|

// LEAK(mtwilliams): We're never freeing the permanent memory pool!

struct fbx_importer {
  const fbx_import_options_t *options;

  fbx_t *fbx;

  fbx_stream_t *stream;

  struct {
    fbx_block_t permanent;
    fbx_block_t transient;
    fbx_block_t strings;
  } memory;

  // Internal representation of hierarchy encountered in `.fbx`.
  fbx_node *nodes;
  fbx_uint32_t num_of_nodes;

  // Internal representation of types of object as defined in `.fbx`.
  const fbx_template_t *templates;
  fbx_uint32_t num_of_templates;

  // As of FBX 7.5 (also known as FBX 2016) nodes are described with 64-bit
  // width types (rather than the original 32-bit types) and empty nodes are
  // 25 bytes long (rather than the original 13 bytes). This tracks if the
  // newer format is used to adjust extraction logic accordingly.
  fbx_bool_t using_newer_format;

  // We reuse the same empty string since we encounter them so often.
  const char *an_empty_string;
};

static fbx_node_t *fbx_importer_alloc_a_node(fbx_importer_t *importer) {
  fbx_node_t *node =
    (fbx_node_t *)fbx_block_allocate_s(&importer->memory.transient,
                                       sizeof(fbx_node_t));

  // We assign a unique identifier to each node. This is used for internal
  // house keeping and such. Do not rely on it!
  node->id = ++importer->num_of_nodes;

  // Zero everything out.
  node->name = NULL;
  node->data = NULL;
  node->num_of_datums = 0;
  node->size_of_data = 0;
  node->properties = NULL;
  node->num_of_properties = 0;
  node->parent = node->children = node->sibling = NULL;

  return node;
}

static const char *fbx_importer_intern_a_string(fbx_importer_t *importer,
                                                const char *string,
                                                fbx_size_t length)
{
  if (length) {
    char *interned = (char *)fbx_block_allocate_s(&importer->memory.strings, length + 1);
    memcpy((void *)interned, (const void *)string, length);
    interned[length] = '\0';
    return interned;
  }

  // Empty string optimization.
  return importer->an_empty_string;
}

static const char *fbx_importer_intern_a_string_from_ref(fbx_importer_t *importer,
                                                         const fbx_ref_to_data_t ref)
{
  return fbx_importer_intern_a_string(importer, ref.string, ref.length);
}

static fbx_importer_t *fbx_importer_setup(const fbx_import_options_t *options)
{
  fbx_importer_t *importer =
    (fbx_importer_t *)fbx_allocate_s(sizeof(fbx_importer_t), 16);

  importer->options = options;

  importer->fbx = (fbx_t *)fbx_allocate_s(sizeof(fbx_t), 16);

  // Stream is bound by caller.
  importer->stream = NULL;

  // Allocate memory pools.
  importer->memory.permanent.base =  (fbx_uintptr_t)fbx_allocate_s(options->permanent_memory_pool, 16);
  importer->memory.permanent.offset = 0;
  importer->memory.permanent.size = options->permanent_memory_pool;
  importer->memory.transient.base =  (fbx_uintptr_t)fbx_allocate_s(options->transient_memory_pool, 16);
  importer->memory.transient.offset = 0;
  importer->memory.transient.size = options->transient_memory_pool;
  importer->memory.strings.base =  (fbx_uintptr_t)fbx_allocate_s(options->strings_memory_pool, 16);
  importer->memory.strings.offset = 0;
  importer->memory.strings.size = options->transient_memory_pool;

  // Set reasonable defaults.
  importer->nodes = NULL;
  importer->num_of_nodes = 0;
  importer->templates = NULL;
  importer->num_of_templates = 0;

  // Create a dummy root node as it makes various operations are lot easier.
  fbx_node_t *root = fbx_importer_alloc_a_node(importer);
  importer->nodes = root;

  // Assume older format until otherwise specified.
  importer->using_newer_format = FBX_FALSE;

  // We can encounter empty strings quite often. This saves 15 bytes of
  // overheard a few hundred times on average.
  char *an_empty_string = (char *)fbx_block_allocate_s(&importer->memory.strings, 1);
  an_empty_string[0] = '\0';
  importer->an_empty_string = an_empty_string;

  return importer;
}

static void fbx_importer_teardown(fbx_importer_t *importer)
{
}

#define FBX_MAGIC "Kaydara FBX Binary\x20\x20\x00\x1a\x00"

static fbx_bool_t fbx_check_for_magic(fbx_stream_t *stream)
{
  char magic[23];

  if (fbx_stream_read(stream, magic, 23) != 23)
    // File is too short (way too short) to be (binary) FBX.
    return FBX_FALSE;

  // If, and only if, the magic matches can we presume (binary) FBX.
  return (memcmp(magic, FBX_MAGIC, 23) == 0);
}

static fbx_bool_t fbx_extract_version_from_preamble(fbx_stream_t *stream,
                                                    fbx_uint32_t *version)
{
  if (fbx_stream_read(stream, (void *)version, 4) != 4)
    return FBX_FALSE;

  return FBX_TRUE;
}

#define fbx_importer_read_uint32_or_uint64(Importer, Value)      \
  if (Importer->using_newer_format) {                            \
    fbx_stream_read_uint64_as_uint32_s(Importer->stream, Value); \
  } else {                                                       \
    fbx_stream_read_uint32_s(Importer->stream, Value);           \
  }

static fbx_bool_t fbx_extract_any_properties(fbx_importer_t *importer,
                                             fbx_node_t *node)
{
  fbx_node_t *container = fbx_node_child_by_name(node, "Properties70");

  if (!container)
    // No properties associated with the node.
    return FBX_TRUE;

  fbx_property_t *properties = NULL;
  fbx_uint32_t num_of_properties = 0;

  // PERF(mtwilliams): Track number of children.
  for (fbx_node_t *child = container->children; child; child = child->sibling)
    num_of_properties += 1;

  properties =
    (fbx_property_t *)fbx_block_allocate_s(&importer->memory.transient,
                                           num_of_properties * sizeof(fbx_property_t));

  node->properties = properties;
  node->num_of_properties = num_of_properties;

  fbx_node_t *subcontainer = container->children;

  for (fbx_uint32_t property = 0; property < num_of_properties; ++property) {
    const void *cursor = subcontainer->data;

    fbx_ref_to_data_t ref_to_name;
    fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &ref_to_name);

    fbx_ref_to_data_t ref_to_type;
    fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &ref_to_type);

    properties[property].type = fbx_property_type_by_name(ref_to_type.string,
                                                          ref_to_type.length);

    properties[property].name =
      fbx_importer_intern_a_string_from_ref(importer, ref_to_name);

    // Appears to hint the sub-type?
    fbx_ref_to_data_t ref_to_hint;
    fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &ref_to_hint);

    if (ref_to_hint.length > 0) {
      properties[property].hint =
        fbx_importer_intern_a_string_from_ref(importer, ref_to_hint);
    } else {
      properties[property].hint = NULL;
    }

    fbx_ref_to_data_t ref_to_flags;
    fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &ref_to_flags);

    properties[property].flags = 0x00000000;

    for (fbx_uint32_t flag = 0; flag < ref_to_flags.length; ++flag) {
      switch (ref_to_flags.string[flag]) {
        // TODO(mtwilliams): Locked & Muted.
        case 'A': /* TODO(mtwilliams): Animatable flag. */ break;
        case '+': /* TODO(mtwilliams): Animated flag. May need to check for `A+`? */ break;
        case 'H': /* TODO(mtwilliams): Hidden flag. */ break;
        case 'U': properties[property].flags |= FBX_PROPERTY_IS_USER_DEFINED; break;
      }
    }

    switch (properties[property].type) {
      case FBX_COMPOUND_PROPERTY:
        break;

      case FBX_STRING_PROPERTY:
        fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &properties[property].value.as_a_ref);
        break;

      case FBX_INTEGER_PROPERTY:
        fbx_extract_a_datum_s(FBX_INT32_DATUM, cursor, &properties[property].value.as_an_integer);
        break;

      case FBX_FLOAT_PROPERTY:
        fbx_real32_t as_a_real32;
        fbx_extract_a_datum_s(FBX_REAL32_DATUM, cursor, &as_a_real32);
        properties[property].value.as_a_number = as_a_real32;
        break;

      case FBX_DOUBLE_PROPERTY:
        fbx_real64_t as_a_real64;
        fbx_extract_a_datum_s(FBX_REAL64_DATUM, cursor, &as_a_real64);
        properties[property].value.as_a_number = as_a_real64;
        break;
    }

    subcontainer = subcontainer->sibling;
  }

  return FBX_TRUE;
}

static fbx_bool_t fbx_extract_a_node(fbx_importer_t *importer,
                                     fbx_node_t **ptr_to_node)
{
  *ptr_to_node = NULL;

  // Determine absolute offest to the start of this node.
  fbx_uint32_t start = fbx_stream_tell(importer->stream);

  // Then read the absolute offest to the end of this node.
  fbx_uint32_t end;
  fbx_importer_read_uint32_or_uint64(importer, &end);

  if (end == 0) {
    // Empty node.
    const fbx_size_t width = (importer->using_newer_format ? 25 : 13);
    return fbx_stream_seek(importer->stream, start + width);
  }

#if 0
  // We can now derive the total length of this node in bytes.
  fbx_uint32_t length = end - start;
#endif

  // Read the number of datums associated with this node.
  fbx_uint32_t num_of_datums;
  fbx_importer_read_uint32_or_uint64(importer, &num_of_datums);

  // Read the size, in bytes, used to store all data associated this node.
  fbx_uint32_t size_of_data;
  fbx_importer_read_uint32_or_uint64(importer, &size_of_data);

  // Read the length, in characters, of the name of this node. Names are not
  // null terminated, so we need to be careful.
  fbx_uint8_t length_of_name;
  fbx_stream_read_uint8_s(importer->stream, &length_of_name);

  // Names are not null-terminated.
  char name[255+1] = { 0, };
  fbx_stream_read_s(importer->stream, (void *)name, length_of_name);

  fbx_node_t *node = fbx_importer_alloc_a_node(importer);

  *ptr_to_node = node;

  if (length_of_name > 0) {
    // Copy name for posterity.
    node->name = (const char *)fbx_block_allocate_s(&importer->memory.strings, length_of_name + 1);
    memcpy((void *)node->name, (const void *)name, length_of_name + 1);
  }

  if (size_of_data > 0) {
    // Copy data for procesing at higher level.
    node->data = fbx_block_allocate_s(&importer->memory.transient, size_of_data);
    fbx_stream_read_s(importer->stream, (void *)node->data, size_of_data);
    node->num_of_datums = num_of_datums;
    node->size_of_data = size_of_data;
  }

  // A node has children if there are left over bytes.
  const fbx_bool_t children = (fbx_stream_tell(importer->stream) < end);

  if (children) {
    fbx_node_t **child = &node->children;

    for (;;) {
      if (fbx_extract_a_node(importer, *child ? &(*child)->sibling : child)) {
        if (*child == NULL)
          // Empty node delineates end of children by acting as sentinel.
          break;
        // Link child to parent.
        (*child)->parent = node;
        // Possibly more...
        child = &(*child)->sibling;
      } else {
        // Failed to process a child.
        return FBX_FALSE;
      }
    }

    // Extract properties if any exist.
    fbx_extract_any_properties(importer, node);
  }

  // Skip the sentinel.
  fbx_stream_seek(importer->stream, end);

  return FBX_TRUE;
}

// TODO(mtwilliams): Replace `fbx_panic` with logging functionality tied to
// importer or entire library.

// Free backing memory used to constitute the internal node hierarchy.
// Validate and clean up data.
 // Includes triangulation the like.

static fbx_bool_t fbx_importer_extract(fbx_importer_t *importer) {
  fbx_node_t **node = &importer->nodes->children;

  for (;;) {
    if (fbx_extract_a_node(importer, *node ? &(*node)->sibling : node)) {
      if (*node == NULL)
        // Empty node signifies end.
        break;
      node = &(*node)->sibling;
    } else {
      return FBX_FALSE;
    }
  }

  return FBX_FALSE;
}

#define FBX_HEADER_VERSION 1003

static fbx_bool_t fbx_process_a_header_version_node(fbx_importer_t *importer,
                                                    fbx_node_t *header_version_node)
{
  const void *cursor = header_version_node->data;

  fbx_uint32_t version;
  fbx_extract_a_datum_s(FBX_INT32_DATUM, cursor, &version);

  if (version != FBX_HEADER_VERSION)
    return FBX_FALSE;

  return FBX_TRUE;
}

static fbx_bool_t fbx_process_a_version_node(fbx_importer_t *importer,
                                             fbx_node_t *version_node)
{
  const void *cursor = version_node->data;

  fbx_uint32_t version;
  fbx_extract_a_datum_s(FBX_INT32_DATUM, cursor, &version);

#if 0
  // We assume the header is canonical.
  if (importer->fbx->version != version)
    fbx_importer_warning("Header and extended header versions don't match?");
#endif

  return FBX_TRUE;
}

static fbx_bool_t fbx_process_a_creation_timestamp_node(fbx_importer_t *importer,
                                                        fbx_node_t *creation_timestamp_node)
{
  fbx_node_t *year_node,
             *month_node,
             *day_node,
             *hour_node,
             *minute_node,
             *second_node,
             *millisecond_node;

  fbx_node_child_by_name_s(creation_timestamp_node, "Year", year_node);
  fbx_node_child_by_name_s(creation_timestamp_node, "Month", month_node);
  fbx_node_child_by_name_s(creation_timestamp_node, "Day", day_node);
  fbx_node_child_by_name_s(creation_timestamp_node, "Hour", hour_node);
  fbx_node_child_by_name_s(creation_timestamp_node, "Minute", minute_node);
  fbx_node_child_by_name_s(creation_timestamp_node, "Second", second_node);
  fbx_node_child_by_name_s(creation_timestamp_node, "Millisecond", millisecond_node);

  fbx_extract_a_datum_from_node_s(FBX_INT32_DATUM, year_node, &importer->fbx->timestamp.year);
  fbx_extract_a_datum_from_node_s(FBX_INT32_DATUM, month_node, &importer->fbx->timestamp.month);
  fbx_extract_a_datum_from_node_s(FBX_INT32_DATUM, day_node, &importer->fbx->timestamp.day);
  fbx_extract_a_datum_from_node_s(FBX_INT32_DATUM, hour_node, &importer->fbx->timestamp.hour);
  fbx_extract_a_datum_from_node_s(FBX_INT32_DATUM, minute_node, &importer->fbx->timestamp.minute);
  fbx_extract_a_datum_from_node_s(FBX_INT32_DATUM, second_node, &importer->fbx->timestamp.second);
  fbx_extract_a_datum_from_node_s(FBX_INT32_DATUM, millisecond_node, &importer->fbx->timestamp.millisecond);

  return FBX_TRUE;
}

static fbx_bool_t fbx_process_a_metadata_node(fbx_importer_t *importer,
                                               fbx_node_t *metadata_node)
{
  fbx_node_t *title_node,
             *subject_node,
             *author_node,
             *keywords_node,
             *revision_node,
             *comment_node;

  fbx_node_child_by_name_s(metadata_node, "Title", title_node);
  fbx_node_child_by_name_s(metadata_node, "Subject", subject_node);
  fbx_node_child_by_name_s(metadata_node, "Author", author_node);
  fbx_node_child_by_name_s(metadata_node, "Keywords", keywords_node);
  fbx_node_child_by_name_s(metadata_node, "Revision", revision_node);
  fbx_node_child_by_name_s(metadata_node, "Comment", comment_node);

  // TODO(mtwilliams): Determine if keywords are given as one string per
  // keyword or as a delimiter separated list.

  fbx_ref_to_data_t ref_to_title,
                    ref_to_subject,
                    ref_to_author,
                    ref_to_keywords,
                    ref_to_revision,
                    ref_to_comment;

  fbx_extract_a_datum_from_node_s(FBX_STRING_DATUM, title_node, &ref_to_title);
  fbx_extract_a_datum_from_node_s(FBX_STRING_DATUM, subject_node, &ref_to_subject);
  fbx_extract_a_datum_from_node_s(FBX_STRING_DATUM, author_node, &ref_to_author);
  fbx_extract_a_datum_from_node_s(FBX_STRING_DATUM, keywords_node, &ref_to_keywords);
  fbx_extract_a_datum_from_node_s(FBX_STRING_DATUM, revision_node, &ref_to_revision);
  fbx_extract_a_datum_from_node_s(FBX_STRING_DATUM, comment_node, &ref_to_comment);

  importer->fbx->metadata.title = fbx_importer_intern_a_string_from_ref(importer, ref_to_title);
  importer->fbx->metadata.subject = fbx_importer_intern_a_string_from_ref(importer, ref_to_subject);
  importer->fbx->metadata.author = fbx_importer_intern_a_string_from_ref(importer, ref_to_author);
  importer->fbx->metadata.keywords = fbx_importer_intern_a_string_from_ref(importer, ref_to_keywords);
  importer->fbx->metadata.revision = fbx_importer_intern_a_string_from_ref(importer, ref_to_revision);
  importer->fbx->metadata.comment = fbx_importer_intern_a_string_from_ref(importer, ref_to_comment);

  return FBX_TRUE;
}

static fbx_bool_t fbx_process_a_scene_info_node(fbx_importer_t *importer,
                                                fbx_node_t *scene_info_node)
{
  const fbx_property_t *app_vendor_property,
                       *app_name_property,
                       *app_version_property;

  if (app_vendor_property = fbx_node_property_by_name(scene_info_node, "LastSaved|ApplicationVendor")) {
    importer->fbx->tool.vendor =
      fbx_importer_intern_a_string_from_ref(importer, app_vendor_property->value.as_a_ref);
  } else {
    importer->fbx->tool.vendor = importer->an_empty_string;
  }

  if (app_name_property = fbx_node_property_by_name(scene_info_node, "LastSaved|ApplicationName")) {
    importer->fbx->tool.name =
      fbx_importer_intern_a_string_from_ref(importer, app_name_property->value.as_a_ref);
  } else {
    importer->fbx->tool.name = importer->an_empty_string;
  }

  if (app_version_property = fbx_node_property_by_name(scene_info_node, "LastSaved|ApplicationVersion")) {
    importer->fbx->tool.version =
      fbx_importer_intern_a_string_from_ref(importer, app_version_property->value.as_a_ref);
  } else {
    importer->fbx->tool.version = importer->an_empty_string;
  }

  fbx_node_t *metadata_node;
  fbx_node_child_by_name_s(scene_info_node, "MetaData", metadata_node);

  if (!fbx_process_a_metadata_node(importer, metadata_node))
    return FBX_FALSE;

  return FBX_TRUE;
}

static fbx_bool_t fbx_process_a_creator_node(fbx_importer_t *importer,
                                             fbx_node_t *creator_node)
{
  fbx_ref_to_data_t ref_to_exporter;
  fbx_extract_a_datum_from_node_s(FBX_STRING_DATUM, creator_node, &ref_to_exporter);

  importer->fbx->exporter.name = fbx_importer_intern_a_string_from_ref(importer, ref_to_exporter);

  return FBX_TRUE;
}

static fbx_bool_t fbx_process_a_header_extension_node(fbx_importer_t *importer,
                                                      fbx_node_t *header_extension_node)
{
  fbx_node_t *header_version_node,
             *version_node;

  fbx_node_child_by_name_s(header_extension_node, "FBXHeaderVersion", header_version_node);
  fbx_node_child_by_name_s(header_extension_node, "FBXVersion", version_node);

  // Check header version.
  if (!fbx_process_a_header_version_node(importer, header_version_node))
    return FBX_FALSE;

  // Validate version information against file header.
  if (!fbx_process_a_version_node(importer, version_node))
    return FBX_FALSE;

  fbx_node_t *creation_timestamp_node;
  fbx_node_child_by_name_s(header_extension_node, "CreationTimeStamp", creation_timestamp_node);

  // Extract timestamp.
  if (!fbx_process_a_creation_timestamp_node(importer, creation_timestamp_node))
    return FBX_FALSE;

  fbx_node_t *scene_info_node,
             *creator_node;

  fbx_node_child_by_name_s(header_extension_node, "SceneInfo", scene_info_node);
  fbx_node_child_by_name_s(header_extension_node, "Creator", creator_node);

  // Extract metadata.
  if (!fbx_process_a_scene_info_node(importer, scene_info_node))
    return FBX_FALSE;
  if (!fbx_process_a_creator_node(importer, creator_node))
    return FBX_FALSE;

  return FBX_TRUE;
}

static fbx_vec3_t fbx_basis_from_axis_and_sign(fbx_int32_t axis,
                                               fbx_int32_t sign)
{
  switch (axis) {
    case 0: return { sign ? 1.f : -1.f, 0.f, 0.f };
    case 1: return { 0.f, sign ? 1.f : -1.f, 0.f };
    case 2: return { 0.f, 0.f, sign ? 1.f : -1.f };
  }

  return { 0.f, 0.f, 0.f };
}

// TODO(mtwilliams): Determine if basis is correct.
// TODO(mtwilliams): Derive units-per-meter?

static fbx_bool_t fbx_process_a_global_settings_node(fbx_importer_t *importer,
                                                     fbx_node_t *global_settings_node)
{
  const fbx_property_t *up_axis_property,
                       *up_axis_sign_property,
                       *front_axis_property,
                       *front_axis_sign_property,
                       *coord_axis_property,
                       *coord_axis_sign_property;

  fbx_node_property_by_name_s(global_settings_node, "UpAxis", up_axis_property);
  fbx_node_property_by_name_s(global_settings_node, "UpAxisSign", up_axis_sign_property);
  fbx_node_property_by_name_s(global_settings_node, "FrontAxis", front_axis_property);
  fbx_node_property_by_name_s(global_settings_node, "FrontAxisSign", front_axis_sign_property);
  fbx_node_property_by_name_s(global_settings_node, "CoordAxis", coord_axis_property);
  fbx_node_property_by_name_s(global_settings_node, "CoordAxisSign", coord_axis_sign_property);

  const fbx_int32_t up_axis = up_axis_property->value.as_an_integer;
  const fbx_int32_t up_axis_sign = up_axis_sign_property->value.as_an_integer;

  importer->fbx->basis.up = fbx_basis_from_axis_and_sign(up_axis, up_axis_sign);

  const fbx_int32_t front_axis = front_axis_property->value.as_an_integer;
  const fbx_int32_t front_axis_sign = front_axis_sign_property->value.as_an_integer;

  importer->fbx->basis.forward = fbx_basis_from_axis_and_sign(front_axis, !front_axis_sign);

  const fbx_int32_t coord_axis = coord_axis_property->value.as_an_integer;
  const fbx_int32_t coord_axis_sign = coord_axis_sign_property->value.as_an_integer;

  importer->fbx->basis.right = fbx_basis_from_axis_and_sign(coord_axis, coord_axis_sign);

  // HACK(mtwilliams): Assume origin at (0, 0, 0).
  importer->fbx->origin = { 0.f, 0.f, 0.f };

  return FBX_TRUE;
}

static fbx_bool_t fbx_process_any_definitions(fbx_importer_t *importer,
                                              fbx_node_t *definitions_node)
{
  // We have to count the number of definitions, because unlike the name would
  // imply, the "Count" node refers to the number of objects (using the
  // definitions?) rather than the number of definitions themselves? This might
  // very well be a bug that's been solidified in the name of backwards
  // compatibility. Sigh.

  fbx_uint32_t count = 0;

  for (fbx_node_t *child = definitions_node->children; child; child = child->sibling)
    if (strcmp(child->name, "ObjectType") == 0)
      count += 1;

  fbx_template_t *templates =
    (fbx_template_t *)fbx_block_allocate_s(&importer->memory.transient,
                                           count * sizeof(fbx_template_t));

  importer->templates = templates;
  importer->num_of_templates = count;

  fbx_uint32_t encountered = 0;

  for (fbx_node_t *definition_node = definitions_node->children; definition_node; definition_node = definition_node->sibling) {
    if (strcmp(definition_node->name, "ObjectType") != 0)
      // Only care about definitions.
      continue;

    fbx_template_t *tmpl = &templates[encountered++];

    tmpl->definition = definition_node;

    fbx_ref_to_data_t ref_to_name;
    fbx_extract_a_datum_from_node_s(FBX_STRING_DATUM, definition_node, &ref_to_name);

    tmpl->name = fbx_importer_intern_a_string_from_ref(importer, ref_to_name);

    // Appears that some definitions don't provide a template at all?
    if (fbx_node_t *template_node = fbx_node_child_by_name(definition_node, "PropertyTemplate")) {
      tmpl->properties = template_node->properties;
      tmpl->num_of_properties = template_node->num_of_properties;
    } else {
      tmpl->properties = NULL;
      tmpl->num_of_properties = 0;
    }
  }

  return FBX_TRUE;
}

static fbx_bool_t fbx_importer_process(fbx_importer_t *importer) {
  fbx_node_t *root = importer->nodes;

  // TODO(mtwilliams): Use `fbx_node_child_by_name_s` to find the header
  // extension?

  fbx_node_t *header_extension_node = root->children;

  // First node should be a header extension.
  if (strcmp(header_extension_node->name, "FBXHeaderExtension") != 0)
    return FBX_FALSE;

  // Process extended header to gather metadata.
  if (!fbx_process_a_header_extension_node(importer, header_extension_node))
    return FBX_FALSE;

  // Definitions define types of objects and default properties to associate
  // with each instance. However, they're optional, so there may be no
  // "templates" and only given properties to go on, so beware!
  if (fbx_node_t *definitions_node = fbx_node_child_by_name(root, "Definitions")) {
    if (!fbx_process_any_definitions(importer, definitions_node))
      return FBX_FALSE;
  } else {
    importer->templates = NULL;
    importer->num_of_templates = 0;
  }

  fbx_node_t *global_settings_node;
  fbx_node_child_by_name_s(root, "GlobalSettings", global_settings_node);

  // Process global settings to determine basis and origin.
  if (!fbx_process_a_global_settings_node(importer, global_settings_node))
    return FBX_FALSE;

  return FBX_TRUE;
}

static fbx_bool_t fbx_importer_run(fbx_importer_t *importer) {
  if (!fbx_check_for_magic(importer->stream))
    fbx_panic("Not a valid `.fbx` file?");

  if (!fbx_extract_version_from_preamble(importer->stream, &importer->fbx->version))
    fbx_panic("Unexpected end of stream!");

  if (importer->fbx->version < 7100)
    fbx_panic("Cannot import because the given file uses an outdated version of FBX!");

  if (importer->fbx->version > 7500)
    fbx_panic("Cannot import because the given file uses a newer version of FBX!");

  if (importer->fbx->version >= 7500)
    // See `fbx_importer_t` for a description of binary incompatible changes
    // introduced in FBX 2016.
    importer->using_newer_format = FBX_TRUE;
  else
    importer->using_newer_format = FBX_FALSE;

  // Extract node hierarchy from stream into internal representation.
  fbx_importer_extract(importer);

  // Walk internal representation to process information into structures.
  fbx_importer_process(importer);

  return FBX_TRUE;
}

//  _____                 _
// |   __|_ _ ___ ___ ___| |_
// |   __|_'_| . | . |  _|  _|
// |_____|_,_|  _|___|_| |_|
//           |_|

// ...

FBX_END_EXTERN_C

#endif
