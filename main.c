#include "FPToolkit.c"
#include "M3d_matrix_tools.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "light_model.c"
#include "object.c"
#include "graphics.c"
#include "tree.c"
#include "sphere.c"
#include "cylinder.c"
#include "plane.c"
#include "torus.c"
#include "inputs.c"

void create_scene()
{
    double pos[3] = {0, -.25, 0};
    double rot[3] = {0, 0, 0};
    double scl[3] = {.5, .5, .5};
    double rgb[3] = {0, 1, 0};

    // tree
    int num_branches = 50;
    scene.objects[scene.num_objects] = *create_object(rot, scl, pos, rgb);
    scene.objects[scene.num_objects].render = render_tree;
    scene.objects[scene.num_objects].head = init_tree(num_branches);
    scene.num_objects++;

    // sphere
    pos[1] = -.5;
    scl[0] = .2;
    scl[1] = .2;
    scl[2] = .2;
    rgb[0] = 0;
    rgb[1] = 1;
    rgb[2] = 1;
    scene.objects[scene.num_objects] = *create_object(rot, scl, pos, rgb);
    scene.objects[scene.num_objects].render = render_sphere;
    scene.num_objects++;

    // cylinder
    pos[1] = 0;
    pos[0] = .5;
    scl[0] = .2;
    scl[1] = .2;
    scl[2] = .2;
    rgb[0] = 1;
    rgb[1] = .4;
    rgb[2] = 1;
    scene.objects[scene.num_objects] = *create_object(rot, scl, pos, rgb);
    scene.objects[scene.num_objects].render = render_cylinder;
    scene.num_objects++;

    pos[0] = 0;
    pos[1] = -.5;
    scl[0] = 1;
    scl[1] = 1;
    scl[2] = 1;
    rgb[0] = .1;
    rgb[1] = .3;
    rgb[2] = 1;
    scene.objects[scene.num_objects] = *create_object(rot, scl, pos, rgb);
    scene.objects[scene.num_objects].render = render_plane;
    scene.num_objects++;

    pos[0] = -.5;
    pos[1] = 0;
    rot[0] = 45;
    scl[0] = .2;
    scl[1] = .2;
    scl[2] = .2;
    rgb[0] = .1;
    rgb[1] = 1;
    rgb[2] = 0;
    scene.objects[scene.num_objects] = *create_object(rot, scl, pos, rgb);
    scene.objects[scene.num_objects].r_inner = 1;
    scene.objects[scene.num_objects].r_outer = .25;
    scene.objects[scene.num_objects].render = render_torus;
    scene.num_objects++;
}

void update_scene()
{
    scene.objects[0].rot[1] = screen.i_time;
    set_obj_mat(&scene.objects[0]);
}

void render_scene()
{

    for (int i = 0; i < scene.num_objects; i++)
    {
        scene.objects[i].render(scene.objects[i]);
    }
}

int main()
{

    init_screen(600, 600);
    G_init_graphics(screen.width, screen.height); // init graphics

    double camera_lens_angle = 30;
    double eye[3] = {0, 0, -2};
    set_camera_constants(screen.width, screen.height, camera_lens_angle, eye);
    double v[3] = {0, 0, 1};
    position_camera(v);

    create_scene();

    int q = 0;
    while (q != 'q')
    {

        refresh_screen();

        render_scene();
        G_display_image();

        q = G_no_wait_key();
        get_key(q, v);
        position_camera(v);
        update_scene();

        screen.i_time += M_PI / 4;
        usleep(1000);
    }
}