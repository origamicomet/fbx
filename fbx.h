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
    /* HACK(mtwilliams): We assume that we're being compiled with GCC. */
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
    /* Realistically, we could support tvOS... */
    #error ("We don't support tvOS!")
  #else
    #error ("We don't support this platform!")
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
  /* Ostensibly... */
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
  #define FBX_EXPORT(Return) Return __stdcall
  #define FBX_CALLBACK(Return) Return __cdecl
#elif FBX_ON_MAC
  #define FBX_EXPORT(Return) Return
  #define FBX_CALLBACK(Return) Return
#elif FBX_ON_LINUX
  #define FBX_EXPORT(Return) Return
  #define FBX_CALLBACK(Return) Return
#elif FBX_ON_ANDROID
  #define FBX_EXPORT(Return) Return
  #define FBX_CALLBACK(Return) Return
#elif FBX_ON_IOS
  #define FBX_EXPORT(Return) Return
  #define FBX_CALLBACK(Return) Return
#endif

#if defined(__cplusplus)
  #define FBX_BEGIN_EXTERN_C extern "C" {
  #define FBX_END_EXTERN_C }
#else
  #define FBX_BEGIN_EXTERN_C
  #define FBX_END_EXTERN_C
#endif

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

/* We (have to) define our own boolean type regardless of compiler support
   to prevent its width from being changed on us. We use this throughout the
   codebase. Be careful not mix our type with others, since it's not
   compatible! */
typedef fbx_uint32_t fbx_bool_t;

#define FBX_TRUE  ((fbx_bool_t)1)
#define FBX_FALSE ((fbx_bool_t)0)

/* This will cause a negative subscript error if the sizes of our types don't
   match our expectations. */
#define FBX_CHECK_SIZE_OF_TYPE(expression) \
  typedef char fbx__size_matches_expectation[(expression) ? 1 : -1]

/* Ensure fixed-width types match our expectations. */
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

/* Ensure pointer-width integer types can actually hold pointers. */
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_intptr_t) == sizeof(void *));
FBX_CHECK_SIZE_OF_TYPE(sizeof(fbx_uintptr_t) == sizeof(void *));

#undef FBX_CHECK_SIZE_OF_TYPE

typedef enum fbx_status {
  FBX_OK       =  0,
  FBX_EMEMORY  = -1, /* Out of memory. */
  FBX_ESPACE   = -2, /* Ran out of space. */
  FBX_EFORMAT  = -3, /* Malformed. */
  FBX_EVERSION = -4, /* Unsupported version. */
  FBX_EFEATURE = -5, /* Unsupported feature. */
  FBX_EDATA    = -6, /* Bad or insufficent data. */
} fbx_status_t;

/* TODO(mtwilliams): Free-standing. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*  _____     ___     _
 * |   __|___|  _|___| |_ _ _
 * |__   | .'|  _| -_|  _| | |
 * |_____|__,|_| |___|_| |_  |
 *                       |___|
 */

/* This library will panic if something goes horribly wrong. This will seldom
   happen but you should still install a handler as it will allow you to get 
   some information about what went wrong. */

typedef struct fbx_panic_info {
  /* File where the panic occured. */
  const char *file;
  /* Line in file where the panic occured. */
  unsigned line;
} fbx_panic_info_t;

typedef FBX_CALLBACK(void)
  fbx_panic_handler_fn(const char *reason,
                       const fbx_panic_info_t *info);

extern FBX_EXPORT(void)
  fbx_get_panic_handler(fbx_panic_handler_fn **handler);

extern FBX_EXPORT(void)
  fbx_set_panic_handler(fbx_panic_handler_fn *handler);

/*  _____
 * |     |___ _____ ___ ___ _ _
 * | | | | -_|     | . |  _| | |
 * |_|_|_|___|_|_|_|___|_| |_  |
 *                         |___|
 */

typedef struct fbx_allocation_info {
  /* File where allocation was requested. */
  const char *file;
  /* Line in file where allocation was requested. */
  unsigned line;
  /* Optional tag indicating type or reason for allocation. */
  const char *tag;
} fbx_allocation_info_t;

typedef FBX_CALLBACK(void *)
  fbx_allocate_callback_fn(fbx_size_t size,
                           fbx_size_t alignment,
                           const fbx_allocation_info_t *info);

typedef FBX_CALLBACK(void)
  fbx_deallocate_callback_fn(void *ptr);

extern FBX_EXPORT(void)
  fbx_get_allocator(fbx_allocate_callback_fn **allocate,
                    fbx_deallocate_callback_fn **deallocate);

extern FBX_EXPORT(void)
  fbx_set_allocator(fbx_allocate_callback_fn *allocate,
                    fbx_deallocate_callback_fn *deallocate);

/*  _____   _ _____
 * |     | / |     |
 * |-   -|/ /|  |  |
 * |_____|_/ |_____|
 */

/* A stream of bytes. */
typedef struct fbx_stream fbx_stream_t;

/* Wraps a buffer as a stream. */
extern FBX_EXPORT(fbx_stream_t *)
  fbx_stream_open_from_memory(void *buffer,
                              fbx_size_t size,
                              const char *flags);

#ifndef FBX_NO_STANDARD_LIBRARY

/* Wraps file in a stream. */
extern FBX_EXPORT(fbx_stream_t *)
  fbx_stream_open_from_file(FILE *file);

/* Opens a file at a given path and wraps it in a stream. */
extern FBX_EXPORT(fbx_stream_t *)
  fbx_stream_open_from_path(const char *path,
                            const char *flags);

#endif

/* Closes a stream. */
extern FBX_EXPORT(void)
  fbx_stream_close(fbx_stream_t *stream);

/* Returns absolute position in a stream. */
extern FBX_EXPORT(fbx_size_t)
  fbx_stream_tell(fbx_stream_t *stream);

/* Moves to an absolute offset in a stream. Returns `FBX_TRUE` if seek was
   succesful or `FBX_FALSE` if not. */
extern FBX_EXPORT(fbx_bool_t)
  fbx_stream_seek(fbx_stream_t *stream,
                  fbx_size_t offset);

/* Skips through a stream. Returns `FBX_TRUE` if skip was successful, or
   `FBX_FALSE` otherwise if not. */
extern FBX_EXPORT(fbx_bool_t)
  fbx_stream_skip(fbx_stream_t *stream,
                  fbx_size_t amount);

#ifdef FBX_NO_IMPORT

/* Reads up to some number of bytes into a buffer. Returns the number of
   bytes actually read. May be less than requested if the stream is
   exhausted prior to completion. */
extern FBX_EXPORT(fbx_size_t)
  fbx_stream_read(fbx_stream_t *stream,
                  void *buffer,
                  fbx_size_t count);

#endif

#ifdef FBX_NO_EXPORT

/* Writes up to some number of bytes from a buffer. Returns the number of
   bytes actually written. May be less than requested if the stream is
   filled prior to completion. */
extern FBX_EXPORT(fbx_size_t)
  fbx_stream_write(fbx_stream_t *stream,
                   const void *buffer,
                   fbx_size_t count);

#endif

/* Returns `FBX_TRUE` if a stream is exhausted, or `FBX_FALSE` if not. */
extern FBX_EXPORT(fbx_bool_t)
  fbx_stream_exhausted(fbx_stream_t *stream);

/*  _____     _   _
 * |     |___| |_| |_
 * | | | | .'|  _|   |
 * |_|_|_|__,|_| |_|_|
 */

/* Ratio of a circle's circumference to its diameter. */
#define FBX_PI 3.14159265358979323846264338327950288f

/* Twice as good as Pi */
#define FBX_TAU (2.f * FBX_PI)

/* Converts degrees to radians. */
static fbx_real32_t fbx_degrees_to_radians(const fbx_real32_t degrees) {
  return degrees * (180.f / FBX_PI);
}

/* Converts radians to degrees. */
static fbx_real32_t fbx_radians_to_degrees(const fbx_real32_t radians) {
  return radians * (FBX_PI / 180.f);
}

/* Represents a point or a direction in two-dimensional space. */
typedef struct fbx_vec2 { fbx_real32_t x, y; } fbx_vec2_t;

/* Represents a point or a direction in three-dimensional space. */
typedef struct fbx_vec3 { fbx_real32_t x, y, z; } fbx_vec3_t;

/* Represents a point or a direction in four-dimensional space. */
typedef struct fbx_vec4 { fbx_real32_t x, y, z, w; } fbx_vec4_t;

/* Represents a direction (and optionally, a magnitude). */
typedef struct fbx_quaternion {
  fbx_real32_t x, y, z, w;
} fbx_quaternion_t;

/* Identity quaternion. */
extern const fbx_quaternion_t FBX_IDENTITY_QUATERNION;

/* Represents a linear transformation of three-dimensional space.
 *
 * Note that we store matrices in column major order. This means columns are
 * laid our contiguously in memory. We also uses columns vectors when
 * multiplying matrices and vectors, meaning transformations are applied
 * right-to-left.
 */
typedef struct fbx_mat4 {
  fbx_real32_t v[4][4];
} fbx_mat4_t;

/* Zero matrix. */
extern const fbx_mat4_t FBX_ZERO_MATRIX;

/* Identity matrix. */
extern const fbx_mat4_t FBX_IDENTITY_MATRIX;

/*  _____     _
 * |     |___| |___ ___
 * |   --| . | | . |  _|
 * |_____|___|_|___|_|
 */

/* TODO(mtwilliams): HSL, HSV, and YUV? */

/* Represents a color. */
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

/*  ____      _
 * |    \ ___| |_ ___
 * |  |  | .'|  _| .'|
 * |____/|__,|_| |__,|
 */

/* Bindings? */
/* Custom properties? */

typedef enum fbx_object_type {
  FBX_UNKNOWN = 0,
  FBX_EMPTY   = 1,
  FBX_MODEL   = 2,
  FBX_MESH    = 3
} fbx_object_type_t;

typedef struct fbx_object {
  fbx_uint64_t id;
  const char *name;
  fbx_object_type_t type;
  void *reified;
  struct fbx_object *parent;
  struct fbx_object **children;
  fbx_uint32_t num_of_children;
} fbx_object_t;

typedef struct fbx_material fbx_material_t;
typedef struct fbx_texture fbx_texture_t;

typedef struct fbx_transform fbx_transform_t;

/* TODO(mtwilliams): Deal with geometric transformations to support 3DS Max. */

typedef struct fbx_transform {
  /* Position relative to parent. */
  fbx_vec3_t position;
  
  /* Rotation relative to parent. */
  fbx_quaternion_t rotation;

  /* Center of rotation. */
  fbx_vec3_t center_of_rotation;

  /* Point rotated around. */
  fbx_vec3_t point_of_rotation;

  /* Scale relative to parent. */
  fbx_vec3_t scale;

  /* Center of scaling. */
  fbx_vec3_t center_of_scaling;

  /* Point scaled around. */
  fbx_vec3_t point_of_scaling;
} fbx_transform_t;

/* Scaled, then rotated, then translated. */

typedef struct fbx_model {
  fbx_transform_t transform;
} fbx_model_t;

typedef enum fbx_topology {
  FBX_POINT    = 1,
  FBX_TRIANGLE = 3,
  FBX_QUADS    = 4
} fbx_topology_t;

typedef struct fbx_mesh {
  /* TODO(mtwilliams): Provide vertex format. */
  void * const* streams;

  /* De facto... */
  const void *positions;

  const fbx_uint32_t *indices;

  /* Number of vertices composing geometry. */
  fbx_uint32_t num_of_vertices;

  /* Number of edges in geometry. */
  fbx_uint32_t num_of_edges;

  /* Number of triangles composing geometry. */
  fbx_uint32_t num_of_faces;
} fbx_mesh_t;


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

/* TODO(mtwilliams): Units per meter? */

typedef struct fbx_scene {
  const fbx_object_t *root;
  fbx_object_t * const *objects;
  fbx_uint32_t num_of_objects;
} fbx_scene_t;

typedef struct fbx {
  fbx_uint32_t version;

  fbx_metadata_t metadata;

  fbx_tool_info_t tool;
  fbx_exporter_info_t exporter;

  fbx_timestamp_t timestamp;

  fbx_vec3_t origin;
  fbx_basis_t basis;

  fbx_scene_t scene;
} fbx_t;

/*  _____                   _
 * |     |_____ ___ ___ ___| |_
 * |-   -|     | . | . |  _|  _|
 * |_____|_|_|_|  _|___|_| |_|
 *             |_|
 */

/* TODO(mtwilliams): Coordinate spaces. */
/* TODO(mtwilliams): Normal smoothing. */
/* TODO(mtwilliams): Triangulation. */

typedef struct fbx_import_options {
  /* The amount of memory to dedicate to permanent allocations. The permanent
     pool is only freed when the imported scene is freed. */
  fbx_size_t permanent_memory_pool;

  /* The amount of memory to dedicate to transient allocations. The transient
     pool is freed after importing. */
  fbx_size_t transient_memory_pool;

  /* The amount of memory to dedicate to string allocations. The string pool
     is only freed when the imported document is freed. */
  fbx_size_t strings_memory_pool;
} fbx_import_options_t;

typedef struct fbx_importer fbx_importer_t;

