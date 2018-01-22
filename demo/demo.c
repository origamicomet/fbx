#define WABISABI_IMPLEMENTATION
#include "wabisabi.h"

#define FBX_IMPLEMENTATION
#include "fbx.h"

#define RIGID_VERTEX_SHADER \
  "#version 140                                                           \n" \
  "                                                                       \n" \
  "#extension GL_ARB_explicit_attrib_location : enable                    \n" \
  "                                                                       \n" \
  "//                                                                     \n" \
  "// Constants                                                           \n" \
  "//                                                                     \n" \
  "                                                                       \n" \
  "// Seconds elapsed.                                                    \n" \
  "uniform float time;                                                    \n" \
  "                                                                       \n" \
  "// Seconds spent on the last frame.                                    \n" \
  "uniform float dt;                                                      \n" \
  "                                                                       \n" \
  "// Transforms from world-space to screen-space.                        \n" \
  "uniform mat4 screen_from_world;                                        \n" \
  "                                                                       \n" \
  "// Transforms from screen-space to world-space.                        \n" \
  "uniform mat4 world_from_screen;                                        \n" \
  "                                                                       \n" \
  "// Transforms from object-space to world-space.                        \n" \
  "uniform mat4 world_from_object;                                        \n" \
  "                                                                       \n" \
  "// Transforms from world-space to object-space.                        \n" \
  "uniform mat4 object_from_world;                                        \n" \
  "                                                                       \n" \
  "//                                                                     \n" \
  "// Input                                                               \n" \
  "//                                                                     \n" \
  "                                                                       \n" \
  "layout(location = 0) in vec3 v_position;                               \n" \
  "                                                                       \n" \
  "//                                                                     \n" \
  "// Output                                                              \n" \
  "//                                                                     \n" \
  "                                                                       \n" \
  "out vec4 f_position_in_ws;                                             \n" \
  "out vec4 f_position_in_ss;                                             \n" \
  "                                                                       \n" \
  "void main() {                                                          \n" \
  "  // Transform vertices from object-space to world-space.              \n" \
  "  vec4 position_in_ws = world_from_object * vec4(v_position, 1.f);     \n" \
  "                                                                       \n" \
  "  // Transform vertices from world-space to screen-space.              \n" \
  "  vec4 position_in_ss = screen_from_world * position_in_ws;            \n" \
  "                                                                       \n" \
  "  // Output vertex position.                                           \n" \
  "  gl_Position = position_in_ss;                                        \n" \
  "                                                                       \n" \
  "  // Pass through position (in world-space and screen-space)           \n" \
  "  // to fragment shader.                                               \n" \
  "  f_position_in_ws = position_in_ws;                                   \n" \
  "  position_in_ss = position_in_ss;                                     \n" \
  "}                                                                      \n" \
  "                                                                       \n"

#define RIGID_FRAGMENT_SHADER \
  "#version 140                                                           \n" \
  "                                                                       \n" \
  "#extension GL_ARB_explicit_attrib_location : enable                    \n" \
  "                                                                       \n" \
  "//                                                                     \n" \
  "// Constants                                                           \n" \
  "//                                                                     \n" \
  "                                                                       \n" \
  "// Seconds elapsed.                                                    \n" \
  "uniform float time;                                                    \n" \
  "                                                                       \n" \
  "// Seconds spent on the last frame.                                    \n" \
  "uniform float dt;                                                      \n" \
  "                                                                       \n" \
  "// Transforms from world-space to screen-space.                        \n" \
  "uniform mat4 screen_from_world;                                        \n" \
  "                                                                       \n" \
  "// Transforms from screen-space to world-space.                        \n" \
  "uniform mat4 world_from_screen;                                        \n" \
  "                                                                       \n" \
  "// Transforms from object-space to world-space.                        \n" \
  "uniform mat4 world_from_object;                                        \n" \
  "                                                                       \n" \
  "// Transforms from world-space to object-space.                        \n" \
  "uniform mat4 object_from_world;                                        \n" \
  "                                                                       \n" \
  "//                                                                     \n" \
  "// Input                                                               \n" \
  "//                                                                     \n" \
  "                                                                       \n" \
  "in vec4 f_position_in_ws;                                              \n" \
  "in vec4 f_position_in_ss;                                              \n" \
  "                                                                       \n" \
  "//                                                                     \n" \
  "// Output                                                              \n" \
  "//                                                                     \n" \
  "                                                                       \n" \
  "layout(location = 0) out vec4 gl_FragColor;                            \n" \
  "                                                                       \n" \
  "void main() {                                                          \n" \
  "  // Output fragment color.                                            \n" \
  "  gl_FragColor = vec4(1.f, 1.f, 1.f, 1.f);                             \n" \
  "}                                                                      \n"

