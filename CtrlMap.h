#pragma once
#include <stack>
#include <string>
#include "Vec3.h"

namespace ctrl {

    enum eOperation {
        REMOVE_THING,
        ADD_THING
    };

    typedef struct sOperation
    {
        sOperation(eOperation, std::string, math::Vec3<int>);

        eOperation _operation;
        std::string _thing_name;
        math::Vec3<int> _thing_position;        
    }sOperation;

    class CtrlMap
    {
    private:
        std::stack<sOperation> _operations;
    public:
        void push_operation(sOperation operation);
        sOperation pop_operation();
        bool empty();
    };

};
