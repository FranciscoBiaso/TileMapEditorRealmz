#pragma once
#include <stack>
#include <string>
#include "Vec3.h"

namespace ctrl {
    /**
    * ENUM OPERATIONS.
    */
    enum eOperation {
        REMOVE_THING, /**< enum remove thing operation */
        ADD_THING /**< enum add thing operation */
    };

    typedef struct sOperation
    {
        /**
         *  @brief constructor.
         */
        sOperation(eOperation, std::string, math::Vec3<int>);

        eOperation _operation; /**< enum operation */
        std::string _thing_name; /**< thing name used into the map */
        math::Vec3<int> _thing_position; /**< thing position - cylinder coords */
    }sOperation;
    
    /*!
        CtrlMap
        =======

        This class represents a controller to manipulate operations over map user interface.
    */
    class CtrlMap
    {
    private:
        std::stack<sOperation> _operations; /**< stack  */
    public:
        /**
         *  @brief Add the last operation into the stack.
         */
        void push_operation(sOperation operation);
        /**
         *  @brief Remove the last operation from stack.
         */
        sOperation pop_operation();
        /**
         *  @brief Checks if stack is empty.
         */
        bool empty();
    };

};