ws_frame_t frame;

ws_real32_t time, dt;

ws_mat4_t screen_from_scene,
          scene_from_screen;

/* Toggles rendering of debug overlay. */
ws_bool_t r_debug = WS_TRUE;

static unsigned depth = 0;

static void printf_with_indentation(unsigned indentation, const char *format, ...)
{
  if (indentation)
    printf("%*.c", indentation, '\0');

  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);
}

static void print_info_about_model(const fbx_model_t *model)
{
  /* TODO(mtwilliams): Use helpers that deal with inheritance mode. */

  const fbx_vec3_t p = model->position;
  const fbx_quaternion_t r = model->rotation;
  const fbx_vec3_t s = model->scale;

  printf_with_indentation(depth*2+1, "Positioned at <%f, %f, %f> relative to parent.\n", p.x, p.y, p.z);
  printf_with_indentation(depth*2+1, "Rotated with <%f, %f, %f, %f> relative to parent.\n", r.x, r.y, r.z, r.w);
  printf_with_indentation(depth*2+1, "Scaled by <%f, %f, %f> relative to parent.\n", s.x, s.y, s.z);
}

static const char *TYPE_TO_STRING[] = { "Unknown", "Empty", "Model", "Mesh" };

static void walk_and_print(const fbx_object_t *object)
{
  printf_with_indentation(depth*2, "%s (%llu)\n", TYPE_TO_STRING[object->type], object->id);

  if (object->name)
    printf_with_indentation(depth*2+1, "Called \"%s\".\n", object->name);

  switch (object->type) {
    case FBX_MODEL: print_info_about_model((const fbx_model_t *)object->reified); break;
  }

  for (fbx_uint32_t child = 0; child < object->num_of_children; ++child) {
    depth++;
    walk_and_print(object->children[child]);
    depth--;
  }
}

ws_mat4_t transform_from_nothing(const fbx_object_t *object);
ws_mat4_t transform_from_empty(const fbx_object_t *object);
ws_mat4_t transform_from_model(const fbx_object_t *object);
ws_mat4_t transform_from_mesh(const fbx_object_t *object);

void render_an_empty(const fbx_object_t *object);
void render_a_model(const fbx_object_t *object);
void render_a_mesh(const fbx_object_t *object);

void render_an_object(const fbx_object_t *object,
                      const ws_mat4_t parent_to_scene)
{
  static ws_mat4_t (*transformers[])(const fbx_object_t *) = {
    transform_from_nothing,
    transform_from_empty,
    transform_from_model,
    transform_from_mesh
  };

  const ws_mat4_t object_to_parent = transformers[object->type](object);
  const ws_mat4_t object_to_scene = ws_mat4_mul(parent_to_scene, object_to_parent);

  if (r_debug) {
    if (object->name && object->type == FBX_MODEL) {
      const ws_mat4_t object_to_screen = ws_mat4_mul(screen_from_scene, object_to_scene);
      const ws_vec3_t position_in_ss = ws_mat4_transform_vec3(object_to_screen, { 0.f, 0.f, 0.f });

      ws_int32_t x = frame.width * (position_in_ss.x * 0.5f + 0.5f);
      ws_int32_t y = frame.height * (position_in_ss.y * 0.5f + 0.5f);

      ws_debug_text(x+1, y+1, 0xff000000, WS_DEBUG_CENTER_ALIGN, object->name);
      ws_debug_text(x, y, 0xffffffff, WS_DEBUG_CENTER_ALIGN, object->name);
    }
  }

  for (unsigned child = 0; child < object->num_of_children; ++child)
    render_an_object(object->children[child], object_to_scene);
}

