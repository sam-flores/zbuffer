
struct camera
{
    double eye[3];
    double up[3];
    double coi[3];
    double x_z_angle;
    double v[4][4];
    double vi[4][4];
    double zbuf[1000][1000];
    double H;
    double a;
    double b;
    double c;
};

struct camera camera;

struct screen
{
    int width;
    int height;
    double i_time;
};

struct screen screen;

struct scene
{
    struct object objects[100];
    int num_objects;
};

struct scene scene;

void zbuf_render(double xyz[3], double N[3], struct object obj)
{
    double argb[3];
    if (xyz[2] > 0 &&
        xyz[1] / xyz[2] < camera.H &&
        xyz[0] / xyz[2] < camera.H &&
        xyz[1] / xyz[2] > -camera.H &&
        xyz[0] / xyz[2] > -camera.H)
    {
        double xp = camera.a * (xyz[0] / xyz[2]) + camera.b; // screen transformation
        double yp = camera.a * (xyz[1] / xyz[2]) + camera.c;
        // check if the point is allowed into zbuffer
        if (xp < screen.width &&
            yp < screen.height &&
            xp >= 0 && yp >= 0 &&
            xyz[2] < camera.zbuf[(int)(xp)][(int)(yp)])
        {
            camera.zbuf[(int)(xp)][(int)(yp)] = xyz[2];
            Light_Model(obj.rgb, camera.eye, xyz, N, argb);
            G_rgb(argb[0], argb[1], argb[2]); // set color to pink
            G_point(xp, yp);
        }
    }
}

void position_camera(double v[3])
{
    camera.coi[0] = camera.eye[0] + v[0];
    camera.coi[1] = camera.eye[1] + v[1];
    camera.coi[2] = camera.eye[2] + v[2];

    camera.up[0] = camera.eye[0] + 0;
    camera.up[1] = camera.eye[1] + 1;
    camera.up[2] = camera.eye[2] + 0;

    M3d_view(camera.v, camera.vi, camera.eye, camera.coi, camera.up); // create view matrix
}

void set_camera_constants(int width, int height, double Half_angle_degrees, double eye[3])
{
    double Half_window_size = 0.5 * width; // 3D graphics set up
    Half_angle_degrees = 30;
    double Tan_half_angle = tan(Half_angle_degrees * M_PI / 180);
    camera.H = tan(30 * M_PI / 180);
    camera.a = 300 / camera.H;
    camera.b = 0.5 * width;
    camera.c = 0.5 * height;
    camera.eye[0] = eye[0];
    camera.eye[1] = eye[1];
    camera.eye[2] = eye[2]; // start eye out at origin

}

void set_rgb(struct object *obj, double rgb[3])
{
    for (int i = 0; i < 3; i++)
        obj->rgb[i] = rgb[i];
}

void set_world_rst(struct object *obj, double rot[3], double scl[3], double pos[3])
{

    for (int i = 0; i < 3; i++)
    {
        obj->rot[i] = rot[i];
        obj->scl[i] = scl[i];
        obj->pos[i] = pos[i];
    }
}

void set_obj_mat(struct object *obj)
{
    int Tn, Ttypelist[100];
    double Tvlist[100];
    Tn = 0;
    Ttypelist[Tn] = RX;
    Tvlist[Tn] = obj->rot[0];
    Tn++;
    Ttypelist[Tn] = RY;
    Tvlist[Tn] = obj->rot[1];
    Tn++;
    Ttypelist[Tn] = RZ;
    Tvlist[Tn] = obj->rot[2];
    Tn++;
    Ttypelist[Tn] = SX;
    Tvlist[Tn] = obj->scl[0];
    Tn++;
    Ttypelist[Tn] = SY;
    Tvlist[Tn] = obj->scl[1];
    Tn++;
    Ttypelist[Tn] = SZ;
    Tvlist[Tn] = obj->scl[2];
    Tn++;
    Ttypelist[Tn] = TX;
    Tvlist[Tn] = obj->pos[0];
    Tn++;
    Ttypelist[Tn] = TY;
    Tvlist[Tn] = obj->pos[1];
    Tn++;
    Ttypelist[Tn] = TZ;
    Tvlist[Tn] = obj->pos[2];
    Tn++;

    M3d_make_movement_sequence_matrix(obj->RST, obj->RST_inverse, Tn,
                                      Ttypelist, Tvlist); // move object
}

struct object *create_object(double rot[3], double scl[3], double pos[3], double rgb[3])
{
    struct object *obj = malloc(sizeof(struct object));
    set_world_rst(obj, rot, scl, pos);
    set_obj_mat(obj);
    set_rgb(obj, rgb);
    return obj;
}

void init_screen(int width, int height)
{
    screen.width = width;
    screen.height = height;
}

void refresh_zbuf()
{
    int i = 0;
    int j = 0;
    while (i < screen.width)
    {
        j = 0;
        while (j < screen.height)
        {
            camera.zbuf[i][j] = 1e20;
            j++;
        }
        i++;
    }
}

void refresh_screen()
{
    G_rgb(0, 0, 0);
    G_clear();
    refresh_zbuf();
}
