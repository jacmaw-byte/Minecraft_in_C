#include <glad.h>
#include <GLFW/glfw3.h>



#include <stdio.h>
#include <shader.h>
#include <vector.h>
#include <atlas.h>
#include "camera.h"
#include "constants.h"
#include "chunk.h"
#include "chunk_manager.h"
#include "noise/value_noise.h"
#include "dda.h"
#include "chunk_mesh.h"
#include "mesh.h"
#include "player/player.h"
#include "glfw_stuff.h"
#include "globals.h"
#include "opengl_stuff.h"
#include <ft2build.h>
#include FT_FREETYPE_H

int compare_chunks(const void *a, const void *b) {

    const struct Chunk *x = *(const struct Chunk * const *)a;
    const struct Chunk *y = *(const struct Chunk * const *)b;

    float x_dist = glm_vec2_distance(x->pos, (vec2){player.cam.pos[0], player.cam.pos[1]});
    float y_dist = glm_vec2_distance(y->pos, (vec2){player.cam.pos[0], player.cam.pos[1]});

    if (x_dist < y_dist)
        return 1;
    if (x_dist > y_dist)
        return -1;

    return 0;
}

int main() {

    player.inventory.items_in_hotbar[0] = DIRT_ITEM;
    player.inventory.items_in_hotbar[1] = GRASS_ITEM;
    player.inventory.items_in_hotbar[2] = GLASS_ITEM;

    GLFWwindow* window = init_glfw();

    const unsigned int chunk_shader = make_shader_program("shaders/frag.glsl", "shaders/vert.glsl");
    const unsigned int transparent_chunk_shader = make_shader_program("shaders/transparent_frag.glsl", "shaders/vert.glsl");
    const unsigned int selector_shader = make_shader_program("shaders/selector_frag.glsl", "shaders/selector_vert.glsl");

    struct Mesh selector;

    init_mesh(&selector, selector_shader, (vec3){0.0f, 0.0f});
    make_selector_cube(&selector, 0.51f, 0.0f, 0.0f, 0.0f);
    update_mesh(&selector);

    struct ChunkManager chunk_manager;

    init_chunk_manager(&chunk_manager, 20, &player.cam.pos, chunk_shader, transparent_chunk_shader);

    make_camera(&player.cam, (vec3){0.0f, CHUNK_SIZE_Y, 0.0f}, 30.0f);

    load_atlas("textures/atlas.png");
    init_opengl_setting();

    FT_Library ft;

    if (FT_Init_FreeType(&ft))
    {
        printf("ERROR::FREETYPE: Could not init FreeType Library \n");
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "fonts/JetBrainsMonoNL-Regular.ttf", 0, &face)) {
        printf("ERROR::FREETYPE: Failed to load font \n");
        return -1;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

    mat4 view_matrix;
    glm_mat4_identity(view_matrix);

    mat4 projection_matrix;
    glm_mat4_identity(projection_matrix);
    glm_perspective(glm_rad(90.0f), aspect_ratio, 0.1f, 1000.0f, projection_matrix);

    last_time = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        delta_time = currentFrame - last_time;
        last_time = currentFrame;

        processInput(window);
        update_camera(&player.cam);

        set_view_matrix(&player.cam, view_matrix);

        generate_chunks(&chunk_manager);
        glClearColor(0.4f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render here

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);

        for (int i = 0; i < chunk_manager.chunk_map.max_nodes; ++i) {
            struct ChunkNode* node = chunk_manager.chunk_map.data[i];

            while (node != nullptr) {
                if (node->filled)
                    draw_chunk_mesh(&node->chunk.mesh, &view_matrix, &projection_matrix);
                node = node->next;
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        struct Vector transparent_chunks;
        init_vector(&transparent_chunks, sizeof(struct Chunk*));

        ivec2 camera_chunk_pos = {
            (int)floorf(player.cam.pos[0] / CHUNK_SIZE_X),
            (int)floorf(player.cam.pos[2]  / CHUNK_SIZE_X)
        };

        for (int i = 0; i < chunk_manager.chunk_map.max_nodes; ++i) {
            struct ChunkNode* node = chunk_manager.chunk_map.data[i];

            while (node != nullptr) {
                if (node->filled && node->chunk.has_transparent_mesh) {
                    append_vector_lit(&transparent_chunks, &node->chunk);
                }
                node = node->next;
            }
        }
        sort_transparent_faces_in_chunk(get_from_chunk_map(&chunk_manager.chunk_map, camera_chunk_pos));

        qsort(transparent_chunks.data, transparent_chunks.size, transparent_chunks.element_size, compare_chunks);
        for (int i = 0; i < transparent_chunks.size; ++i) {
            struct Chunk* chunk = ((struct Chunk**)transparent_chunks.data)[i];

            draw_chunk_mesh(&chunk->transparent_mesh, &view_matrix, &projection_matrix);
        }
        free_vector(&transparent_chunks);

        hit = dda(&chunk_manager, (struct Ray){
            .max_dist = 5.0f,
            .origin = {player.cam.pos[0], player.cam.pos[1], player.cam.pos[2]},
            .direction = {player.cam.direction[0], player.cam.direction[1], player.cam.direction[2]}
        });

        if (hit.hit) {
            move_mesh(&selector, (vec3){hit.pos[0] + 0.5f, hit.pos[1] + 0.5f, hit.pos[2] + 0.5f});

            draw_mesh(&selector, &view_matrix, &projection_matrix);
        }

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    free_chunk_manager(&chunk_manager);
    mesh_deconstructor(&selector);
    return 0;
}