ws_mat4_t transform_from_nothing(const fbx_object_t *object)
{
  return WS_IDENTITY_MATRIX;
}

ws_mat4_t transform_from_empty(const fbx_object_t *object)
{
  return WS_IDENTITY_MATRIX;
}

ws_mat4_t transform_from_model(const fbx_object_t *object)
{
  const fbx_model_t *model = (const fbx_model_t *)object->reified;

  return ws_compose_a_mat4(*(const ws_vec3_t *)&model->position,
                           *(const ws_quaternion_t *)&model->rotation,
                           *(const ws_vec3_t *)&model->scale);
}

ws_mat4_t transform_from_mesh(const fbx_object_t *object)
{
  return WS_IDENTITY_MATRIX;
}

typedef struct camera {
  ws_vec3_t position;
  ws_quaternion_t rotation;
  ws_real32_t field_of_view;
  ws_real32_t near;
  ws_real32_t far;
} camera_t;

ws_mat4_t camera_to_projection(const camera_t *camera,
                               ws_uint32_t width_of_target,
                               ws_uint32_t height_of_target)
{
  return ws_perspective_to_mat4(camera->field_of_view,
                                ws_real32_t(width_of_target) / ws_real32_t(height_of_target),
                                camera->near,
                                camera->far);
}

ws_mat4_t camera_to_view(const camera_t *camera)
{
  const ws_mat4_t matrix = ws_compose_a_mat4(camera->position,
                                             camera->rotation,
                                             { -1.f, 1.f, 1.f });

  return ws_mat4_inverse(matrix);
}

void camera_yaw(camera_t *camera, const ws_real32_t angle)
{
  const ws_vec3_t up = { 0.f, 1.f, 0.f };
  const ws_quaternion_t rotation = ws_quat_from_axis_angle(up, angle);
  camera->rotation = ws_quat_mul(camera->rotation, rotation);
}

void camera_pitch(camera_t *camera, const ws_real32_t angle)
{
  const ws_vec3_t right = { 1.f, 0.f, 0.f };
  const ws_quaternion_t rotation = ws_quat_from_axis_angle(right, angle);
  camera->rotation = ws_quat_mul(camera->rotation, rotation);
}

void camera_roll(camera_t *camera, const ws_real32_t angle)
{
  const ws_vec3_t forward = { 0.f, 0.f, 1.f };
  const ws_quaternion_t rotation = ws_quat_from_axis_angle(forward, angle);
  camera->rotation = ws_quat_mul(camera->rotation, rotation);
}

typedef struct camera_freelook_options {
  /* Horizontal and vertical sensitivity in degrees per second. */
  struct {
    ws_real32_t horizontal;
    ws_real32_t vertical;
  } sensitivity;

  /* Speed in units per second. */
  ws_real32_t speed;
} camera_freelook_options_t;

/* TODO(mtwilliams): Freeze up vector. */

void camera_freelook(camera_t *camera,
                     const camera_freelook_options_t *options,
                     const ws_real32_t dt,
                     const ws_vec2_t look,
                     const ws_vec3_t move)
{
  /* Apply horizontal and vertical rotation. */
  camera_yaw(camera, 2.f * WS_PI * look.x * options->sensitivity.horizontal * dt);
  camera_pitch(camera, 2.f * WS_PI * look.y * options->sensitivity.vertical * dt);

  /* Build translation vector from input. */
  ws_vec3_t translation = ws_vec3_muls(move, options->speed * dt);

  /* Then transform so movement to make it relative to look direction. */
  translation = ws_quat_transform(camera->rotation, translation);

  /* Then translate. */
  camera->position.x += translation.x;
  camera->position.y += translation.y;
  camera->position.z += translation.z;
}

