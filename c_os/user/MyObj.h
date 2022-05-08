#ifndef __MyObj_INCLUDE_H_
#define __MyObj_INCLUDE_H_

class MyObj {
public:
    MyObj() : value(5) {};
    MyObj(unsigned int val) : value(val) {};
    unsigned int value;
};

#endif
