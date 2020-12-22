#ifndef NODE_H
#define NODE_H

struct Node {
    int xi, xf;
    int yi, yf;
    int zi, zf;
    unsigned char color;
    bool leaf;
    Node* children[8] = {};

    Node(){};

	Node(int xi, int xf, int yi, int yf, int zi, int zf) :
        leaf{false}, xi{xi}, xf{xf}, yi{yi}, yf{yf}, zi{zi}, zf{zf} {}
};

#endif