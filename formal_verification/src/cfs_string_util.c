#include <stddef.h>
#include <stdint.h>

#define CFE_SB_BAD_ARGUMENT (-1)

/* 
 * This is NASA cFS's string copy utility function, isolated for formal verification.
 * We are using ACSL (ANSI/ISO C Specification Language) to mathematically prove its safety.
 */

/*@
  requires DestMaxSize > 0 ==> \valid(DestStringPtr + (0 .. DestMaxSize - 1));
  requires SourceMaxSize > 0 && SourceStringPtr != \null ==> \valid_read(SourceStringPtr + (0 .. SourceMaxSize - 1));
  requires DefaultString != \null ==> \valid_read(DefaultString + (0 .. DestMaxSize - 1));
  
  assigns DestStringPtr[0 .. DestMaxSize - 1];
  
  behavior invalid_args:
    assumes DestMaxSize == 0 || DestStringPtr == \null;
    assigns \nothing;
    ensures \result == CFE_SB_BAD_ARGUMENT;

  behavior valid_copy:
    assumes DestMaxSize > 0 && DestStringPtr != \null;
    ensures \result >= 0 && \result < DestMaxSize;
    ensures DestStringPtr[\result] == 0;

  complete behaviors;
  disjoint behaviors;
*/
int32_t CFE_SB_MessageStringGet(char       *DestStringPtr,
                              const char *SourceStringPtr,
                              const char *DefaultString,
                              size_t      DestMaxSize,
                              size_t      SourceMaxSize)
{
    int32_t Result;

    /*
     * Error in caller if DestMaxSize == 0.
     * Cannot terminate the string, since there is no place for the NUL
     * In this case, do nothing
     */
    if (DestMaxSize == 0 || DestStringPtr == NULL)
    {
        Result = CFE_SB_BAD_ARGUMENT;
    }
    else
    {
        Result = 0;

        /*
         * Check if should use the default, which is if
         * the source string has zero length (first char is NUL).
         */
        if (DefaultString != NULL && (SourceMaxSize == 0 || *SourceStringPtr == 0))
        {
            SourceStringPtr = DefaultString;
            SourceMaxSize   = DestMaxSize;
        }

        /* Reserve 1 character for the required NUL */
        --DestMaxSize;

        /*@
          loop invariant Result >= 0;
          loop invariant SourceMaxSize >= 0;
          loop invariant DestMaxSize >= 0;
          loop invariant Result + DestMaxSize + 1 == \at(DestMaxSize, Pre);
          loop assigns DestStringPtr[0 .. Result], Result, SourceMaxSize, DestMaxSize, SourceStringPtr, DestStringPtr;
          loop variant DestMaxSize;
        */
        while (SourceMaxSize > 0 && *SourceStringPtr != 0 && DestMaxSize > 0)
        {
            *DestStringPtr = *SourceStringPtr;
            ++DestStringPtr;
            ++SourceStringPtr;
            --SourceMaxSize;
            --DestMaxSize;

            ++Result;
        }

        /* Put the NUL in the last character */
        *DestStringPtr = 0;
    }

    return Result;
}