/*  _____                 _
 * |   __|_ _ ___ ___ ___| |_
 * |   __|_'_| . | . |  _|  _|
 * |_____|_,_|  _|___|_| |_|
 *           |_|
 */

/* TODO(mtwilliams): Export functionality. */

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

/* We rely on setjmp and longjmp as it drastically reduces the complexity of
   error handling. This, unfortunately, ties us to the standard library.
   Theoretically you can provide your own implementation, but doing so will
   likely be hair pulling. This shouldn't affect you too much because
   interchange formats should be well away from your runtime. */
#include <setjmp.h>

/* Default to safer inflation and deflation that checks for errors. Error
   handling can affect performance but is better than crashing! */
#ifndef FBX__ZLIB_SAFE
  #define FBX__ZLIB_SAFE 1
#endif

/*  _____ _   _ _ _ _
 * |  |  | |_|_| |_| |_ _ _
 * |  |  |  _| | | |  _| | |
 * |_____|_| |_|_|_|_| |_  |
 *                     |___|
 */

#define FBX_MIN(a, b) \
  (((a) < (b)) ? (a) : (b))

#define FBX_MAX(a, b) \
  (((a) > (b)) ? (a) : (b))

#define fbx_copy(source, destination, size_in_bytes) \
  memcpy((void *)(destination), (void *)(source), size_in_bytes)

#define fbx_copy_s(source, destination, size_in_bytes) \
  memmove((void *)(destination), (void *)(source), size_in_bytes)

#define fbx_zero(ptr, size_in_bytes) \
  memset((void *)(ptr), 0, size_in_bytes)

FBX_BEGIN_EXTERN_C

/*  _____     ___     _
 * |   __|___|  _|___| |_ _ _
 * |__   | .'|  _| -_|  _| | |
 * |_____|__,|_| |___|_| |_  |
 *                       |___|
 */

static void fbx__default_panic_handler(const char *reason,
                                       const fbx_panic_info_t *info)
{
  fputs(reason, stderr);
  abort();
}

static fbx_panic_handler_fn *fbx__panic_handler = &fbx__default_panic_handler;

void fbx_get_panic_handler(fbx_panic_handler_fn **handler)
{
  *handler = fbx__panic_handler;
}

void fbx_set_panic_handler(fbx_panic_handler_fn *handler)
{
  fbx__panic_handler = handler;
}

