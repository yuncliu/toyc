#include "node.h"

map<string, double> Node::symbol_table;

Node::Node(){
}

Node::~Node(){
}

double Node::ex() {
    switch(this->type) {
        case CONSTANT:
            return this->value;
            break;
        case IDENTIFIER:
            break;
        case OPERATION:
            break;
    }
    return 0;
}
double Node::ex(Node::opType op, vector<Node>& nodes) {
    switch(this->type) {
        case CONSTANT:
            return this->value;
            break;
        case IDENTIFIER:
            break;
        case OPERATION:
            this->exop(op, nodes);
            break;
    }
    return 0;
}

double Node::exop(Node::opType op, vector<Node>& nodes) {
    switch(op) {
        case opADD:
            this->value = nodes[0].value + nodes[1].value;
            break;
        case opIF:
            if (nodes[0].value) {
                nodes[1].ex();
            }
            break;
        default:
            break;
    }
    return 0;
}
