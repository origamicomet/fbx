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

ws_uint32_t r_program;

ws_uint32_t r_time_uniform_loc;
ws_uint32_t r_dt_uniform_loc;

ws_uint32_t r_screen_from_world_uniform_loc;
ws_uint32_t r_world_from_screen_uniform_loc;

ws_uint32_t r_world_from_object_uniform_loc;
ws_uint32_t r_object_from_world_uniform_loc;

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

  const fbx_vec3_t p = model->transform.position;
  const fbx_quaternion_t r = model->transform.rotation;
  const fbx_vec3_t s = model->transform.scale;

  printf_with_indentation(depth*2+1, "Positioned at <%f, %f, %f> relative to parent.\n", p.x, p.y, p.z);
  printf_with_indentation(depth*2+1, "Rotated with <%f, %f, %f, %f> relative to parent.\n", r.x, r.y, r.z, r.w);
  printf_with_indentation(depth*2+1, "Scaled by <%f, %f, %f> relative to parent.\n", s.x, s.y, s.z);
}

static void print_info_about_mesh(const fbx_mesh_t *mesh)
{
  printf_with_indentation(depth*2+1, "Composed of %u triangles corresponding to %u vertices.\n", mesh->num_of_faces, mesh->num_of_vertices);
}

static const char *TYPE_TO_STRING[] = { "Unknown", "Empty", "Model", "Mesh" };