#define FBX_PANIC(Format, ...) \
  fbx_panic({ __FILE__, __LINE__ }, Format, ##__VA_ARGS__)

void fbx_panic(const fbx_panic_info_t info, const char *format, ...)
{
  char reason[4096 + 1];

  va_list ap;

  va_start(ap, format);
  vsprintf(reason, format, ap);
  va_end(ap);

  fbx__panic_handler(reason, &info);
}

/*  _____
 * |     |___ _____ ___ ___ _ _
 * | | | | -_|     | . |  _| | |
 * |_|_|_|___|_|_|_|___|_| |_  |
 *                         |___|
 */

static void *fbx__default_allocate_callback(fbx_size_t size,
                                            fbx_size_t alignment,
                                            const fbx_allocation_info_t *info)
{
  (void)info;

#if FBX_ON_MSVC
  return _aligned_malloc(size, alignment);
#else
  #if FBX_ON_POSIX
  #endif
#endif
}

static void fbx__default_deallocate_callback(void *ptr)
{
#if FBX_ON_MSVC
  _aligned_free(ptr);
#else
  #if FBX_ON_POSIX
  #endif
#endif
}

static fbx_allocate_callback_fn *fbx__allocate_callback = &fbx__default_allocate_callback;
static fbx_deallocate_callback_fn *fbx__deallocate_callback = &fbx__default_deallocate_callback;

void fbx_get_allocator(fbx_allocate_callback_fn **allocate,
                       fbx_deallocate_callback_fn **deallocate)
{
  *allocate = fbx__allocate_callback;
  *deallocate = fbx__deallocate_callback;
}

void fbx_set_allocator(fbx_allocate_callback_fn *allocate,
                       fbx_deallocate_callback_fn *deallocate)
{
  fbx__allocate_callback = allocate;
  fbx__deallocate_callback = deallocate;
}

#define FBX_ALLOCATE(Size, Alignment) \
  fbx_allocate(Size, Alignment, { __FILE__, __LINE__, NULL })

#define FBX_ALLOCATE_S(Size, Alignment) \
  fbx_allocate_s(Size, Alignment, { __FILE__, __LINE__, NULL })

#define FBX_ALLOCATE_TAGGED(Tag, Size, Alignment) \
  fbx_allocate(Size, Alignment, { __FILE__, __LINE__, Tag })

#define FBX_ALLOCATE_TAGGED_S(Tag, Size, Alignment) \
  fbx_allocate_s(Size, Alignment, { __FILE__, __LINE__, Tag })

void *fbx_allocate(fbx_size_t size,
                   fbx_size_t alignment,
                   const fbx_allocation_info_t info)
{
  void *ptr;

  if (alignment <= 16)
    alignment = 16;

  ptr = fbx__allocate_callback(size, alignment, &info);

  if (ptr)
    /* We always zero memory, as it prevents an entire class of errors. */
    fbx_zero(ptr, size);

  return ptr;
}

void *fbx_allocate_s(fbx_size_t size,
                     fbx_size_t alignment,
                     const fbx_allocation_info_t info)
{
  void *ptr;

  ptr = fbx_allocate(size, alignment, info);

  if (ptr == NULL)
    /* We can get `NULL` if out of memory. */
    FBX_PANIC("Out of memory!\n");

  return ptr;
}

void fbx_free(void *ptr)
{
  fbx__deallocate_callback(ptr);
}

/* TODO(mtwilliams): Bookmarking. Maintain a linked list of positions that can
   be rewound to. If rewinding to bookmark prior to the last bookmark, remove
   it from the list to let rewinds cascade. */

/* A simple bump allocator. */
typedef struct fbx_block {
  fbx_uintptr_t base;
  fbx_size_t offset;
  fbx_size_t size;
} fbx_block_t;

void *fbx_block_allocate(fbx_block_t *block,
                         fbx_size_t size)
{
  void *ptr;

  /* Allocations are rounded up to the closest multiple of 16, thereby ensuring
     16-byte alignment of every allocation (assuming base is aligned.) */
  size = (((size + 15) / 16) * 16);

  if ((block->offset + size) > block->size)
    /* Not enough space. */
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
    /* We can get `NULL` if out of memory. */
    FBX_PANIC("Out of memory!\n");

  return ptr;
}

void fbx_block_reset(fbx_block_t *block)
{
  block->offset = 0;

  /* Rezero. Again, to prevent an entire class of errors. */
  fbx_zero(block->base, block->size);
}

/*  _____   _ _____
 * |     | / |     |
 * |-   -|/ /|  |  |
 * |_____|_/ |_____|
 */

/* TODO(mtwilliams): Support large files. */

typedef fbx_size_t (fbx_stream_read_callback_fn)(fbx_stream_t *stream,
                                                 void *buffer,
                                                 fbx_size_t count);

typedef fbx_size_t (fbx_stream_write_callback_fn)(fbx_stream_t *stream,
                                                  const void *buffer,
                                                  fbx_size_t count);

typedef fbx_size_t (fbx_stream_tell_callback_fn)(fbx_stream_t *stream);

typedef fbx_bool_t (fbx_stream_seek_callback_fn)(fbx_stream_t *stream,
                                                 fbx_size_t offset);

typedef fbx_bool_t (fbx_stream_skip_callback_fn)(fbx_stream_t *stream,
                                                 fbx_size_t amount);

typedef fbx_bool_t (fbx_stream_exhausted_callback_fn)(fbx_stream_t *stream);

typedef void (fbx_stream_close_callback_fn)(fbx_stream_t *stream);

typedef struct fbx_stream_dispatch {
  fbx_stream_read_callback_fn *read;
  fbx_stream_write_callback_fn *write;
  fbx_stream_tell_callback_fn *tell;
  fbx_stream_seek_callback_fn *seek;
  fbx_stream_skip_callback_fn *skip;
  fbx_stream_exhausted_callback_fn *exhausted;
  fbx_stream_close_callback_fn *close;
} fbx_stream_dispatch_t;

struct fbx_stream {
  const fbx_stream_dispatch_t *dispatch;
};

typedef struct fbx_memory_stream {
  fbx_stream_t container;
  fbx_uint8_t *buffer;
  fbx_size_t size;
  fbx_size_t cursor;
  fbx_bool_t readable;
  fbx_bool_t writeable;
} fbx_memory_stream_t;

extern const fbx_stream_dispatch_t fbx_memory_stream__dispatch;

static fbx_stream_t *fbx_memory_stream_open(void *buffer,
                                            fbx_size_t size,
                                            const char *flags)
{
  fbx_memory_stream_t *stream =
    (fbx_memory_stream_t *)FBX_ALLOCATE_TAGGED_S("stream", sizeof(fbx_memory_stream_t), 16);

  stream->container.dispatch = &fbx_memory_stream__dispatch;

  stream->buffer = (fbx_uint8_t *)buffer;
  stream->size = size;
  stream->cursor = 0;

  while (const char flag = *flags++) {
    switch (flag) {
      case 'r': case 'R': stream->readable = FBX_TRUE; break;
      case 'w': case 'W': stream->writeable = FBX_TRUE; break;
    }
  }

  return &stream->container;
}

static void fbx_memory_stream_close(fbx_memory_stream_t *stream)
{
  fbx_free((void *)stream);
}

static fbx_size_t fbx_memory_stream_read(fbx_memory_stream_t *stream,
                                         void *buffer,
                                         fbx_size_t count)
{
  if (stream->readable) {
    const fbx_size_t read = FBX_MIN(stream->cursor + count, stream->size);
    fbx_copy(&stream->buffer[stream->cursor], buffer, count);
    stream->cursor += read;
    return read;
  }

  return 0;
}

static fbx_size_t fbx_memory_stream_write(fbx_memory_stream_t *stream,
                                          const void *buffer,
                                          fbx_size_t count)
{
  if (stream->writeable) {
    const fbx_size_t write = FBX_MIN(stream->cursor + count, stream->size);
    fbx_copy(buffer, &stream->buffer[stream->cursor], count);
    stream->cursor += write;
    return write;
  }

  return 0;
}

static fbx_size_t fbx_memory_stream_tell(fbx_memory_stream_t *stream)
{
  return stream->cursor;
}

static fbx_bool_t fbx_memory_stream_seek(fbx_memory_stream_t *stream,
                                         fbx_size_t offset)
{
  if (offset > stream->size)
    return FBX_FALSE;

  stream->cursor = offset;

  return FBX_TRUE;
}

static fbx_bool_t fbx_memory_stream_skip(fbx_memory_stream_t *stream,
                                         fbx_size_t amount)
{
  if (stream->size - stream->cursor < amount)
    return FBX_FALSE;

  stream->cursor += amount;

  return FBX_TRUE;
}

static fbx_bool_t fbx_memory_stream_exhausted(fbx_memory_stream_t *stream)
{
  return (stream->cursor == stream->size);
}

static const fbx_stream_dispatch_t fbx_memory_stream__dispatch = {
  (fbx_stream_read_callback_fn *)&fbx_memory_stream_read,
  (fbx_stream_write_callback_fn *)&fbx_memory_stream_write,
  (fbx_stream_tell_callback_fn *)&fbx_memory_stream_tell,
  (fbx_stream_seek_callback_fn *)&fbx_memory_stream_seek,
  (fbx_stream_skip_callback_fn *)&fbx_memory_stream_skip,
  (fbx_stream_exhausted_callback_fn *)&fbx_memory_stream_exhausted,
  (fbx_stream_close_callback_fn *)&fbx_memory_stream_close
};

#ifndef FBX_NO_STANDARD_LIBRARY

typedef struct fbx_file_stream {
  fbx_stream_t container;
  FILE *handle;
  fbx_bool_t owner;
} fbx_file_stream_t;

extern const fbx_stream_dispatch_t fbx_file_stream__dispatch;

fbx_stream_t *fbx_file_stream_open(FILE *handle,
                                   fbx_bool_t owner)
{
  fbx_file_stream_t *stream =
    (fbx_file_stream_t *)FBX_ALLOCATE_TAGGED_S("stream", sizeof(fbx_file_stream_t), 1);

  stream->container.dispatch = &fbx_file_stream__dispatch;

  stream->handle = handle;
  stream->owner = owner;

  return &stream->container;
}

void fbx_file_stream_close(fbx_file_stream *stream)
{
  if (stream->owner)
    fclose(stream->handle);

  fbx_free((void *)stream);
}

static fbx_size_t fbx_file_stream_read(fbx_file_stream *stream,
                                       void *buffer,
                                       fbx_size_t count)
{
  return fread(buffer, 1, count, stream->handle);
}

static fbx_size_t fbx_file_stream_write(fbx_file_stream *stream,
                                        const void *buffer,
                                        fbx_size_t count)
{
  return fwrite(buffer, 1, count, stream->handle);
}

static fbx_size_t fbx_file_stream_tell(fbx_file_stream_t *stream)
{
  return ftell(stream->handle);
}

static fbx_bool_t fbx_file_stream_seek(fbx_file_stream_t *stream,
                                       fbx_size_t offset)
{
  return !!!fseek(stream->handle, offset, SEEK_SET);
}

static fbx_bool_t fbx_file_stream_skip(fbx_file_stream_t *stream,
                                       fbx_size_t amount)
{
  return !!!fseek(stream->handle, amount, SEEK_CUR);
}

static fbx_bool_t fbx_file_stream_exhausted(fbx_file_stream_t *stream)
{
  return !!feof(stream->handle);
}

static const fbx_stream_dispatch_t fbx_file_stream__dispatch = {
  (fbx_stream_read_callback_fn *)&fbx_file_stream_read,
  (fbx_stream_write_callback_fn *)&fbx_file_stream_write,
  (fbx_stream_tell_callback_fn *)&fbx_file_stream_tell,
  (fbx_stream_seek_callback_fn *)&fbx_file_stream_seek,
  (fbx_stream_skip_callback_fn *)&fbx_file_stream_skip,
  (fbx_stream_exhausted_callback_fn *)&fbx_file_stream_exhausted,
  (fbx_stream_close_callback_fn *)&fbx_file_stream_close
};

#endif

fbx_stream_t *fbx_stream_open_from_memory(void *buffer,
                                          fbx_size_t size,
                                          const char *flags)
{
  return fbx_memory_stream_open(buffer, size, flags);
}

#ifndef FBX_NO_STANDARD_LIBRARY

fbx_stream_t *fbx_stream_open_from_file(FILE *file)
{
  return fbx_file_stream_open(file, FBX_FALSE);
}

static void mode_from_flags(const char *flags,
                            char *mode)
{
  while (const char flag = *flags++) {
    switch (flag) {
      case 'r': case 'R': *mode++ = 'r'; break;
      case 'w': case 'W': *mode++ = 'w'; break;
    }
  }

  *mode++ = 'b';
}

fbx_stream_t *fbx_stream_open_from_path(const char *path,
                                        const char *flags)
{
  char mode[8+1] = { 0, };

  mode_from_flags(flags, mode);

  if (FILE *file = fopen(path, mode))
    return fbx_file_stream_open(file, FBX_TRUE);

  /* Couldn't open file. */
  return NULL;
}

#endif

void fbx_stream_close(fbx_stream_t *stream)
{
  stream->dispatch->close(stream);
}

fbx_size_t fbx_stream_tell(fbx_stream_t *stream)
{
  return stream->dispatch->tell(stream);
}

fbx_bool_t fbx_stream_seek(fbx_stream_t *stream,
                           fbx_size_t offset)
{
  return stream->dispatch->seek(stream, offset);
}

fbx_bool_t fbx_stream_skip(fbx_stream_t *stream,
                           fbx_size_t amount)
{
  return stream->dispatch->skip(stream, amount);
}

#ifndef FBX_NO_IMPORT

fbx_size_t fbx_stream_read(fbx_stream_t *stream,
                           void *buffer,
                           fbx_size_t count)
{
  return stream->dispatch->read(stream, buffer, count);
}

#endif

#ifndef FBX_NO_EXPORT

fbx_size_t fbx_stream_write(fbx_stream_t *stream,
                            const void *buffer,
                            fbx_size_t count)
{
  return stream->dispatch->write(stream, buffer, count);
}

#endif

fbx_bool_t fbx_stream_exhausted(fbx_stream_t *stream)
{
  return stream->dispatch->exhausted(stream);
}

#ifndef FBX_NO_IMPORT

/* Boolean values are encoded as 1-bit values in the least significant bit of
   of a single byte. */

static fbx_bool_t fbx_stream_read_bool(fbx_stream_t *stream, fbx_bool_t *value) {
  return (fbx_stream_read(stream, (void *)value, 1) == 1);
}

/* BUG(mtwilliams): These don't work on big endian machines. */

/* Integer values are little endian. */

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

/* Single-precision and double-precision IEEE 754 numbers. */

static fbx_bool_t fbx_stream_read_real32(fbx_stream_t *stream, fbx_real32_t *value) {
  return (fbx_stream_read(stream, (void *)value, 4) == 4);
}

static fbx_bool_t fbx_stream_read_real64(fbx_stream_t *stream, fbx_real64_t *value) {
  return (fbx_stream_read(stream, (void *)value, 8) == 8);
}

/* HACK(mtwilliams): We're down casting to 32-bit width types when we encounter
   files using the newer format, rather than up casting from 32-bit width types
   when we encounter files using the original format. Th breaks for files
   larger than 4GiB. In practice this should not be an issue, but may bite us
   or someone else the future. */

fbx_bool_t fbx_stream_read_uint64_as_uint32(fbx_stream_t *stream, fbx_uint32_t *value) {
  fbx_uint64_t wide;

  /* Read into wide type. */
  if (!fbx_stream_read_uint64(stream, &wide))
    return FBX_FALSE;

  /* Downcast willy nilly. */
  *value = wide;

  return FBX_TRUE;
}

#endif

#ifndef FBX_NO_EXPORT

/* TODO(mtwilliams): Writing variants. */

#endif

/* "Safe" variants of reading operations. If reading fails, these will early
   out and cause and unwinding. */

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

/*                                              
 *  _____                               _         
 * |     |___ _____ ___ ___ ___ ___ ___|_|___ ___ 
 * |   --| . |     | . |  _| -_|_ -|_ -| | . |   |
 * |_____|___|_|_|_|  _|_| |___|___|___|_|___|_|_|
 *                 |_|                            
 */

#ifdef FBX_USE_EXTERNAL_COMPRESSION
  
#ifndef fbx__zlib_inflate
  #error ("You must define `fbx__zlib_inflate` if using an external compression library!")
#endif

#ifndef fbx__zlib_deflate
  #error ("You must define `fbx__zlib_deflate` if using an external compression library!")
#endif

#else

/* https://tools.ietf.org/html/rfc1950 */
/* https://tools.ietf.org/html/rfc1951 */

fbx_uint32_t fbx__zlib_adler32(const void *data, fbx_size_t length)
{
  const fbx_uint8_t *D = (const fbx_uint8_t *)data;

  fbx_uint32_t s1 = 1, s2 = 0;

  while (length > 0) {
    /* We can defer modulo for quite a while. See Wikipedia. */
    fbx_uint32_t k = (length < 5552) ? length : 5552;
    
    /* Unrolled summation. */
    for (fbx_size_t i = k / 16; i; --i, D += 16) {
      s1 += D[0];  s2 += s1; s1 += D[1];  s2 += s1;
      s1 += D[2];  s2 += s1; s1 += D[3];  s2 += s1;
      s1 += D[4];  s2 += s1; s1 += D[5];  s2 += s1;
      s1 += D[6];  s2 += s1; s1 += D[7];  s2 += s1;
      s1 += D[8];  s2 += s1; s1 += D[9];  s2 += s1;
      s1 += D[10]; s2 += s1; s1 += D[11]; s2 += s1;
      s1 += D[12]; s2 += s1; s1 += D[13]; s2 += s1;
      s1 += D[14]; s2 += s1; s1 += D[15]; s2 += s1;
    }

    /* Handle leftovers. */
    for (fbx_size_t i = k % 16; i; --i) {
      s1 += *D++; s2 += s1;
    }

    s1 %= 65521;
    s2 %= 65521;

    length -= k;
  }

  return (s2 << 16) | s1;
}

typedef struct fbx__zlib_huffman {
  fbx_uint16_t counts[16];
  fbx_uint16_t code_to_symbol[288];
} fbx__zlib_huffman_t;

static void fbx__zlib_build_fixed_huffman_trees(fbx__zlib_huffman_t *l_tree,
                                                fbx__zlib_huffman_t *d_tree)
{
  /* Build length and literal tree. */

  for (unsigned i = 0; i < 7; ++i) l_tree->counts[i] = 0;

  l_tree->counts[7] = 24;
  l_tree->counts[8] = 152;
  l_tree->counts[9] = 112;

  for (unsigned i = 0; i < 24; ++i) l_tree->code_to_symbol[i] = 256 + i;
  for (unsigned i = 0; i < 144; ++i) l_tree->code_to_symbol[24 + i] = i;
  for (unsigned i = 0; i < 8; ++i) l_tree->code_to_symbol[24 + 144 + i] = 280 + i;
  for (unsigned i = 0; i < 112; ++i) l_tree->code_to_symbol[24 + 144 + 8 + i] = 144 + i;

  /* Build distance tree. */

  for (unsigned i = 0; i < 5; ++i) d_tree->counts[i] = 0;
  
  d_tree->counts[5] = 32;
  
  for (unsigned i = 0; i < 32; ++i) d_tree->code_to_symbol[i] = i;
}

static void fbx__zlib_build_huffman_tree(fbx__zlib_huffman_t *t,
                                         const fbx_uint8_t *lengths,
                                         unsigned n)
{
  /* Count number of codes bucketed by length. */
  for (unsigned i = 0; i < 16; ++i) t->counts[i] = 0;
  for (unsigned i = 0; i < n; ++i) t->counts[lengths[i]]++;

  /* Zero length codes can't be encountered. */
  t->counts[0] = 0;

  unsigned offsets[16];

  /* Compute offset table for distribution sort. */
  for (unsigned sum = 0, i = 0; i < 16; ++i) {
    offsets[i] = sum;
    sum += t->counts[i];
  }

  /* Create code to symbol translation table. */
  for (unsigned i = 0; i < n; ++i)
    if (lengths[i])
      t->code_to_symbol[offsets[lengths[i]]++] = i;
}

typedef struct fbx__zlib_inflater {
  const fbx_uint8_t *in;
  fbx_size_t left;

  fbx_uint8_t *out;
  fbx_size_t room;

  fbx_uint8_t byte;
  fbx_size_t bits;

#ifdef FBX__ZLIB_SAFE
  jmp_buf error;
#endif
} fbx__zlib_inflater_t;

#define FBX__ZLIB_OK           (0)
#define FBX__ZLIB_EMETHOD     -(1)
#define FBX__ZLIB_EWINDOW     -(2)
#define FBX__ZLIB_ESPACE      -(3)
#define FBX__ZLIB_ESPACE      -(4)
#define FBX__ZLIB_EBLOCK      -(5)
#define FBX__ZLIB_EDICTIONARY -(6)
#define FBX__ZLIB_EDATA       -(7)
#define FBX__ZLIB_ECHECKSUM   -(8)

static fbx_uint8_t fbx__zlib_inflate_get_next_byte(fbx__zlib_inflater_t *I)
{
#ifdef FBX__ZLIB_SAFE
  if (!I->left)
    longjmp(I->error, FBX__ZLIB_EDATA);
#endif
  return I->left--, *I->in++;
}

static unsigned fbx__zlib_inflate_get_next_bit(fbx__zlib_inflater_t *I)
{
  unsigned bit;

  if (!I->bits--) {
    I->byte = fbx__zlib_inflate_get_next_byte(I);
    I->bits = 7;
  }

  bit = I->byte & 1;
  I->byte >>= 1;

  return bit;
}

static unsigned fbx__zlib_inflate_get_bits(fbx__zlib_inflater_t *I,
                                           fbx_size_t n)
{
  unsigned v = 0;
  
  for (unsigned mask = 1, limit = 1 << n; mask < limit; mask <<= 1)
    v += fbx__zlib_inflate_get_next_bit(I) ? mask : 0;
  
  return v;
}

static void fbx__zlib_inflate_put(fbx__zlib_inflater_t *I, fbx_uint8_t byte)
{
#ifdef FBX__ZLIB_SAFE
  if (!I->room)
    longjmp(I->error, FBX__ZLIB_ESPACE);
#endif
  *I->out++ = byte;
  I->room--;
}

static void fbx__zlib_inflate_repeat(fbx__zlib_inflater_t *I, unsigned length, unsigned distance)
{
#ifdef FBX__ZLIB_SAFE
  if (length > I->room)
    longjmp(I->error, FBX__ZLIB_ESPACE);
#endif

  const fbx_uint8_t *referenced = I->out - distance;

  while (length) {
    *I->out++ = *referenced++;
    I->room--;
    --length;
  }
}

static void fbx__zlib_inflate_uncompressed_block(fbx__zlib_inflater_t *I)
{
  fbx_uint16_t length, complement;
 
  /* Read length of block and its one's complement, making sure to skip any
     remaining bits to align to a byte boundary. */
  length = fbx__zlib_inflate_get_next_byte(I) | 256 * fbx__zlib_inflate_get_next_byte(I);
  complement = fbx__zlib_inflate_get_next_byte(I) | 256 * fbx__zlib_inflate_get_next_byte(I);

#if FBX__ZLIB_SAFE
  if (length != ~complement)
    longjmp(I->error, FBX__ZLIB_EBLOCK);
#endif

  while (length--) {
  #if FBX__ZLIB_SAFE
    if (!I->left)
      longjmp(I->error, FBX__ZLIB_EDATA);
    if (!I->room)
      longjmp(I->error, FBX__ZLIB_ESPACE);
  #endif

    I->left--;
    I->room--;

    *I->out++ = *I->in++;
  }

  /* Since we skipped any remaining bits to align to a byte boundary, we
     preemptively get the next eight bits to simplify `fbx__zlib_inflate_get_next_bit`. */
  I->byte = fbx__zlib_inflate_get_next_byte(I);
  I->bits = 8;
}

static unsigned fbx__zlib_inflate_decode_symbol(fbx__zlib_inflater_t *I,
                                                const fbx__zlib_huffman_t *t)
{
  int sum = 0, current = 0, length = 0;

  do {
    current = 2 * current + fbx__zlib_inflate_get_next_bit(I);

    length += 1;

    sum += t->counts[length];
    current -= t->counts[length];
  } while (current >= 0);

  return t->code_to_symbol[sum + current];
}

/* Special ordering of code length codes. */
static const fbx_uint8_t fbx__zlib_clc_order[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

static void fbx__zlib_decode_huffman_trees(fbx__zlib_inflater_t *I,
                                           fbx__zlib_huffman_t *l_tree,
                                           fbx__zlib_huffman_t *d_tree)
{
  unsigned n_l, n_d, n_c;

  n_l = fbx__zlib_inflate_get_bits(I, 5) + 257;
  n_d = fbx__zlib_inflate_get_bits(I, 5) + 1;
  n_c = fbx__zlib_inflate_get_bits(I, 4) + 4;

  fbx_uint8_t lengths[288 + 32];

  /* Read in code lengths for the Huffman tree used to encode the code lengths
    for the following trees. */
  for (unsigned i = 0; i < n_c; ++i)
    lengths[fbx__zlib_clc_order[i]] = fbx__zlib_inflate_get_bits(I, 3);
  for (unsigned i = n_c; i < 19; ++i)
    lengths[fbx__zlib_clc_order[i]] = 0;

  /* Build tree used to encode code lengths for following trees. */
  fbx__zlib_huffman_t cl_tree;
  fbx__zlib_build_huffman_tree(&cl_tree, lengths, 19);

  /* Decode code lengths for the dynamic trees. */
  for (unsigned n = 0; n < (n_l + n_d);) {
    unsigned symbol = fbx__zlib_inflate_decode_symbol(I, &cl_tree);
    switch (symbol) {
      case 16: { /* Copy the previous code length 3-6 times. */
        const fbx_uint8_t previous = lengths[n-1];
        for (unsigned i = fbx__zlib_inflate_get_bits(I, 2) + 3; i; --i)
          lengths[n++] = previous;
      } break;
      case 17: { /* Repeat a code length of 0 for 3-10 times. */
        for (unsigned i = fbx__zlib_inflate_get_bits(I, 3) + 3; i; --i)
          lengths[n++] = 0;
      } break;
      case 18: { /* Repeat a code length of 0 for 11-138 times. */
        for (unsigned i = fbx__zlib_inflate_get_bits(I, 7) + 11; i; --i)
          lengths[n++] = 0;
      } break;
      default: { /* Represent code lengths of 0-15. */
        lengths[n++] = symbol;
      }
    }
  }

  /* Build literal/length tree. */
  fbx__zlib_build_huffman_tree(l_tree, &lengths[0], n_l);

  /* Build distance tree. */
  fbx__zlib_build_huffman_tree(d_tree, &lengths[n_l], n_d);
}

/* Lookup tables for length and distance. */
static const fbx_uint8_t fbx__zlib_length_bits[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5 };
static const fbx_uint16_t fbx_zlib__length_base[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
static const fbx_uint8_t fbx__zlib_distance_bits[] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
static const fbx_uint16_t fbx__zlib_distance_base[] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };

static void fbx__zlib_inflate_huffman_block(fbx__zlib_inflater_t *I,
                                            fbx__zlib_huffman_t *l_tree,
                                            fbx__zlib_huffman_t *d_tree)
{
  unsigned symbol, length, distance;

decode:
  symbol = fbx__zlib_inflate_decode_symbol(I, l_tree);

  switch (symbol) {
    case 256: /* End of block. */
      return;

    case 257: case 258: case 259: case 260: case 261: case 262: case 263:
    case 264: case 265: case 266: case 267: case 268: case 269: case 270:
    case 271: case 272: case 273: case 274: case 275: case 276: case 277:
    case 278: case 279: case 280: case 281: case 282: case 283: case 284:
    case 285: /* Length-distance pair. */
      length = fbx__zlib_inflate_get_bits(I, fbx__zlib_length_bits[symbol-257]) + fbx_zlib__length_base[symbol-257];
      symbol = fbx__zlib_inflate_decode_symbol(I, d_tree);
      distance = fbx__zlib_inflate_get_bits(I, fbx__zlib_distance_bits[symbol]) + fbx__zlib_distance_base[symbol];
      fbx__zlib_inflate_repeat(I, length, distance);
      goto decode;

    default: /* Literal. */
      fbx__zlib_inflate_put(I, symbol);
      goto decode;
  }
}

static void fbx__zlib_inflate_fixed_huffman_block(fbx__zlib_inflater_t *I)
{
  /* Build length/literal and distance trees used by fixed blocks. */
  fbx__zlib_huffman_t l_tree, d_tree;
  fbx__zlib_build_fixed_huffman_trees(&l_tree, &d_tree);

  /* Inflate block using trees. */
  fbx__zlib_inflate_huffman_block(I, &l_tree, &d_tree);
}

static void fbx__zlib_inflate_dynamic_huffman_block(fbx__zlib_inflater_t *I)
{
  /* Decode length/literal and distances trees. */
  fbx__zlib_huffman_t l_tree, d_tree;
  fbx__zlib_decode_huffman_trees(I, &l_tree, &d_tree);

  /* Inflate block using trees. */
  fbx__zlib_inflate_huffman_block(I, &l_tree, &d_tree);
}

/* TODO(mtwilliams): Assert that `CMF * 256 + FLG` is a multiple of 31. */

static int fbx__zlib_inflate(const void *in, fbx_size_t in_sz,
                             void *out, fbx_size_t out_sz)
{
  fbx__zlib_inflater_t I;

  I.in = (const fbx_uint8_t *)in; I.left = in_sz;
  I.out = (fbx_uint8_t *)out; I.room = out_sz;
  I.byte = 0; I.bits = 0;

#if FBX__ZLIB_SAFE
  if (int error = setjmp(I.error))
    return error;
#endif

  unsigned c_method, c_info;

  c_method = fbx__zlib_inflate_get_bits(&I, 4);
  c_info = fbx__zlib_inflate_get_bits(&I, 4);

  if (c_method != 8)
    /* We only support DEFLATE. */
    return FBX__ZLIB_EMETHOD;

  if (c_info > 7)
    /* We only support windows up to 32KiB. */
    return FBX__ZLIB_EWINDOW;

  unsigned f_check, f_dictionary, f_level;

  f_check = fbx__zlib_inflate_get_bits(&I, 5);
  f_dictionary = fbx__zlib_inflate_get_bits(&I, 1);
  f_level = fbx__zlib_inflate_get_bits(&I, 2);

  if (f_dictionary != 0)
    /* We don't support preset dictionaries. */
    return FBX__ZLIB_EDICTIONARY;

  for (;;) {
    unsigned b_type, b_final;

    b_final = fbx__zlib_inflate_get_bits(&I, 1);
    b_type = fbx__zlib_inflate_get_bits(&I, 2);

    switch (b_type) {
      case 0: /* No compression. */
        fbx__zlib_inflate_uncompressed_block(&I);
        break;
      case 1: /* Compressed with fixed Huffman codes. */
        fbx__zlib_inflate_fixed_huffman_block(&I);
        break;
      case 2: /* Compressed with dynamic Huffman codes. */
        fbx__zlib_inflate_dynamic_huffman_block(&I);
        break;
    #ifdef FBX__ZLIB_SAFE
      case 3: /* Reserved; erroneous. */
        return FBX__ZLIB_EBLOCK;
    #endif
    }

    if (b_final)
      /* Final block, fall through to check checksum. */
      break;
    
  #if FBX__ZLIB_SAFE
    if (!I.left)
      /* Ran out of data before final block. */
      return FBX__ZLIB_EDATA;
  #endif
  }

#if FBX__ZLIB_SAFE
  if (I.left < 4)
    /* Not enough data left for checksum to be present. */
    return FBX__ZLIB_EDATA;

  fbx_uint32_t checksum, expected;

  checksum = fbx__zlib_adler32(out, out_sz - I.room);

  /* Checksum is in big endian. */
  expected = ((const fbx_uint8_t *)I.in)[0] << 24
           | ((const fbx_uint8_t *)I.in)[1] << 16
           | ((const fbx_uint8_t *)I.in)[2] <<  8
           | ((const fbx_uint8_t *)I.in)[3] <<  0;

  if (checksum != expected)
    return FBX__ZLIB_ECHECKSUM;
#endif

  return FBX__ZLIB_OK;
}

#endif

/*  ____      _
 * |    \ ___| |_ ___
 * |  |  | .'|  _| .'|
 * |____/|__,|_| |__,|
 */

typedef enum fbx_datum_type {
  FBX_UNKNOWN_DATUM = 0,

  FBX_BOOLEAN_DATUM = 'C',
  FBX_INT16_DATUM = 'Y',
  FBX_INT32_DATUM = 'I',
  FBX_INT64_DATUM = 'L',
  FBX_REAL32_DATUM = 'F',
  FBX_REAL64_DATUM = 'D',

  FBX_BOOLEAN_DATA = 'b',
  FBX_INT32_DATA = 'i',
  FBX_INT64_DATA = 'l',
  FBX_REAL32_DATA = 'f',
  FBX_REAL64_DATA = 'd',

  FBX_STRING_DATUM = 'S',
  FBX_BINARY_DATUM = 'R'
} fbx_datum_type_t;

static fbx_size_t fbx_size_of_datum(fbx_datum_type_t type)
{
  switch (type) {
    case FBX_BOOLEAN_DATUM: return 1;
    case FBX_INT32_DATUM: return 4;
    case FBX_INT64_DATUM: return 8;
    case FBX_REAL32_DATUM: return 4;
    case FBX_REAL64_DATUM: return 8;
  }

  return 0;
}

typedef struct fbx_ref_to_data {
  fbx_uint32_t length;

  union {
    const void *pointer;
    const void *binary;
    const char *string;
  };
} fbx_ref_to_data_t;

typedef enum fbx_data_encoding {
  FBX_RAW        = 0,
  FBX_COMPRESSED = 1
} fbx_data_encoding_t;

typedef struct fbx_data_t {
  /* Indicates array encoding. */
  fbx_data_encoding_t encoding;

  /* Type of elements in array. */
  fbx_datum_type_t type;

  /* Number of elements in array. */
  fbx_uint32_t count;
  
  union {
    /* Untyped pointer to elements. */
    const void *elements;

    /* Typed pointers. */
    const fbx_bool_t *ptr_to_booleans;
    const fbx_int32_t *ptr_to_int32;
    const fbx_int64_t *ptr_to_int64;
    const fbx_real32_t *ptr_to_real32;
    const fbx_real64_t *ptr_to_real64;
  };

  /* Pointer to raw data. */
  const void *raw;

  /* Size of raw data. */
  fbx_uint32_t size;
} fbx_data_t;

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
    fbx_data_t as_an_array;
  };
} fbx_datum_t;

