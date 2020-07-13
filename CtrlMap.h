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
    /**
    * ENUM MANIPULATOR, used to detect 2 sequence of operations.
    */
    enum eManipulator{
        NONE= 0,  /**< enum  no operations etc*/
        CTRL_Z = 1,
        OPERATION,
        CTRL_Y
    };
    /**
    * The concept of operation.
    */
    typedef struct sOperation
    {
        /**
         *  @brief constructor.
         */
        sOperation(eOperation, data::Thing);
        eOperation _operation; /**< enum  REMOVE_THING, ADD_THING, etc*/
        data::Thing _thing;  /**< enum thing object grab when addThing into the map */
        /**
         *  @brief Swaps the current operation with your reflection.
         */
        sOperation& swap_operation();
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
        int _manipulator[2]; /**< following the last 2 operations */ 
    public:
        /**
         *  constructor.
         */
        CtrlMap();
        /**
         *  @brief Add the last operation into the stack (CTRL-Z).
         */
        void add_ctrlz(sOperation operation);
        /**
         *  @brief Add the last operation into the stack (CTRL-Y).
         *  Push operations occur when ctrl+z is pressed.
         */
        void add_ctrly(sOperation operation);
        /**
         *  @brief Remove the last operation from stack when user press CTRL+Z.
         */
        sOperation rem_ctrlz();
        /**
         *  @brief Remove the last operation from stack when user press CTRL+Y.
         */
        sOperation rem_ctrly();
        /**
         *  @brief Checks if stack (CTRL+Z) is empty.
         */
        bool empty();
        /**
         *  @brief Checks if inv stack (CTRL+Y) is empty.
         */
        bool inv_empty();
        /**
         *  @brief return stack (CTRL+Z) size.
         */
        int get_size();
        /**
         *  @brief return stack (CTRL+Y) size.
         */
        int inv_get_size();
        /**
         *  @brief cleans the stack.
         */
        void inv_clean_stack();
        /**
         *  @brief update the last operation. This function is used to grab the last 2 sequence of operations.
         */
        void add_last_operation(ctrl::eManipulator);
    };

};
