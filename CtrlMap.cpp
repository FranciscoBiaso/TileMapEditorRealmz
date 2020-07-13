#include "CtrlMap.h"
#include "Thing.h"

ctrl::sOperation::sOperation(eOperation operation, data::Thing thing) :
    _operation(operation), _thing(thing)
{}

ctrl::sOperation& ctrl::sOperation::swap_operation()
{
    switch (_operation)
    {
    case ctrl::eOperation::ADD_THING: _operation = ctrl::eOperation::REMOVE_THING;
        break;
    case ctrl::eOperation::REMOVE_THING: _operation = ctrl::eOperation::ADD_THING;
        break;
    }
    return *this;
}

ctrl::CtrlMap::CtrlMap(){}

void ctrl::CtrlMap::add_ctrlz(sOperation operation)
{
    _operations.push(operation);
}

void ctrl::CtrlMap::add_ctrly(sOperation operation)
{
    _inverse_operations.push(operation);
}

ctrl::sOperation ctrl::CtrlMap::rem_ctrlz()
{
    sOperation op = _operations.top();
    _operations.pop();
    return op;
}

ctrl::sOperation ctrl::CtrlMap::rem_ctrly()
{
    sOperation op = _inverse_operations.top();
    _inverse_operations.pop();
    return op;
}

bool ctrl::CtrlMap::empty()
{
    return _operations.empty();
}

bool ctrl::CtrlMap::inv_empty()
{
    return _inverse_operations.empty();
}

int ctrl::CtrlMap::get_size()
{
    return _operations.size();
}

int ctrl::CtrlMap::inv_get_size()
{
    return _inverse_operations.size();
}

void ctrl::CtrlMap::inv_clean_stack()
{
    while (!_inverse_operations.empty())
        _inverse_operations.pop();
}

void ctrl::CtrlMap::add_last_operation(ctrl::eManipulator operation)
{ 
    // 2º operation -> 1º operation
    // 3º operation -> 2º operation
    _manipulator[1] = _manipulator[0];
    _manipulator[0] = operation;

    if (_manipulator[1] == ctrl::eManipulator::CTRL_Z &&
        _manipulator[0] == ctrl::eManipulator::OPERATION)
    {
        inv_clean_stack();
    }
}