#define BUMP(Pointer, Count) \
  (void *)(fbx_uintptr_t(Pointer) + (Count))

static fbx_bool_t fbx_extract_an_array(const void *cursor,
                                       fbx_datum_t *datum,
                                       const void **updated)
{
  /* Read in number of elements in array. */
  const fbx_uint32_t count = *(fbx_uint32_t *)cursor; 
  cursor = BUMP(cursor, 4);

  /* Identify encoding. */
  const fbx_uint32_t encoding = *(fbx_uint32_t *)cursor;
  cursor = BUMP(cursor, 4);

  /* Determine if array is encoded since they are handled differently. */
  const fbx_bool_t encoded = (encoding != FBX_RAW);

  fbx_uint32_t size_in_memory,
               size_in_file;

  /* Derive size in bytes from type and count. */
  switch (datum->type) {
    case FBX_BOOLEAN_DATA: size_in_memory = 1 * count; break;
    case FBX_INT32_DATA: size_in_memory = 4 * count; break;
    case FBX_INT64_DATA: size_in_memory = 8 * count; break;
    case FBX_REAL32_DATA: size_in_memory = 4 * count; break;
    case FBX_REAL64_DATA: size_in_memory = 8 * count; break;
  }

  /* Read in size in file if encoded, otherwise it's the same. */
  size_in_file = encoded ? *(fbx_uint32_t *)cursor : size_in_memory;
  cursor = BUMP(cursor, 4);

  datum->as_an_array.encoding = fbx_data_encoding_t(encoding);

  /* Derive type of elements. */
  switch (datum->type) {
    case FBX_BOOLEAN_DATA: datum->as_an_array.type = FBX_BOOLEAN_DATUM; break;
    case FBX_INT32_DATA: datum->as_an_array.type = FBX_INT32_DATUM; break;
    case FBX_INT64_DATA: datum->as_an_array.type = FBX_INT64_DATUM; break;
    case FBX_REAL32_DATA: datum->as_an_array.type = FBX_REAL32_DATUM; break;
    case FBX_REAL64_DATA: datum->as_an_array.type = FBX_REAL64_DATUM; break;
  }

  /* Copy count through. */
  datum->as_an_array.count = count;

  /* Elements can only be accessed after decoding. */
  datum->as_an_array.elements = NULL;

  /* Regardless of encoding, we keep a pointer to the raw data. */
  datum->as_an_array.raw = cursor;

  /* As well as it's size. */
  datum->as_an_array.size = size_in_file;

  return FBX_TRUE;
}

