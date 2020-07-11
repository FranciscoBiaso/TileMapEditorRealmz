#include "CtrlMap.h"

ctrl::sOperation::sOperation(eOperation operation, std::string thing_name, math::Vec3<int> thing_position) :
    _operation(operation), _thing_position(thing_position), _thing_name(thing_name)
{}

void ctrl::CtrlMap::push_operation(sOperation operation)
{
    _operations.push(operation);
}

ctrl::sOperation ctrl::CtrlMap::pop_operation()
{
    sOperation op = _operations.top();
    _operations.pop();
    return op;
}

bool ctrl::CtrlMap::empty()
{
    return _operations.empty();
}
