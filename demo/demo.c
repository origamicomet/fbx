#define WABISABI_IMPLEMENTATION
#include "wabisabi.h"

#define FBX_IMPLEMENTATION
#include "fbx.h"

static unsigned indentation = 0;
static const unsigned indentation_per_level = 2;

static void indent(unsigned levels) {
  for (unsigned spaces = levels * indentation_per_level; spaces; --spaces)
    fputc(' ', stdout);
}

static void print_a_property(const fbx_property_t *property) {
  switch (property->type) {
    case FBX_STRING_PROPERTY:
      indent(indentation);
      fputc(' ', stdout);

      fprintf(stdout,
              " => \"%.*s\"\n",
              property->value.as_a_ref.length,
              property->value.as_a_ref.string);

      break;
  }
}

static void walk_and_print(const fbx_node_t *nodes) {
  for (const fbx_node_t *node = nodes; node; node = node->sibling) {
    if (node->name)
      if (strcmp(node->name, "Properties70") == 0)
        continue;

    indent(indentation);
    fprintf(stdout, "Node \"%s\" #%u\n", node->name, node->id);

    for (unsigned property = 0; property < node->num_of_properties; ++property) {
      indent(indentation);
      fputc(' ', stdout);

      if (node->properties[property].flags & FBX_PROPERTY_IS_USER_DEFINED) {
        printf("User-defined Property \"%s\" (%s/%s)\n",
               node->properties[property].name,
               fbx_property_type_to_name(node->properties[property].type),
               node->properties[property].hint);
      } else {
        printf("Property \"%s\" (%s/%s)\n",
               node->properties[property].name,
               fbx_property_type_to_name(node->properties[property].type),
               node->properties[property].hint);
      }

      print_a_property(&node->properties[property]);
    }

    if (node->children) {
      indentation++;
      walk_and_print(node->children);
      indentation--;
    }
  }
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

  importer->stream = fbx_stream_open_from_path(path);

  if (!importer->stream) {
    fprintf(stderr, "Cannot open \"%s\"!\n", path);
    return EXIT_FAILURE;
  }

  fprintf(stdout, "Loading \"%s\"...\n", path);

  if (!fbx_importer_run(importer))
    fprintf(stderr, "Failed.\n");
  else
    fprintf(stdout, "Ok.\n");

  fbx_stream_close(importer->stream);

  fbx_size_t memory_used_for_import = importer->memory.permanent.offset
                                    + importer->memory.transient.offset
                                    + importer->memory.strings.offset;

  fprintf(stdout,
          "Used %u (%u/%u/%u) bytes of memory.\n",
          memory_used_for_import,
          importer->memory.permanent.offset,
          importer->memory.transient.offset,
          importer->memory.strings.offset);

  // walk_and_print(importer->nodes);

  fprintf(stdout,
          "Version is %u.\n",
          importer->fbx->version);

  fprintf(stdout,
          "Created at %4u/%02u/%02u %02u:%02u:%02u:%04u.\n",
          importer->fbx->timestamp.year,
          importer->fbx->timestamp.month,
          importer->fbx->timestamp.day,
          importer->fbx->timestamp.hour,
          importer->fbx->timestamp.minute,
          importer->fbx->timestamp.second,
          importer->fbx->timestamp.millisecond);

  fprintf(stdout,
          "Authored with %s %s (%s).\n",
          importer->fbx->tool.vendor,
          importer->fbx->tool.name,
          importer->fbx->tool.version);

  fprintf(stdout,
          "Exported by %s.\n",
          importer->fbx->exporter.name);

  fprintf(stdout,
          "Up = <%1.0f, %1.0f, %1.0f>\n"
          "Forward = <%1.0f, %1.0f, %1.0f>\n"
          "Right = <%1.0f, %1.0f, %1.0f>\n",
          importer->fbx->basis.up.x, importer->fbx->basis.up.y, importer->fbx->basis.up.z,
          importer->fbx->basis.forward.x, importer->fbx->basis.forward.y, importer->fbx->basis.forward.z,
          importer->fbx->basis.right.x, importer->fbx->basis.right.y, importer->fbx->basis.right.z);

  fbx_uint32_t num_of_empties = 0;
  fbx_uint32_t num_of_models = 0;
  fbx_uint32_t num_of_meshes = 0;

  for (fbx_uint32_t object = 0; object < importer->fbx->scene.num_of_objects; ++object) {
    switch (importer->fbx->scene.objects[object]->type) {
      case FBX_EMPTY: num_of_empties += 1; break;
      case FBX_MODEL: num_of_models += 1; break;
      case FBX_MESH: num_of_meshes += 1; break;
    }
  }

  fprintf(stdout,
          "Scene is composed of %u objects.\n"
          " There are %u empties.\n"
          " There are %u models.\n"
          " There are %u meshes.\n",
          importer->fbx->scene.num_of_objects,
          num_of_empties,
          num_of_models,
          num_of_meshes);

  for (;;);

  return EXIT_SUCCESS;
}
