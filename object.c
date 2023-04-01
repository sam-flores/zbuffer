
struct object
{
    double rot[3];
    double scl[3];
    double pos[3];
    double rgb[3];
    double r_inner;
    double r_outer;
    struct treenode *head;
    double RST[4][4];
    double RST_inverse[4][4];
    void (*render)(struct object obj);
};