//
// Created by Ryan Seavey on 8/29/2026.
//
module;

#include <stdexcept>


export module ES_debug;


export namespace ES::debug{
    /**
    * An exception type made to represent when IEE754 (inevitably) fails us.
    * This ought to be thrown when an operation fails plainly due to the current floating point type running out of precision.
    */
    struct precision_exhausted : std::logic_error {
        using std::logic_error::logic_error;
    };


    /**
     * Tee hee...
     */
    struct parker_not_implemented : std::logic_error {
        using std::logic_error::logic_error;
    };

}