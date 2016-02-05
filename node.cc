#include "node.h"
extern map<string, Node*> symbol_table;

Node::Node(double value){
    this->type = CONSTANT;
    this->value = value;
}

Node::Node(string name, double value) {
    this->type  = VARIABLES;
    this->value = value;
    this->name  = name;
    symbol_table.insert(pair<string, Node*>(name, this));
}

Node::Node(opType op) {
    this->operation = op;
    this->type = OPERATION;
}

Node::~Node(){
}

double Node::ex() {
    switch(this->type) {
        case CONSTANT:
            return this->value;
            break;
        case VARIABLES:
            return this->value;
            break;
        case OPERATION:
            return this->exop();
            break;
    }
    return 0;
}

double Node::exop() {
    double r = 0;
    vector<Node*>::iterator it;
    switch(this->operation) {
        case opADD:
            LOG("execute [%f + %f]\n", nodes[0]->ex(), nodes[1]->ex());
            return nodes[0]->ex() + nodes[1]->ex();
            break;
        case opMINUS:
            LOG("execute [%f - %f]\n", nodes[0]->ex(), nodes[1]->ex());
            return nodes[0]->ex() - nodes[1]->ex();
            break;
        case opMUL:
            LOG("execute [%f * %f]\n", nodes[0]->ex(), nodes[1]->ex());
            return nodes[0]->ex() * nodes[1]->ex();
            break;
        case opDIV:
            LOG("execute [%f / %f]\n", nodes[0]->ex(), nodes[1]->ex());
            return nodes[0]->ex() / nodes[1]->ex();
            break;
        case opIF:
            LOG("executing if\n");
            if (nodes[0]->ex()) {
                return this->nodes[1]->ex();
            }
            break;
        case opWHILE:
            r = nodes[0]->ex();
            LOG("while iterate result = [%f]\n", r);
            while (r > 0) {
                r = nodes[0]->ex();
                nodes[1]->ex();
            }
            break;
        case opASSIGN:
            LOG("execute =\n");
            if (nodes[1]->type == OPERATION) {
                nodes[0]->value = nodes[1]->ex();
                return nodes[0]->ex();
            }else {
                nodes[0]->value = nodes[1]->value;
                return nodes[0]->ex();
            }
            break;
        case opPRINT:
            printf("\x1B[32m>>");
            for (size_t i = 0; i < this->nodes.size(); ++i) {
                printf("%f ", this->nodes[i]->ex());
            }
            printf("\033[0m\n");
            break;
        case opLIST:
            for (it = this->nodes.begin(); it != this->nodes.end(); ++it) {
                (*it)->ex();
            }
            break;
        default:
            break;
    }
    return 0;
}

void Node::push_parameter(Node* n) {
    this->nodes.push_back(n);
}