static void walk_and_print(const fbx_object_t *object)
{
  printf_with_indentation(depth*2, "%s (%llu)\n", TYPE_TO_STRING[object->type], object->id);

  if (object->name)
    printf_with_indentation(depth*2+1, "Called \"%s\".\n", object->name);

  switch (object->type) {
    case FBX_MODEL: print_info_about_model((const fbx_model_t *)object->reified); break;
    case FBX_MESH: print_info_about_mesh((const fbx_mesh_t *)object->reified); break;
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

void render_an_empty(const fbx_object_t *object, const ws_mat4_t object_to_scene);
void render_a_model(const fbx_object_t *object, const ws_mat4_t object_to_scene);
void render_a_mesh(const fbx_object_t *object, const ws_mat4_t object_to_scene);

static const struct {
  ws_bool_t name;
  ws_uint32_t color;
} r_viz[] = {
  { WS_FALSE, 0x00000000 }, /* Nothing */
  { WS_TRUE,  0xffffffff }, /* Empty */
  { WS_TRUE,  0xffffff00 }, /* Model */
  { WS_TRUE,  0xff00ff00 }, /* Mesh  */
};

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

  for (unsigned child = 0; child < object->num_of_children; ++child) {
    depth++;
    render_an_object(object->children[child], object_to_scene);
    depth--;
  }

  static void (*renderers[])(const fbx_object_t *, const ws_mat4_t) = {
    render_an_empty,
    render_an_empty,
    render_a_model,
    render_a_mesh
  };

  renderers[object->type](object, object_to_scene);

  if (r_debug) {
    if (r_viz[object->type].name && object->name) {
      const ws_vec3_t position_in_ws = ws_mat4_transform_vec3(object_to_scene, { 0.f, 0.f, 0.f });
      const ws_vec3_t position_in_ss = ws_mat4_transform_vec3(screen_from_scene, position_in_ws);

      if (position_in_ss.z < 1.f) {
        ws_int32_t x = frame.width * (position_in_ss.x * 0.5f + 0.5f);
        ws_int32_t y = frame.height * (1.f - (position_in_ss.y * 0.5f + 0.5f));

        x += 8 * (depth-1);
        y += 8 * (depth-1);

        ws_debug_text(x+1, y+1, 0xff000000, WS_DEBUG_CENTER_ALIGN, object->name);
        ws_debug_text(x, y, r_viz[object->type].color, WS_DEBUG_CENTER_ALIGN, object->name);
      }
    }
  }
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

  const ws_mat4_t translation =
    ws_translation_to_mat4(*(const ws_vec3_t *)&model->transform.position);

  const ws_mat4_t rotation =
    ws_rotation_to_mat4(*(const ws_quaternion_t *)&model->transform.rotation);

  const ws_mat4_t scale =
    ws_scale_to_mat4(*(const ws_vec3_t *)&model->transform.scale);

  const ws_mat4_t r_pivot =
    ws_translation_to_mat4(*(const ws_vec3_t *)&model->transform.center_of_rotation);

  const ws_mat4_t r_offset =
    ws_translation_to_mat4(*(const ws_vec3_t *)&model->transform.point_of_rotation);

  const ws_mat4_t s_pivot =
    ws_translation_to_mat4(*(const ws_vec3_t *)&model->transform.center_of_scaling);

  const ws_mat4_t s_offset =
    ws_translation_to_mat4(*(const ws_vec3_t *)&model->transform.point_of_scaling);

  ws_mat4_t transform = WS_IDENTITY_MATRIX;

  transform = ws_mat4_mul(transform, translation);
  transform = ws_mat4_mul(transform, r_offset);
  transform = ws_mat4_mul(transform, r_pivot);
  transform = ws_mat4_mul(transform, rotation);
  transform = ws_mat4_mul(transform, ws_mat4_inverse(r_pivot));
  transform = ws_mat4_mul(transform, s_offset);
  transform = ws_mat4_mul(transform, s_pivot);
  transform = ws_mat4_mul(transform, scale);
  transform = ws_mat4_mul(transform, ws_mat4_inverse(s_pivot));

  return transform;
}

ws_mat4_t transform_from_mesh(const fbx_object_t *object)
{
  return WS_IDENTITY_MATRIX;
}

void render_an_empty(const fbx_object_t *object,
                   const ws_mat4_t object_to_scene)
{
}

void render_a_model(const fbx_object_t *object,
                   const ws_mat4_t object_to_scene)
{
}

void render_a_mesh(const fbx_object_t *object,
                   const ws_mat4_t object_to_scene)
{
  const fbx_mesh_t *mesh = (const fbx_mesh_t *)object->reified;

  ws_uint32_t vbo, ibo;

  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glBufferData(GL_ARRAY_BUFFER, mesh->num_of_vertices * 3 * 4, (const void *)mesh->positions, GL_STREAM_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->num_of_faces * 3 * 4, (const void *)mesh->indices, GL_STREAM_DRAW);

  glEnableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, 0);

  glUseProgram(r_program);

  const ws_mat4_t world_from_object = object_to_scene,
                  object_from_world = ws_mat4_inverse(world_from_object);

  glUniformMatrix4fv(r_world_from_object_uniform_loc, 1, GL_TRUE, (const ws_real32_t *)&world_from_object);
  glUniformMatrix4fv(r_object_from_world_uniform_loc, 1, GL_TRUE, (const ws_real32_t *)&object_from_world);

  glDrawElements(GL_TRIANGLES, 3 * mesh->num_of_faces, GL_UNSIGNED_INT, 0);

  // TODO(mtwilliams): Draw vertices as well.
  // glDrawArrays(GL_POINTS, 0, mesh->num_of_vertices);

  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
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

void setup()
{
  glUseProgram(r_program);

  /* Update time and delta-time uniforms. */
  glUniform1f(r_time_uniform_loc, time);
  glUniform1f(r_dt_uniform_loc, dt);

  /* Update world-view-projection uniforms. */
  glUniformMatrix4fv(r_screen_from_world_uniform_loc, 1, GL_TRUE, (const ws_real32_t *)&screen_from_scene);
  glUniformMatrix4fv(r_world_from_screen_uniform_loc, 1, GL_TRUE, (const ws_real32_t *)&scene_from_screen);

  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glFrontFace(GL_CW);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void finish()
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void render(const fbx_scene_t *scene)
{
  /* Prepare for rendering scene. */
  setup();

  /* Walk scene to render. */
  render_an_object(scene->root, WS_IDENTITY_MATRIX);

  finish();
}

ws_uint32_t program_from_sources(const char *vertex,
                                 const char *fragment);

int main(int argc, const char *argv[]) {
#if 1
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

  /* Compile and link shaders for rendering scene. */
  r_program = program_from_sources(RIGID_VERTEX_SHADER, RIGID_FRAGMENT_SHADER);

  /* Cache uniform locations. */
  r_time_uniform_loc = glGetUniformLocation(r_program, "time");
  r_dt_uniform_loc = glGetUniformLocation(r_program, "dt");
  r_screen_from_world_uniform_loc = glGetUniformLocation(r_program, "screen_from_world");
  r_world_from_screen_uniform_loc = glGetUniformLocation(r_program, "world_from_screen");
  r_world_from_object_uniform_loc = glGetUniformLocation(r_program, "world_from_object");
  r_object_from_world_uniform_loc = glGetUniformLocation(r_program, "object_from_world");

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

ws_uint32_t program_from_sources(const char *vertex,
                                 const char *fragment)
{
  ws_uint32_t program = glCreateProgram();

  ws_uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
  ws_uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vs, 1, &vertex, NULL);
  glShaderSource(fs, 1, &fragment, NULL);

  glCompileShader(vs);
  glCompileShader(fs);

  ws_int32_t vs_compiled_successfully;
  ws_int32_t fs_compiled_successfully;

  glGetShaderiv(vs, GL_COMPILE_STATUS, &vs_compiled_successfully);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &fs_compiled_successfully);

  if (!vs_compiled_successfully) {
    ws_int32_t length;
    glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);

    char *log = (char *)ws_allocate_s(length + 1, 1);
    glGetShaderInfoLog(vs, length, NULL, log);

    fprintf(stderr, "Vertex shader compilation failed!\n");
    fputs(log, stderr);
    fprintf(stderr, "---\n");
  }

  if (!fs_compiled_successfully) {
    ws_int32_t length;
    glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);

    char *log = (char *)ws_allocate_s(length + 1, 1);
    glGetShaderInfoLog(fs, length, NULL, log);

    fprintf(stderr, "Fragment shader compilation failed!\n");
    fputs(log, stderr);
    fprintf(stderr, "---\n");
  }

  glAttachShader(program, vs);
  glAttachShader(program, fs);

  glLinkProgram(program);

  ws_int32_t program_linked_successfully;

  glGetProgramiv(program, GL_LINK_STATUS, &program_linked_successfully);

  if (!program_linked_successfully) {
    ws_int32_t length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    char *log = (char *)ws_allocate_s(length + 1, 1);
    glGetProgramInfoLog(program, length, NULL, log);

    fprintf(stderr, "Shader linking failed!\n");
    fputs(log, stderr);
    fprintf(stderr, "---\n");
  }

  glDetachShader(program, vs);
  glDetachShader(program, fs);

  glDeleteShader(vs);
  glDeleteShader(fs);

  const ws_bool_t successful = vs_compiled_successfully &&
                               fs_compiled_successfully &&
                               program_linked_successfully;

  if (!successful) {
    glDeleteProgram(program);
    return 0;
  }

  // Temporarily bind to ensure the shader is truly ready.
  glUseProgram(program);
  glUseProgram(0);

  return program;
}