static fbx_bool_t fbx_inflate_an_array(fbx_data_t *data,
                                       fbx_block_t *block)
{
  const fbx_size_t size_afer_inflation =
    fbx_size_of_datum(data->type) * data->count;

  void *inflated =
    fbx_block_allocate_s(block, size_afer_inflation);

  data->elements = inflated;

  return !fbx__zlib_inflate(data->raw, data->size,
                            inflated, size_afer_inflation);
}

static fbx_bool_t fbx_decode_an_array(fbx_data_t *data,
                                      fbx_block_t *block)
{
  switch (data->encoding) {
    case FBX_RAW:
      /* No need to decode. */
      data->elements = data->raw;
      return FBX_TRUE;

    case FBX_COMPRESSED:
      return fbx_inflate_an_array(data, block);

    default:
      /* Unknown or unsupported encoding. */
      return FBX_FALSE;
  }
}

#define fbx_decode_an_array_s(Data, Block) \
  if (!fbx_decode_an_array(Data, Block)) { \
    return FBX_FALSE;                      \
  }

static fbx_bool_t fbx_extract_a_datum(const void *cursor,
                                      fbx_datum_t *datum,
                                      const void **updated)
{
  datum->type = FBX_UNKNOWN_DATUM;

  /* Identify type from prefixed byte. */
  datum->type_as_byte = *((fbx_uint8_t *)cursor);

  /* Bump cursor to data. */
  cursor = BUMP(cursor, 1);

  switch (datum->type) {
    /* Primitives. */
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

    /* Arrays of primitives. */
    case FBX_BOOLEAN_DATA:
    case FBX_INT32_DATA:
    case FBX_INT64_DATA:
    case FBX_REAL32_DATA:
    case FBX_REAL64_DATA:
      return fbx_extract_an_array(cursor, datum, updated);

    /* Strings and binaries. */
    case FBX_STRING_DATUM:
    case FBX_BINARY_DATUM:
      datum->as_a_ref.length = *(fbx_uint32_t *)cursor;
      datum->as_a_ref.pointer = BUMP(cursor, 4);
      *updated = BUMP(cursor, 4 + datum->as_a_ref.length);
      return FBX_TRUE;
  }

  /* Unknown type. */
  return FBX_FALSE;
}

/* Don't export our internal helper macro. */
#undef BUMP

static fbx_bool_t fbx_extract_a_datum_typed(fbx_datum_type_t type,
                                            const void *cursor,
                                            const void **updated,
                                            void *extracted)
{
  fbx_datum_t datum;

  if (!fbx_extract_a_datum(cursor, &datum, updated))
    /* Failed to extract. */
    return FBX_FALSE;

  if (datum.type != type)
    /* Type mismatch. */
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

    case FBX_BOOLEAN_DATA:
    case FBX_INT32_DATA:
    case FBX_INT64_DATA:
    case FBX_REAL32_DATA:
    case FBX_REAL64_DATA:
      *(fbx_data_t *)extracted = datum.as_an_array;
      return FBX_TRUE;

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

/* Generated with the help of `scripts/fnv1a.c`. */

typedef enum fbx_property_type {
  FBX_COMPOUND_PROPERTY = 0xe79b2efc,
  FBX_OBJECT_PROPERTY   = 0xb8c60cba,
  FBX_ENUM_PROPERTY     = 0x816cb000,
  FBX_BOOLEAN_PROPERTY  = 0xc894953d,
  FBX_INTEGER_PROPERTY  = 0x95e97e5e,
  FBX_FLOAT_PROPERTY    = 0xa6c45d85,
  FBX_DOUBLE_PROPERTY   = 0xa0eb0f08,
  FBX_NUMBER_PROPERTY   = 0x61e9eac0,
  FBX_STRING_PROPERTY   = 0x84006183,
  FBX_TIME_PROPERTY     = 0xc1d8710b,
  FBX_DATETIME_PROPERTY = 0x9bec7490,
  FBX_VECTOR_PROPERTY   = 0x53b2b785,
  FBX_COLOR_PROPERTY    = 0xe5b43cf8
} fbx_property_type_t;

static fbx_property_type_t fbx_property_type_by_name(const char *name,
                                                     size_t length)
{
  const fbx_uint8_t *start = (const fbx_uint8_t *)name;
  const fbx_uint8_t *end = start + length;

  /* Hash with Fowler–Noll–Vo's Hash (FNV-1a). */
  fbx_uint32_t hash = 2166136261ul;
  for (const fbx_uint8_t *I = start; I != end; ++I)
    hash = (hash ^ *I) * 16777619ul;

  /* Remap certain hashes to types. */
  switch (hash) {
    /* Translation, rotation, and scaling are remapped to the three-dimensional
       vector type instead of being special cased. */
    case 0x519a291f: case 0xffcc7052: case 0x679a6399:
      return FBX_VECTOR_PROPERTY;
    /* There appear to be two distinct color types without difference. */
    case 0xe5b43cf8: case 0xff873b9b:
      return FBX_COLOR_PROPERTY;
  }

  return (fbx_property_type_t)hash;
}

typedef enum fbx_property_flag {
  /* Signifies the property is user-defined. An "extension" in some sense. */
  FBX_PROPERTY_IS_USER_DEFINED = (1 << 31)
} fbx_property_flag_t;

/* Internal representation of properties attached to nodes. */
typedef struct fbx_property {
  fbx_property_type_t type;

  const char *name;

#if 0
  /* Sub-type? */
  const char *hint;
#endif

  fbx_uint32_t flags;

  union {
    fbx_ref_to_data_t as_a_ref;
    fbx_bool_t as_a_boolean;
    fbx_int32_t as_an_integer;
    fbx_real64_t as_a_number;
    fbx_vec3_t as_a_vector;
    fbx_color_t as_a_color;
  } value;
} fbx_property_t;

/* Internal representation of nodes encountered in `.fbx` files. */
typedef struct fbx_node {
  /* Unique identifier assigned during import to aid housekeeping. */
  fbx_uint32_t id;

  /* Name given to this node. May, on the rare occasion, be `NULL`. */
  const char *name;

  /* Data associated with this node. */
  const void *data;
  /* Number of elements comprising `data`. */
  fbx_uint32_t num_of_datums;
  /* Size of `data` in bytes. */
  fbx_uint32_t size_of_data;

  /* Properties associated with this node. */
  const fbx_property_t *properties;
  /* Number of properties associated with this node. */
  fbx_uint32_t num_of_properties;

  /* Parent node. */
  struct fbx_node *parent;
  /* First child. */
  struct fbx_node *children;
  /* Next sibling, i.e. next child of parent. */
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

typedef struct fbx_template {
  const char *name;

  const fbx_property_t *properties;
  fbx_uint32_t num_of_properties;

  /* Node that defined this type. */
  const fbx_node_t *definition;
} fbx_template_t;

static const fbx_property_t *fbx_template_property_by_name(const fbx_template_t *tmpl,
                                                           const char *name)
{
  for (fbx_uint32_t property = 0; property < tmpl->num_of_properties; ++property)
    if (strcmp(tmpl->properties[property].name, name) == 0)
      return &tmpl->properties[property];
  return NULL;
}

/*  _____                   _
 * |     |_____ ___ ___ ___| |_
 * |-   -|     | . | . |  _|  _|
 * |_____|_|_|_|  _|___|_| |_|
 *             |_|
 */

/* LEAK(mtwilliams): We're never freeing the permanent memory pool! */

struct fbx_importer {
  const fbx_import_options_t *options;

  fbx_stream_t *stream;

  struct {
    fbx_block_t permanent;
    fbx_block_t transient;
    fbx_block_t strings;
  } memory;

  /* Internal representation of hierarchy encountered in `.fbx`. */
  fbx_node *nodes;
  fbx_uint32_t num_of_nodes;

  /* Internal representation of types of object as defined in `.fbx`. */
  const fbx_template_t *templates;
  fbx_uint32_t num_of_templates;

  /* We preemptively cache pointers to commonly used templates to amortize
     cost of lookups. */
  const fbx_template_t *model_template;
  const fbx_template_t *geometry_template;

  /* Legally, a `.fbx` can lack any definitions, objects, and connections,
     making it empty, at least for our uses. */
  fbx_bool_t is_basically_empty;

  /* As of FBX 7.5 (also known as FBX 2016) nodes are described with 64-bit
     width types (rather than the original 32-bit types) and empty nodes are
     25 bytes long (rather than the original 13 bytes). This tracks if the
     newer format is used to adjust extraction logic accordingly. */
  fbx_bool_t using_newer_format;

  /* We reuse the same empty string since we encounter them so often. */
  const char *an_empty_string;

  /* Imported document. */
  fbx_t *fbx;

  /* Various things can go wrong or be wrong during import. If we can't
     recover then we jump to rapidly unwind and early out. */
  jmp_buf error;

  /* We reserve some space to store the reason for the error since an error can
     be raised due to memory exhaustion. */
  char reason[256];
};

static void fbx_importer_error(fbx_importer_t *importer,
                               fbx_status_t error,
                               const char *format,
                               ...)
{
  va_list ap;

  /* Format reason into space we reserved upfront. */
  va_start(ap, format);
  vsprintf(importer->reason, format, ap);
  va_end(ap);

  /* Unwind! See `fbx_importer_run`. */
  longjmp(importer->error, (int)error);
}

static void *fbx_importer_transient_alloc(fbx_importer_t *importer,
                                          fbx_size_t size)
{
  if (void *ptr = fbx_block_allocate(&importer->memory.transient, size))
    return ptr;

  fbx_importer_error(importer, FBX_EMEMORY, "Exhausted transient memory pool!");
}

static void *fbx_importer_permanent_alloc(fbx_importer_t *importer,
                                          fbx_size_t size)
{
  if (void *ptr = fbx_block_allocate(&importer->memory.permanent, size))
    return ptr;

  fbx_importer_error(importer, FBX_EMEMORY, "Exhausted permanent memory pool!");
}

static const char *fbx_importer_intern_a_string(fbx_importer_t *importer,
                                                const char *string,
                                                fbx_size_t length)
{
  if (length == 0)
    /* Empty string optimization. */
    return importer->an_empty_string;    
    
  char *storage = (char *)fbx_block_allocate(&importer->memory.strings, length + 1);

  if (!storage)
    fbx_importer_error(importer, FBX_EMEMORY, "Exhausted string pool!");

  fbx_copy(string, storage, length);
  
  /* Null-terminate because it's never done for us. Autodesk man... */
  storage[length] = '\0';
  
  return storage;
}

static const char *fbx_importer_intern_a_string_from_ref(fbx_importer_t *importer,
                                                         const fbx_ref_to_data_t ref)
{
  return fbx_importer_intern_a_string(importer, ref.string, ref.length);
}

static fbx_node_t *fbx_importer_alloc_a_node(fbx_importer_t *importer) {
  fbx_node_t *node =
    (fbx_node_t *)fbx_importer_transient_alloc(importer,
                                               sizeof(fbx_node_t));

  /* Zero everything out. */
  fbx_zero(node, sizeof(fbx_node_t));

  /* We assign a unique identifier to each node. This is used for internal
     house keeping and such. Do not rely on it! */
  node->id = ++importer->num_of_nodes;

  return node;
}

static fbx_object_t *fbx_importer_alloc_an_object(fbx_importer_t *importer)
{
  fbx_object_t *object =
    (fbx_object_t *)fbx_importer_permanent_alloc(importer,
                                                 sizeof(fbx_object_t));

  /* Zero everything out. */
  fbx_zero(object, sizeof(fbx_object_t));

  return object;
}

#define FBX_IMPORTER_DEF_ALLOC(Type, Name)                                     \
  static Type *fbx_importer_alloc_a_##Name(fbx_importer_t *importer) {         \
    Type *Name = (Type *)fbx_importer_permanent_alloc(importer, sizeof(Type)); \
    fbx_zero(Name, sizeof(Type));                                              \
    return Name;                                                               \
  }

FBX_IMPORTER_DEF_ALLOC(fbx_model_t, model);
FBX_IMPORTER_DEF_ALLOC(fbx_mesh_t, mesh);

#undef FBX_IMPORTER_DEF_ALLOC

static fbx_importer_t *fbx_importer_setup(const fbx_import_options_t *options)
{
  fbx_importer_t *importer =
    (fbx_importer_t *)FBX_ALLOCATE_TAGGED_S("importer", sizeof(fbx_importer_t), 16);

  importer->options = options;

  /* Stream is bound by caller. */
  importer->stream = NULL;

  /* Allocate memory pools. */
  importer->memory.permanent.base = (fbx_uintptr_t)FBX_ALLOCATE_TAGGED_S("permanent_memory_pool", options->permanent_memory_pool, 16);
  importer->memory.permanent.offset = 0;
  importer->memory.permanent.size = options->permanent_memory_pool;
  importer->memory.transient.base = (fbx_uintptr_t)FBX_ALLOCATE_TAGGED_S("transient_memory_pool", options->transient_memory_pool, 16);
  importer->memory.transient.offset = 0;
  importer->memory.transient.size = options->transient_memory_pool;
  importer->memory.strings.base = (fbx_uintptr_t)FBX_ALLOCATE_TAGGED_S("strings_memory_pool", options->strings_memory_pool, 16);
  importer->memory.strings.offset = 0;
  importer->memory.strings.size = options->transient_memory_pool;

  /* Set reasonable defaults. */
  importer->nodes = NULL;
  importer->num_of_nodes = 0;
  importer->templates = NULL;
  importer->num_of_templates = 0;

  /* Create a dummy root node as it makes various operations are lot easier. */
  fbx_node_t *root = fbx_importer_alloc_a_node(importer);
  importer->nodes = root;

  /* Pointers to templates are cached after definitions are processed. */
  importer->model_template = NULL;
  importer->geometry_template = NULL;

  /* Assume there's a scene for us. There may not be though! */
  importer->is_basically_empty = FBX_FALSE;

  /* Assume older format until otherwise specified. */
  importer->using_newer_format = FBX_FALSE;

  /* We can encounter empty strings quite often. This saves 15 bytes of
    overheard a few hundred times on average. */
  char *an_empty_string = (char *)fbx_block_allocate_s(&importer->memory.strings, 1);
  an_empty_string[0] = '\0';
  importer->an_empty_string = an_empty_string;

  /* Allocate space for imported document. */
  importer->fbx = (fbx_t *)FBX_ALLOCATE_TAGGED_S("fbx", sizeof(fbx_t), 16);

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
    /* File is too short (way too short) to be (binary) FBX. */
    return FBX_FALSE;

  /* If, and only if, the magic matches can we presume (binary) FBX. */
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
    /* No properties associated with the node. */
    return FBX_TRUE;

  fbx_property_t *properties = NULL;
  fbx_uint32_t num_of_properties = 0;

  /* PERF(mtwilliams): Track number of children. */
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

    properties[property].name = fbx_importer_intern_a_string_from_ref(importer,
                                                                      ref_to_name);

    /* Appears to hint the sub-type? */
    fbx_ref_to_data_t ref_to_hint;
    fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &ref_to_hint);

  #if 0
    if (ref_to_hint.length > 0) {
      properties[property].hint = fbx_importer_intern_a_string_from_ref(importer,
                                                                        ref_to_hint);
    } else {
      properties[property].hint = NULL;
    }
  #endif

    fbx_ref_to_data_t ref_to_flags;
    fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &ref_to_flags);

    properties[property].flags = 0x00000000;

    for (fbx_uint32_t flag = 0; flag < ref_to_flags.length; ++flag) {
      switch (ref_to_flags.string[flag]) {
        /* TODO(mtwilliams): Locked & Muted. */
        case 'A': /* TODO(mtwilliams): Animatable flag. */ break;
        case '+': /* TODO(mtwilliams): Animated flag. May need to check for `A+`? */ break;
        case 'H': /* TODO(mtwilliams): Hidden flag. */ break;
        case 'U': properties[property].flags |= FBX_PROPERTY_IS_USER_DEFINED; break;
      }
    }

    switch (properties[property].type) {
      case FBX_COMPOUND_PROPERTY:
        break;

      case FBX_OBJECT_PROPERTY:
        break;

      case FBX_ENUM_PROPERTY:
        fbx_extract_a_datum_s(FBX_INT32_DATUM, cursor, &properties[property].value.as_an_integer);
        break;

      case FBX_BOOLEAN_PROPERTY:
        /* Booleans properties are stored using integer datums... yeah. */
        fbx_int32_t as_an_integer;
        fbx_extract_a_datum_s(FBX_INT32_DATUM, cursor, &as_an_integer);
        properties[property].value.as_a_boolean = !!as_an_integer;
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

      case FBX_STRING_PROPERTY:
        fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &properties[property].value.as_a_ref);
        break;

      case FBX_VECTOR_PROPERTY:
        fbx_real64_t x, y, z;
        fbx_extract_a_datum_s(FBX_REAL64_DATUM, cursor, &x);
        fbx_extract_a_datum_s(FBX_REAL64_DATUM, cursor, &y);
        fbx_extract_a_datum_s(FBX_REAL64_DATUM, cursor, &z);
        properties[property].value.as_a_vector.x = fbx_real32_t(x);
        properties[property].value.as_a_vector.y = fbx_real32_t(y);
        properties[property].value.as_a_vector.z = fbx_real32_t(z);
        break;

      case FBX_COLOR_PROPERTY:
        fbx_real64_t r, g, b;
        fbx_extract_a_datum_s(FBX_REAL64_DATUM, cursor, &r);
        fbx_extract_a_datum_s(FBX_REAL64_DATUM, cursor, &g);
        fbx_extract_a_datum_s(FBX_REAL64_DATUM, cursor, &b);
        properties[property].value.as_a_color.r = fbx_real32_t(r);
        properties[property].value.as_a_color.g = fbx_real32_t(g);
        properties[property].value.as_a_color.b = fbx_real32_t(b);
        properties[property].value.as_a_color.a = 1.f;
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

  /* Determine absolute offest to the start of this node. */
  fbx_uint32_t start = fbx_stream_tell(importer->stream);

  /* Then read the absolute offest to the end of this node. */
  fbx_uint32_t end;
  fbx_importer_read_uint32_or_uint64(importer, &end);

  if (end == 0) {
    /* Empty node. */
    const fbx_size_t width = (importer->using_newer_format ? 25 : 13);
    return fbx_stream_seek(importer->stream, start + width);
  }

