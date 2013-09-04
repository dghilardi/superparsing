#ifndef MRFNODE_H
#define MRFNODE_H

class MRFNode{
public:
    virtual int getLabel() = 0;
    virtual void setLabel(int newLabel) = 0;
};

#endif // MRFNODE_H
