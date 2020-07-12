#pragma once
#include <stack>
#include <string>
#include "Vec3.h"
#include "Thing.h"

namespace ctrl {
    /**
    * ENUM OPERATIONS.
    */
    enum eOperation {
        REMOVE_THING, /**< enum remove thing operation */
        ADD_THING /**< enum add thing operation */
    };

    enum eManipulator{
        NONE= 0,
        CTRL_Z = 1,
        OPERATION,
        CTRL_Y
    };

    typedef struct sOperation
    {
        /**
         *  @brief constructor.
         */
        sOperation(eOperation, data::Thing);

        eOperation _operation; /**< enum operation */
        data::Thing _thing;
        /**
         *  @brief Swaps the current operation with your reflection.
         */
        void swap_operation();

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
        std::stack<sOperation> _inverse_operations; /**< stack  */
        int _manipulator[2]; // sequence of last 2 operations //
        int _manipulator_cursor;
    public:
        CtrlMap();
        /**
         *  @brief Add the last operation into the stack.
         */
        void push_operation(sOperation operation);
        void push_inv_operation(sOperation operation);
        /**
         *  @brief Remove the last operation from stack.
         */
        sOperation pop_operation();
        sOperation pop_inv_operation();
        /**
         *  @brief Checks if stack is empty.
         */
        bool empty();
        bool inv_empty();
        int get_size();
        int inv_get_size();
        void inv_clean_stack();

        void update_manipulator(ctrl::eManipulator);
    };

};