#if 0
  /* We can now derive the total length of this node in bytes. */
  fbx_uint32_t length = end - start;
#endif

  /* Read the number of datums associated with this node. */
  fbx_uint32_t num_of_datums;
  fbx_importer_read_uint32_or_uint64(importer, &num_of_datums);

  /* Read the size, in bytes, used to store all data associated this node. */
  fbx_uint32_t size_of_data;
  fbx_importer_read_uint32_or_uint64(importer, &size_of_data);

  /* Read the length, in characters, of the name of this node. Names are not
     null terminated, so we need to be careful. */
  fbx_uint8_t length_of_name;
  fbx_stream_read_uint8_s(importer->stream, &length_of_name);

  /* Names are not null-terminated. */
  char name[255+1] = { 0, };
  fbx_stream_read_s(importer->stream, (void *)name, length_of_name);

  fbx_node_t *node = fbx_importer_alloc_a_node(importer);

  *ptr_to_node = node;

  if (length_of_name > 0) {
    /* Copy name for posterity. */
    node->name = fbx_importer_intern_a_string(importer, name, length_of_name);
  }

  if (size_of_data > 0) {
    /* Copy data for procesing at higher level. */
    node->data = fbx_block_allocate_s(&importer->memory.transient, size_of_data);
    fbx_stream_read_s(importer->stream, (void *)node->data, size_of_data);
    node->num_of_datums = num_of_datums;
    node->size_of_data = size_of_data;
  }

  /* A node has children if there are left over bytes. */
  const fbx_bool_t children = (fbx_stream_tell(importer->stream) < end);

  if (children) {
    fbx_node_t **child = &node->children;

    for (;;) {
      if (fbx_extract_a_node(importer, *child ? &(*child)->sibling : child)) {
        if (*child == NULL)
          /* Empty node delineates end of children by acting as sentinel. */
          break;
        /* Link child to parent. */
        (*child)->parent = node;
        /* Possibly more... */
        child = &(*child)->sibling;
      } else {
        /* Failed to process a child. */
        return FBX_FALSE;
      }
    }

    /* Extract properties if any exist. */
    fbx_extract_any_properties(importer, node);
  } else {
    /* No children. */
    node->children = NULL;

    /* So definitely no properties. */
    node->properties = NULL;
    node->num_of_properties = 0;
  }

  /* Skip the sentinel. */
  fbx_stream_seek(importer->stream, end);

  return FBX_TRUE;
}

/* TODO(mtwilliams): Replace `fbx_panic` with logging functionality tied to
   importer or entire library. */

/* TODO(mtwilliams): Use `setjmp` and `longjmp` to easily early out? */

static fbx_bool_t fbx_importer_extract(fbx_importer_t *importer) {
  fbx_node_t **node = &importer->nodes->children;

  for (;;) {
    if (fbx_extract_a_node(importer, *node ? &(*node)->sibling : node)) {
      if (*node == NULL)
        /* Empty node signifies end. */
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
  /* We assume the header is canonical. */
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

  /* TODO(mtwilliams): Determine if keywords are given as one string per
     keyword or as a delimiter separated list. */

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
      fbx_importer_intern_a_string_from_ref(importer,
                                            app_vendor_property->value.as_a_ref);
  } else {
    importer->fbx->tool.vendor = importer->an_empty_string;
  }

  if (app_name_property = fbx_node_property_by_name(scene_info_node, "LastSaved|ApplicationName")) {
    importer->fbx->tool.name =
      fbx_importer_intern_a_string_from_ref(importer,
                                            app_name_property->value.as_a_ref);
  } else {
    importer->fbx->tool.name = importer->an_empty_string;
  }

  if (app_version_property = fbx_node_property_by_name(scene_info_node, "LastSaved|ApplicationVersion")) {
    importer->fbx->tool.version =
      fbx_importer_intern_a_string_from_ref(importer,
                                            app_version_property->value.as_a_ref);
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

  importer->fbx->exporter.name =
    fbx_importer_intern_a_string_from_ref(importer, ref_to_exporter);

  return FBX_TRUE;
}

static fbx_bool_t fbx_process_a_header_extension_node(fbx_importer_t *importer,
                                                      fbx_node_t *header_extension_node)
{
  fbx_node_t *header_version_node,
             *version_node;

  fbx_node_child_by_name_s(header_extension_node, "FBXHeaderVersion", header_version_node);
  fbx_node_child_by_name_s(header_extension_node, "FBXVersion", version_node);

  /* Check header version. */
  if (!fbx_process_a_header_version_node(importer, header_version_node))
    return FBX_FALSE;

  /* Validate version information against file header. */
  if (!fbx_process_a_version_node(importer, version_node))
    return FBX_FALSE;

  fbx_node_t *creation_timestamp_node;
  fbx_node_child_by_name_s(header_extension_node, "CreationTimeStamp", creation_timestamp_node);

  /* Extract timestamp. */
  if (!fbx_process_a_creation_timestamp_node(importer, creation_timestamp_node))
    return FBX_FALSE;

  fbx_node_t *scene_info_node,
             *creator_node;

  fbx_node_child_by_name_s(header_extension_node, "SceneInfo", scene_info_node);
  fbx_node_child_by_name_s(header_extension_node, "Creator", creator_node);

  /* Extract metadata. */
  if (!fbx_process_a_scene_info_node(importer, scene_info_node))
    return FBX_FALSE;
  if (!fbx_process_a_creator_node(importer, creator_node))
    return FBX_FALSE;

  return FBX_TRUE;
}

static fbx_vec3_t fbx_vector_from_axis_and_sign(fbx_int32_t axis,
                                               fbx_int32_t sign)
{
  switch (axis) {
    case 0: return { sign ? 1.f : -1.f, 0.f, 0.f };
    case 1: return { 0.f, sign ? 1.f : -1.f, 0.f };
    case 2: return { 0.f, 0.f, sign ? 1.f : -1.f };
  }

  return { 0.f, 0.f, 0.f };
}

/* TODO(mtwilliams): Determine if basis is correct. */
/* TODO(mtwilliams): Derive units-per-meter? */

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

  importer->fbx->basis.up = fbx_vector_from_axis_and_sign(up_axis, up_axis_sign);

  const fbx_int32_t front_axis = front_axis_property->value.as_an_integer;
  const fbx_int32_t front_axis_sign = front_axis_sign_property->value.as_an_integer;

  /* We flip the sign since the "front axis" points towards the screen. */
  importer->fbx->basis.forward = fbx_vector_from_axis_and_sign(front_axis, !front_axis_sign);

  const fbx_int32_t coord_axis = coord_axis_property->value.as_an_integer;
  const fbx_int32_t coord_axis_sign = coord_axis_sign_property->value.as_an_integer;

  importer->fbx->basis.right = fbx_vector_from_axis_and_sign(coord_axis, coord_axis_sign);

  /* HACK(mtwilliams): Assume origin at (0, 0, 0). */
  importer->fbx->origin = { 0.f, 0.f, 0.f };

  return FBX_TRUE;
}

static fbx_bool_t fbx_process_any_definitions(fbx_importer_t *importer,
                                              fbx_node_t *definitions_node)
{
  /* We have to count the number of definitions, because unlike the name would
     imply, the "Count" node refers to the number of objects (using the
     definitions?) rather than the number of definitions themselves? This might
     very well be a bug that's been solidified in the name of backwards
     compatibility. Sigh. */

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
      /* Only care about definitions. */
      continue;

    fbx_template_t *tmpl = &templates[encountered++];

    tmpl->definition = definition_node;

    fbx_ref_to_data_t ref_to_name;
    fbx_extract_a_datum_from_node_s(FBX_STRING_DATUM, definition_node, &ref_to_name);

    tmpl->name = fbx_importer_intern_a_string_from_ref(importer, ref_to_name);

    /* Appears that some definitions don't provide a template at all? */
    if (fbx_node_t *template_node = fbx_node_child_by_name(definition_node, "PropertyTemplate")) {
      tmpl->properties = template_node->properties;
      tmpl->num_of_properties = template_node->num_of_properties;
    } else {
      tmpl->properties = NULL;
      tmpl->num_of_properties = 0;
    }

    /* Preemptively cache pointers to templates to amortize cost of lookups. */
    if (strcmp(tmpl->name, "Model") == 0)
      importer->model_template = tmpl;
    else if (strcmp(tmpl->name, "Geometry") == 0)
      importer->geometry_template = tmpl;
  }

  return FBX_TRUE;
}

