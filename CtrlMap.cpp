#include "CtrlMap.h"
#include "Thing.h"

ctrl::sOperation::sOperation(eOperation operation, data::Thing thing) :
    _operation(operation), _thing(thing)
{}

void ctrl::sOperation::swap_operation()
{
    switch (_operation)
    {
    case ctrl::eOperation::ADD_THING: _operation = ctrl::eOperation::REMOVE_THING;
        break;
    case ctrl::eOperation::REMOVE_THING: _operation = ctrl::eOperation::ADD_THING;
        break;
    }
}

ctrl::CtrlMap::CtrlMap()
{
    _manipulator_cursor = 0;
}

void ctrl::CtrlMap::push_operation(sOperation operation)
{
    _operations.push(operation);
}

void ctrl::CtrlMap::push_inv_operation(sOperation operation)
{
    _inverse_operations.push(operation);
}

ctrl::sOperation ctrl::CtrlMap::pop_operation()
{
    sOperation op = _operations.top();
    _operations.pop();
    return op;
}

ctrl::sOperation ctrl::CtrlMap::pop_inv_operation()
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

void ctrl::CtrlMap::update_manipulator(ctrl::eManipulator operation)
{ 
    // 2º operation -> 1º operation
    // 3º operation -> 2º operation
    _manipulator[1] = _manipulator[0];
    _manipulator[0] = operation;
    _manipulator_cursor = (_manipulator_cursor+1) % 2;

    if (_manipulator[1] == ctrl::eManipulator::CTRL_Z &&
        _manipulator[0] == ctrl::eManipulator::OPERATION)
    {
        inv_clean_stack();
    }
}