/* Our basic freelook camera controller. */
static void freelook(camera_t *camera, ws_real32_t dt) {
  ws_vec2_t look = { 0.f, 0.f };
  ws_vec3_t move = { 0.f, 0.f, 0.f };

  if (frame.mouse.left) {
    look.x = -frame.mouse.delta.x;
    look.y = -frame.mouse.delta.y;
  }

  if (frame.keyboard.w)
    move.z -= 1.f;
  if (frame.keyboard.s)
    move.z += 1.f;

  if (frame.keyboard.a)
    move.x -= 1.f;
  if (frame.keyboard.d)
    move.x += 1.f;

  camera_freelook_options_t options;
  options.sensitivity.horizontal = 0.5f;
  options.sensitivity.vertical = 0.5f;
  options.speed = frame.keyboard.shift ? 100.f : 10.f;

  camera_freelook(camera, &options, dt, look, move);
}

void render(const fbx_scene_t *scene)
{
  /* Walk scene to render. */
  render_an_object(scene->root, WS_IDENTITY_MATRIX);
}

int main(int argc, const char *argv[]) {
#if 0
  static const char *path = "demo/data/monkey.fbx";
#else
  static const char *path = "demo/data/boat.fbx";
#endif

  fbx_import_options_t options;

  options.permanent_memory_pool = 1048575;
  options.transient_memory_pool = 1048575;
  options.strings_memory_pool = 1048575;

  fbx_importer_t *importer = fbx_importer_setup(&options);

  importer->stream = fbx_stream_open_from_path(path, "r");

  if (!importer->stream) {
    fprintf(stderr, "Cannot open \"%s\"!\n", path);
    return EXIT_FAILURE;
  }

  printf("Loading \"%s\"...\n", path);

  if (fbx_importer_run(importer) != FBX_OK)
    return fprintf(stderr, "Failed.\n"), EXIT_FAILURE;
  else
    printf("Ok.\n");

  fbx_stream_close(importer->stream);

  fbx_size_t memory_used_for_import = importer->memory.permanent.offset
                                    + importer->memory.transient.offset
                                    + importer->memory.strings.offset;

  printf("Used %u (%u/%u/%u) bytes of memory.\n",
         memory_used_for_import,
         importer->memory.permanent.offset,
         importer->memory.transient.offset,
         importer->memory.strings.offset);

  printf("Version is %u.\n",
         importer->fbx->version);

  printf("Created at %4u/%02u/%02u %02u:%02u:%02u:%04u.\n",
         importer->fbx->timestamp.year,
         importer->fbx->timestamp.month,
         importer->fbx->timestamp.day,
         importer->fbx->timestamp.hour,
         importer->fbx->timestamp.minute,
         importer->fbx->timestamp.second,
         importer->fbx->timestamp.millisecond);

  printf("Authored with %s (%s) by %s.\n",
         importer->fbx->tool.name,
         importer->fbx->tool.version,
         importer->fbx->tool.vendor);

  printf("Exported by \"%s\".\n",
         importer->fbx->exporter.name);

  printf("Up = <%1.0f, %1.0f, %1.0f>\n"
         "Forward = <%1.0f, %1.0f, %1.0f>\n"
         "Right = <%1.0f, %1.0f, %1.0f>\n",
         importer->fbx->basis.up.x, importer->fbx->basis.up.y, importer->fbx->basis.up.z,
         importer->fbx->basis.forward.x, importer->fbx->basis.forward.y, importer->fbx->basis.forward.z,
         importer->fbx->basis.right.x, importer->fbx->basis.right.y, importer->fbx->basis.right.z);

  const fbx_scene_t *scene = &importer->fbx->scene;

  fbx_uint32_t num_of_empties = 0;
  fbx_uint32_t num_of_models = 0;
  fbx_uint32_t num_of_meshes = 0;

  for (fbx_uint32_t object = 0; object < scene->num_of_objects; ++object) {
    switch (scene->objects[object]->type) {
      case FBX_EMPTY: num_of_empties += 1; break;
      case FBX_MODEL: num_of_models += 1; break;
      case FBX_MESH: num_of_meshes += 1; break;
    }
  }

  printf("Scene is composed of %u objects.\n"
         " There are %u empties.\n"
         " There are %u models.\n"
         " There are %u meshes.\n",
         importer->fbx->scene.num_of_objects,
         num_of_empties,
         num_of_models,
         num_of_meshes);

  printf("Hierarchy:\n");

  /* Pretty print scene hierarchy. */
  walk_and_print(scene->root);

  ws_config_t config;

  config.title = "Demo";
  config.width = 1280;
  config.height = 720;
  config.major = 3;
  config.minor = 3;
  config.debug = WS_TRUE;

  ws_initialize(&config);

  /* HACK(mtwilliams): OpenGL 3.2 and onwards no longer provide an implicit
     vertex array object, but requires a vertex array object for vertex
     specification, so we mimic that behavior by generating our own implict
     vertex array object. */
  ws_uint32_t vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  camera_t camera;

  camera.position = { 0.f, 0.f, 0.f };
  camera.rotation = { 0.f, 0.f, 0.f, 1.f };
  camera.field_of_view = 90.f;
  camera.near = 0.01f;
  camera.far = 1000.f;

  /* TODO(mtwilliams): Setup a basis matrix that converts from coordinate
     system of imported scene to our coordinate system. */
  ws_mat4_t basis = WS_IDENTITY_MATRIX;

  for (;;) {
    ws_frame_pull(&frame);

    if (frame.keyboard.escape)
      frame.quit = WS_TRUE;

    time += ws_real32_t(frame.delta_in_us) / 1000000.f;
    dt = ws_real32_t(frame.delta_in_us) / 1000000.f;

    freelook(&camera, dt);

    ws_mat4_t world, view, projection;

    /* The imported scene is not scaled for now. We should scale from
       centimeters to meters! */
    world = WS_IDENTITY_MATRIX;

    /* Build perspective and view matrices from camera. */
    projection = camera_to_projection(&camera, frame.width, frame.height);
    view = camera_to_view(&camera);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);

    glViewport(0, 0, frame.width, frame.height);
    glScissor(0, 0, frame.width, frame.height);

    static const ws_color_t background = ws_rgb(50, 77, 92);
    static const ws_real32_t far[1] = { 1.0f };

    glClearBufferfv(GL_COLOR, 0, (const fbx_real32_t *)&background);
    glClearBufferfv(GL_DEPTH, 0, far);

    ws_debug_begin(frame.width, frame.height);

    /* Concatenate matrices to build a matrices that transform from scene space
       to screen space and vice versa. */
    screen_from_scene = ws_mat4_mul(projection, ws_mat4_mul(view, ws_mat4_mul(basis, world)));
    scene_from_screen = ws_mat4_inverse(screen_from_scene);

    render(scene);

    /* Display scene space position of camera. */
    ws_debug_textf(frame.width / 2, 4, 0xffffffff, WS_DEBUG_CENTER_ALIGN, "<%f, %f, %f>", camera.position.x, camera.position.y, camera.position.z);

    /* Display path to imported file in bottom right hand side. */
    ws_debug_text(frame.width - 4, frame.height - 12, 0xffffffff, WS_DEBUG_RIGHT_ALIGN, path);

    ws_debug_end();

    /* Synchronize with vertical refresh. */
    frame.sync = WS_TRUE;

    ws_frame_push(&frame);
  }

  /* Fall through, using `frame.quit`? */
  ws_shutdown();

  return EXIT_SUCCESS;
}