#define fbx_property_or_default_by_name(Base, Node, Name, Property) \
  if (!(Property = fbx_node_property_by_name(Node, Name))) {        \
    if (Base) {                                                     \
      Property = fbx_template_property_by_name(Base, Name);         \
    }                                                               \
  }

#define fbx_property_or_default_by_name_s(Base, Node, Name, Property) \
  if (!(Property = fbx_node_property_by_name(Node, Name))) {          \
    if (Base) {                                                       \
      if (!(Property = fbx_template_property_by_name(Base, Name))) {  \
        return FBX_FALSE;                                             \
      }                                                               \
    } else {                                                          \
      return FBX_FALSE;                                               \
    }                                                                 \
  }

#define fbx_property_is_type_s(Type, Property) \
  if (Property->type != Type) {                \
    return FBX_FALSE;                          \
  }


/* TODO(mtwillaism): Extract information required for inverse kinematics. */

/* TODO(mtwilliams): Constitute transform properly. */

typedef enum fbx_order_of_rotation {
  FBX_XYZ = 0,
  FBX_XZY = 1,
  FBX_YZX = 2,
  FBX_YXZ = 3,
  FBX_ZXY = 4,
  FBX_ZYX = 5
} fbx_order_of_rotation_t;

static fbx_bool_t fbx_order_of_rotation_recognized(fbx_order_of_rotation_t order)
{
  switch (order) {
    case FBX_XYZ:
    case FBX_XZY:
    case FBX_YZX:
    case FBX_YXZ:
    case FBX_ZXY:
    case FBX_ZYX:
      return FBX_TRUE;
  }

  return FBX_FALSE;
}

/* PERF(mtwilliams): Analytically derive specialization for all variants? */

fbx_quaternion_t fbx_quat_from_axis_angle(const fbx_vec3_t axis,
                                          const fbx_real32_t angle)
{
  const fbx_real32_t sin_of_half_angle = sinf(angle * 0.5f);
  const fbx_real32_t cos_of_half_angle = cosf(angle * 0.5f);

  return {
    sin_of_half_angle * axis.x,
    sin_of_half_angle * axis.y,
    sin_of_half_angle * axis.z,
    cos_of_half_angle
  };
}

fbx_quaternion_t fbx_quat_mul(const fbx_quaternion_t lhs,
                              const fbx_quaternion_t rhs)
{
  return {
    lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
    lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
    lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
    lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
  };
}

static fbx_quaternion_t rotation_from_order_and_angles(fbx_order_of_rotation_t order,
                                                       const fbx_vec3_t angles)
{
  fbx_quaternion_t rot[3];

  rot[0] = fbx_quat_from_axis_angle({ 1.f, 0.f, 0.f }, fbx_degrees_to_radians(angles.x));
  rot[1] = fbx_quat_from_axis_angle({ 0.f, 1.f, 0.f }, fbx_degrees_to_radians(angles.y));
  rot[2] = fbx_quat_from_axis_angle({ 0.f, 0.f, 1.f }, fbx_degrees_to_radians(angles.z));

  fbx_size_t idx[3];

  switch (order) {
    case FBX_XYZ: idx[0] = 0; idx[1] = 1; idx[2] = 2; break;
    case FBX_XZY: idx[0] = 0; idx[1] = 2; idx[2] = 1; break;
    case FBX_YZX: idx[0] = 1; idx[1] = 2; idx[2] = 0; break;
    case FBX_YXZ: idx[0] = 1; idx[1] = 0; idx[2] = 2; break;
    case FBX_ZXY: idx[0] = 2; idx[1] = 0; idx[2] = 1; break;
    case FBX_ZYX: idx[0] = 2; idx[1] = 1; idx[2] = 0; break;
  }

  return fbx_quat_mul(fbx_quat_mul(rot[idx[0]], rot[idx[1]]), rot[idx[2]]);
}

static void *fbx_reify_a_model(fbx_importer_t *importer,
                               fbx_node_t *node)
{
  const fbx_template_t *base = importer->model_template;

  fbx_model_t *model = fbx_importer_alloc_a_model(importer);

  const fbx_property_t *translation_active_property,
                       *rotation_active_property,
                       *scaling_active_property;

  fbx_property_or_default_by_name_s(base, node, "TranslationActive", translation_active_property);
  fbx_property_or_default_by_name_s(base, node, "RotationActive", rotation_active_property);
  fbx_property_or_default_by_name_s(base, node, "ScalingActive", scaling_active_property);
  
  fbx_property_is_type_s(FBX_BOOLEAN_PROPERTY, translation_active_property);
  fbx_property_is_type_s(FBX_BOOLEAN_PROPERTY, rotation_active_property);
  fbx_property_is_type_s(FBX_BOOLEAN_PROPERTY, scaling_active_property);

  const fbx_property_t *local_translation_property,
                       *local_rotation_property,
                       *local_scaling_property;

  fbx_property_or_default_by_name_s(base, node, "Lcl Translation", local_translation_property);
  fbx_property_or_default_by_name_s(base, node, "Lcl Rotation", local_rotation_property);
  fbx_property_or_default_by_name_s(base, node, "Lcl Scaling", local_scaling_property);

  fbx_property_is_type_s(FBX_VECTOR_PROPERTY, local_translation_property);
  fbx_property_is_type_s(FBX_VECTOR_PROPERTY, local_rotation_property);
  fbx_property_is_type_s(FBX_VECTOR_PROPERTY, local_scaling_property);

  fbx_bool_t translated = translation_active_property->value.as_a_boolean;
  fbx_bool_t rotated = rotation_active_property->value.as_a_boolean;
  fbx_bool_t scaled = scaling_active_property->value.as_a_boolean;

  /* HACK(mtwilliams): Appears that translation and scaling are implicit? */
  translated |= FBX_TRUE;
  scaled |= FBX_TRUE;

  if (translated) {
    model->transform.position = local_translation_property->value.as_a_vector;
  } else {
    /* No translation. */
    model->transform.position = { 0.f, 0.f, 0.f };
  }

  if (rotated) {
    const fbx_property_t *order_of_rotation_property;
    fbx_property_or_default_by_name_s(base, node, "RotationOrder", order_of_rotation_property);
    fbx_property_is_type_s(FBX_ENUM_PROPERTY, order_of_rotation_property);

    const fbx_order_of_rotation_t order_of_rotation =
      fbx_order_of_rotation_t(order_of_rotation_property->value.as_an_integer);

    if (!fbx_order_of_rotation_recognized(order_of_rotation))
      /* TODO(mtwilliams): Handle spheric rotation? */
      fbx_importer_error(importer, FBX_EFEATURE, "Unsupported rotation order.");

    fbx_quaternion_t rotation = 
      rotation_from_order_and_angles(order_of_rotation,
                                     local_rotation_property->value.as_a_vector);

    const fbx_property_t *pre_rotation_property,
                         *post_rotation_property;

    fbx_property_or_default_by_name_s(base, node, "PreRotation", pre_rotation_property);
    fbx_property_or_default_by_name_s(base, node, "PostRotation", post_rotation_property);

    fbx_property_is_type_s(FBX_VECTOR_PROPERTY, pre_rotation_property);
    fbx_property_is_type_s(FBX_VECTOR_PROPERTY, post_rotation_property);

    fbx_quaternion_t pre_rotation =
      rotation_from_order_and_angles(order_of_rotation,
                                     pre_rotation_property->value.as_a_vector);

    fbx_quaternion_t post_rotation =
      rotation_from_order_and_angles(order_of_rotation,
                                     post_rotation_property->value.as_a_vector);

    /* Should we bake pre-rotation and post-rotation into rotation? */

    // pre_rotation * rotation * inverse(post_rotation)
    model->transform.rotation = rotation;
  
    const fbx_property_t *rotation_pivot_property,
                         *rotation_offset_property;

    fbx_property_or_default_by_name_s(base, node, "RotationPivot", rotation_pivot_property);
    fbx_property_or_default_by_name_s(base, node, "RotationOffset", rotation_offset_property);
    
    fbx_property_is_type_s(FBX_VECTOR_PROPERTY, rotation_pivot_property);
    fbx_property_is_type_s(FBX_VECTOR_PROPERTY, rotation_offset_property);

    model->transform.center_of_rotation = rotation_pivot_property->value.as_a_vector;
    model->transform.point_of_rotation = rotation_offset_property->value.as_a_vector;
  } else {
    /* No rotation. */
    model->transform.rotation = { 0.f, 0.f, 0.f, 1.f };

    /* No pivot or offset. */
    model->transform.center_of_rotation = { 0.f, 0.f, 0.f };
    model->transform.point_of_rotation = { 0.f, 0.f, 0.f };
  }

  if (scaled) {
    model->transform.scale = local_scaling_property->value.as_a_vector;

    const fbx_property_t *scaling_pivot_property,
                         *scaling_offset_property;

    fbx_property_or_default_by_name_s(base, node, "ScalingPivot", scaling_pivot_property);
    fbx_property_or_default_by_name_s(base, node, "ScalingOffset", scaling_offset_property);
    
    fbx_property_is_type_s(FBX_VECTOR_PROPERTY, scaling_pivot_property);
    fbx_property_is_type_s(FBX_VECTOR_PROPERTY, scaling_offset_property);

    model->transform.center_of_scaling = scaling_pivot_property->value.as_a_vector;
    model->transform.point_of_scaling = scaling_offset_property->value.as_a_vector;
  } else {
    /* No scaling. */
    model->transform.scale = { 1.f, 1.f, 1.f };

    /* No pivot or offset. */
    model->transform.center_of_scaling = { 0.f, 0.f, 0.f };
    model->transform.point_of_scaling = { 0.f, 0.f, 0.f };
  }

  return (void *)model;
}

#if 0

static fbx_bool_t fbx_identify_topology_of_geometry(fbx_importer_t *importer,
                                                    fbx_data_t fd,
                                                    fbx_topology_t *topology)
{
  const fbx_int32_t *indices = fd.ptr_to_int32;

  /* Determine topology by finding the first negative index. */
  if (indices[1-1] < 0) return *topology = FBX_POINT, FBX_TRUE;
  if (indices[3-1] < 0) return *topology = FBX_TRIANGLE, FBX_TRUE;
  if (indices[4-1] < 0) return *topology = FBX_QUAD, FBX_TRUE;
  if (indices[5-1] < 0) return *topology = FBX_PENTAGONAL, FBX_TRUE;
  if (indices[6-1] < 0) return *topology = FBX_HEXAGONAL, FBX_TRUE;
  if (indices[7-1] < 0) return *topology = FBX_HEPTAGONAL, FBX_TRUE;
  if (indices[8-1] < 0) return *topology = FBX_OCTAGONAL, FBX_TRUE;

  /* Unknown topology. Probably something esoteric. */
  return FBX_FALSE;
}

#endif

/* TODO(mtwilliams): Check GeometryVersion? */
/* MEM(mtwilliams): Streaming decode? */

/* TODO(mtwilliams): Peek at next index to determine what index our last
   vertex of our last triangle should be and use that to select the order
   we triangulate this polygon to make triangle strips rather than soups. */

/* Fan triangulation. */
static fbx_uint32_t triangulate(const fbx_int32_t *indices,
                                fbx_uint32_t n,
                                fbx_uint32_t *derived)
{
  for (fbx_uint32_t i = 1, j = 2; j < n-1; i = j++) {
    *derived++ = indices[0];
    *derived++ = indices[i];
    *derived++ = indices[j];
  }

  /* Last triangle is special-cased since the last index is complemented to
     indicate end of polygon. */
  *derived++ = indices[0];
  *derived++ = indices[n-2];
  *derived++ = ~indices[n-1];

  return n - 2;
}

