#include "NodeGene.hpp"
#include "Util.hpp"

NodeGene::NodeGene(int id, NodeType type)
{
    this->id = id;
    this->bias = randDouble(-1, 1);
    this->type = type;
}
NodeGene::NodeGene(int id, NodeType type, double bias)
{
    this->id = id;
    this->bias = bias;
    this->type = type;
}
NodeGene::NodeGene(NodeType type, double bias)
{
    this->id = nextID++;
    this->bias = bias;
    this->type = type;
}
NodeGene::NodeGene(NodeGene &other)
{
    this->id = other.id;
    this->bias = other.bias;
    this->type = other.type;
}
void NodeGene::addToLink(const LinkGene &link)
{
    toLinks.push_back(&link);
}

void NodeGene::removeLink(const LinkGene &link)
{
    for (int i = 0; i < toLinks.size(); i++)
    {
        if (toLinks[i]->getID() == link.getID())
        {
            toLinks.erase(toLinks.begin() + i);
            return;
        }
    }
}

int NodeGene::getID() const
{
    return id;
}

double NodeGene::getBias() const
{
    return bias;
}

void NodeGene::setBias(double bias)
{
    this->bias = bias;
}

NodeType NodeGene::getType() const
{
    return type;
}

vector<const LinkGene *> NodeGene::getToLinks() const
{
    return toLinks;
}

void NodeGene::addAccumalator(double value)
{
    accumalator += value;
}

double NodeGene::activate()
{
    if (type == NodeType::OUTPUT)
    {
        this->output = accumalator + bias;
        return this->output;
    }
    this->output = activationRelu.activate(accumalator + bias);
    return output;
}

void NodeGene::resetAccumalator()
{
    accumalator = 0;
}

void NodeGene::setNextID(int id)
{
    nextID = id;
}

NodePtr NodeGene::getNewNode()
{
    double bias = randDouble(-1, 1);
    return std::make_shared<NodeGene>(HIDDEN, bias);
}