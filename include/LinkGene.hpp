#ifndef LinkGene_HPP
#define LinkGene_HPP
#include "NodeGene.hpp"

class NodeGene;

class LinkGene
{
public:
    LinkGene(NodeGene *toNode, NodeGene *fromNode, double weight);
    LinkGene(NodeGene *toNode, NodeGene *fromNode, double weight, int id);
    LinkGene(LinkGene &other);
    void setEnabled(bool enabled);
    double getWeight() const;
    void setWeight(double weight);
    bool isEnabled() const;
    int getID() const;
    NodeGene *getToNode() const;
    NodeGene *getFromNode() const;

    static void setNextID(int id);

    inline static int nextID = 0;
private:
    int LinkeID;
    NodeGene *toNode;
    NodeGene *fromNode;
    double weight;
    bool enabled;
};

#endif