static void *fbx_reify_a_mesh(fbx_importer_t *importer,
                              fbx_node_t *node)
{
  const fbx_template_t *base = importer->geometry_template;

  fbx_mesh_t *mesh = fbx_importer_alloc_a_mesh(importer);

  /* Allocate space to track streams. */
  void **streams = (void **)fbx_block_allocate_s(&importer->memory.permanent,
                                                 1 * sizeof(void *));

  /* Pass through so users can iterate over all channels and corresponding streams. */
  mesh->streams = streams;

  fbx_node_t *vn;
  fbx_node_child_by_name_s(node, "Vertices", vn);

  /* We extract vertex data into transient memory rather than permanent memory
     because we don't want to pass through double-width coordinates to users. */
  fbx_data_t vd;
  fbx_extract_a_datum_from_node_s(FBX_REAL64_DATA, vn, &vd);
  fbx_decode_an_array_s(&vd, &importer->memory.transient);

  /* We're fed a raw array so the number of vertices is actually a third. */
  mesh->num_of_vertices = vd.count / 3;

  /* Allocate storage for coordinates that we'll pass to users. */
  fbx_real32_t *positions =
    (fbx_real32_t *)fbx_importer_permanent_alloc(importer,
                                                 vd.count * sizeof(fbx_real32_t));

  /* Copy coordinates to permanent memory, halving precision along the way. */
  for (fbx_uint32_t i = 0; i < vd.count; ++i)
    positions[i] = fbx_real32_t(vd.ptr_to_real64[i]); 

  /* Positions always occupy first stream. */
  streams[0] = (void *)positions;

  fbx_node_t *fn;
  fbx_node_child_by_name_s(node, "PolygonVertexIndex", fn);

  /* Extract polygonal data into temporary memory since topology can change on
     us willy-nilly and we want to pass users sane topology. */
  fbx_data_t fd;
  fbx_extract_a_datum_from_node_s(FBX_INT32_DATA, fn, &fd);
  fbx_decode_an_array_s(&fd, &importer->memory.transient);

  /* Number of polyons and triangles. */
  fbx_uint32_t np = 0, nt = 0;

  /* Walk over indices to determine number polygons and triangles. */
  for (fbx_uint32_t i = 0, n = 0; i < fd.count; ++i, ++n)
    if (fd.ptr_to_int32[i] < 0)
      np++, nt += n - 2, n = 0;

  /* Allocate space for mapping from polygon to triangles. */
    /* This is filled while triangulating XXX. */

  /* Allocate space for triangulated indicies. */
  fbx_uint32_t *indices =
    (fbx_uint32_t *)fbx_importer_permanent_alloc(importer,
                                                 3 * nt * sizeof(fbx_uint32_t));

  mesh->indices = indices;

  /* Triangulate polygons to normalize topology. */
  for (fbx_uint32_t i = 0, j = 0, t = 0; i < fd.count; ++i) {
    if (fd.ptr_to_int32[i] < 0) {
      t += triangulate(&fd.ptr_to_int32[j], i-j+1, &indices[t*3]);
      j = i + 1;
    }
  }

  mesh->num_of_faces = nt;

  /* Provide direct pointers to commonly used streams. */
  mesh->positions = streams[0];

  return (void *)mesh;
}

    /* Layer */
   /* LayerElement */
    /* Type */
    /* TypedIndex */

  /* LayerElement??? */
   /* Version? */
   /* Name */
   /* MappingInformationType */
   /* ReferenceInformationType */
  /* LayerElementNormal */
   /* Normals */
   /* NormalsW */
  /* LayerElementUV */
   /* UV */
   /* UVIndex */
  /* LayerElementSmoothing */
   /* Smoothing */
  /* LayerElementMaterial */
   /* Materials */

/* Features */
 /* Transforms */
  /* With contraints, including locking. */
 /* Vertices, Edges, and Faces. */
  /* Topology */
 /* Bounding Boxes */
  /* Computed? */
 /* Normal, tangent, and binormal calculation. */
  /* Or import. */
  /* Smooth or not. */

/* TODO(mtwilliams): Object indirection. */
 /* Need to map UIDs to `fbx_object_t *`. */
 /* Need to map `fbx_object_t *` to relevant. */

/* BUG(mtwilliams): Appears that "Objects" and "Connections" may be optional
   nodes. The resulting file wouldn't be useful, but should be handled
   gracefully. */

static fbx_object_t *fbx_reify_an_object(fbx_importer_t *importer,
                                         fbx_node_t *node)
{
  fbx_object_t *object = fbx_importer_alloc_an_object(importer);

  const void *cursor = node->data;

  fbx_uint64_t id;
  fbx_extract_a_datum_s(FBX_INT64_DATUM, cursor, &id);

  fbx_ref_to_data_t ref_to_name_and_class;
  fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &ref_to_name_and_class);

  fbx_ref_to_data_t ref_to_sub_class;
  fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &ref_to_sub_class);

  object->id = id;

  /* Name is delimited from class by `\x0\x1`. */
  if (fbx_size_t length_of_name = strlen(ref_to_name_and_class.string)) {
    object->name = fbx_importer_intern_a_string(importer,
                                                ref_to_name_and_class.string,
                                                length_of_name);
  } else {
    object->name = NULL;
  }

  /* Rather than relying on the hard to decipher type and sub-type strings we
     guess the type from the node's name. This appears to work well enough. */
  if (strcmp(node->name, "Model") == 0) {
    object->type = FBX_MODEL;
    object->reified = fbx_reify_a_model(importer, node);
  } else if (strcmp(node->name, "Geometry") == 0) {
    object->type = FBX_MESH;
    object->reified = fbx_reify_a_mesh(importer, node);
  } else {
    object->type = FBX_UNKNOWN;
    object->reified = NULL;
    return object;
  }

  if (!object->reified)
    fbx_importer_error(importer, FBX_EDATA, "Reification of %llu failed.", object->id);

  return object;
}

typedef enum fbx_type_of_connection {
  FBX_OBJECT_TO_OBJECT     = 'OO',
  FBX_OBJECT_TO_PROPERTY   = 'OP',
  FBX_PROPERTY_TO_OBJECT   = 'PO',
  FBX_PROPERTY_TO_PROPERTY = 'PP'
} fbx_type_of_connection_t;

/* PERF(mtwilliams): Use index make lookups cheaper. */

static fbx_object_t *fbx_scene_object_by_id(fbx_scene_t *scene,
                                            fbx_uint64_t id)
{
  for (fbx_uint32_t object = 0; object < scene->num_of_objects; ++object)
    if (scene->objects[object]->id == id)
      return scene->objects[object];

  return NULL;
}

static void fbx_connect_an_object_to_another_object(fbx_importer_t *importer,
                                                    fbx_object_t *parent,
                                                    fbx_object_t *child)
{
  if (parent->children == NULL) {
    /* Allocate enough space to support up to 255 children. */
    parent->children =
      (fbx_object_t **)fbx_importer_permanent_alloc(importer,
                                                    255 * sizeof(fbx_object_t *));
  } else {
    if (parent->num_of_children >= 255) {
      fbx_importer_error(importer, FBX_ESPACE, "Too many children under %llu!", parent->id);
    }
  }

  if (child->parent != NULL) {
    /* Although other types of connections can allow multiple links emanating
       from the same object, this makes no sense because it's an analogue to a
       scene graph. */
    fbx_importer_error(importer, FBX_EDATA, "Cannot reparent %llu under %llu.", child->id, child->parent);
  }

  parent->children[parent->num_of_children++] = child;
}

static fbx_bool_t fbx_reify_applicable_objects_and_connections(fbx_importer_t *importer,
                                                               fbx_node_t *root)
{
  fbx_node_t *definitions_node;
  fbx_node_child_by_name_s(root, "Definitions", definitions_node);

  fbx_node_t *count_node;
  fbx_node_child_by_name_s(definitions_node, "Count", count_node);

  /* Non-intuitively, this is actually the number of objects. Yeah... */
  fbx_uint32_t count;
  fbx_extract_a_datum_from_node_s(FBX_INT32_DATUM, count_node, &count);

  /* Appears that there's an extra object counted because "GlobalSettings" has
     a definition. */
  count -= 1;

  /* Reserve space for implicit root object. */
  count += 1;

  fbx_object_t **objects =
    (fbx_object_t **)fbx_block_allocate_s(&importer->memory.permanent,
                                          count * sizeof(fbx_object_t *));

  /* We manually insert a root object since it appears to be implicit. */
  objects[0] = fbx_importer_alloc_an_object(importer);
  objects[0]->id = 0;
  objects[0]->type = FBX_EMPTY;

  importer->fbx->scene.root = objects[0];
  importer->fbx->scene.objects = objects;
  importer->fbx->scene.num_of_objects = count;

  fbx_node_t *objects_node;
  fbx_node_child_by_name_s(root, "Objects", objects_node);

  fbx_uint32_t reified = 0;

  for (fbx_node_t *object_node = objects_node->children; object_node; object_node = object_node->sibling)
    objects[++reified] = fbx_reify_an_object(importer, object_node);

  fbx_node_t *connections_node;
  fbx_node_child_by_name_s(root, "Connections", connections_node);

  for (fbx_node_t *connection_node = connections_node->children; connection_node; connection_node = connection_node->sibling) {
    const void *cursor = connection_node->data;

    fbx_ref_to_data_t ref_to_type;
    fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &ref_to_type);

    union { fbx_type_of_connection_t type; char type_as_string[2]; };
    fbx_zero(&type, sizeof(type));
    fbx_copy(ref_to_type.pointer, type_as_string, 2);

    fbx_uint64_t id_of_parent, id_of_child;
    fbx_extract_a_datum_s(FBX_INT64_DATUM, cursor, &id_of_child);
    fbx_extract_a_datum_s(FBX_INT64_DATUM, cursor, &id_of_parent);

    switch (type) {
      case FBX_OBJECT_TO_OBJECT: {
        fbx_object_t *parent = fbx_scene_object_by_id(&importer->fbx->scene, 
                                                      id_of_parent);

        fbx_object_t *child = fbx_scene_object_by_id(&importer->fbx->scene, 
                                                     id_of_child);

        fbx_connect_an_object_to_another_object(importer, parent, child);
      } break;

      /* TODO(mtwilliams): Determine if more advanced features rely on these
         types of connections. */
    #if 0
      case FBX_OBJECT_TO_PROPERTY: {
        fbx_ref_to_data_t ref_to_property;
        fbx_extract_a_datum_s(FBX_STRING_DATUM, cursor, &ref_to_property);
      } break;

      case FBX_PROPERTY_TO_OBJECT: {
      } break;

      case FBX_PROPERTY_TO_PROPERTY: {
      } break;
    #endif
    }
  }

  return FBX_TRUE;
}

static fbx_bool_t fbx_importer_process(fbx_importer_t *importer) {
  fbx_node_t *root = importer->nodes;

  /* TODO(mtwilliams): Use `fbx_node_child_by_name_s` to find the header
     extension? */

  fbx_node_t *header_extension_node = root->children;

  /* First node should be a header extension. */
  if (strcmp(header_extension_node->name, "FBXHeaderExtension") != 0)
    fbx_importer_error(importer, FBX_EFORMAT, "Missing header extension.");

  /* Process extended header to gather metadata. */
  if (!fbx_process_a_header_extension_node(importer, header_extension_node))
    fbx_importer_error(importer, FBX_EFORMAT, "Bad header extension.");

  fbx_node_t *global_settings_node;
  fbx_node_child_by_name_s(root, "GlobalSettings", global_settings_node);

  /* Process global settings to determine basis and origin. */
  if (!fbx_process_a_global_settings_node(importer, global_settings_node))
    return FBX_FALSE;

  /* Definitions define types of objects and default properties to associate
     with each instance. */
  if (fbx_node_t *definitions_node = fbx_node_child_by_name(root, "Definitions")) {
    if (!fbx_process_any_definitions(importer, definitions_node))
      return FBX_FALSE;
  } else {
    importer->templates = NULL;
    importer->num_of_templates = 0;

    /* If no definitions are provided, it's reasonable to assume no objects or
       connections are provided, because they're intimately coupled. The result
       is basically an empty scene. */
    importer->is_basically_empty = FBX_TRUE;
  }

  if (importer->is_basically_empty) {
    /* Quickly setup an empty scene and early out. */

    importer->fbx->scene.objects = NULL;
    importer->fbx->scene.num_of_objects = 0;

    return FBX_TRUE;
  }

  /* Process objects and their implicit and explicit connections into a form
     that's easily digested. This is where the magic happens. */
  if (!fbx_reify_applicable_objects_and_connections(importer, root))
    return FBX_FALSE;

  return FBX_TRUE;
}

static fbx_status_t fbx_importer_run(fbx_importer_t *importer)
{
  if (fbx_status_t status = (fbx_status_t)setjmp(importer->error))
    return status;

  if (!fbx_check_for_magic(importer->stream))
    fbx_importer_error(importer, FBX_EFORMAT, "No magic.");

  fbx_extract_version_from_preamble(importer->stream, &importer->fbx->version);

  if (importer->fbx->version < 7100)
    fbx_importer_error(importer, FBX_EVERSION, "Given file uses an outdated format!");

  if (importer->fbx->version > 7500)
    fbx_importer_error(importer, FBX_EVERSION, "Given file uses unsupported format!");

  if (importer->fbx->version >= 7500)
    /* See `fbx_importer_t` for a description of binary incompatible changes
       introduced in FBX 2016. */
    importer->using_newer_format = FBX_TRUE;
  else
    importer->using_newer_format = FBX_FALSE;

  /* Extract node hierarchy from stream into internal representation. */
  fbx_importer_extract(importer);

  /* Walk internal representation to process information into structures. */
  fbx_importer_process(importer);

  /* Free backing memory used to constitute the internal node hierarchy. */
  
  /* Validate and clean up data. */
   /* Includes triangulation the like. */

  return FBX_OK;
}

/*  _____                 _
 * |   __|_ _ ___ ___ ___| |_
 * |   __|_'_| . | . |  _|  _|
 * |_____|_,_|  _|___|_| |_|
 *           |_|
 */

/* ... */

FBX_END_EXTERN_C

#endif
