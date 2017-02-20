
#ifndef running_average_filter_h
#define running_average_filter_h

#include <string.h>

namespace mono { namespace io {

    template <uint16_t Length, typename Type = uint16_t>
    class RunningAverageFilter
    {
    protected:
        Type filterData[Length];
        Type index;
        Type filterSum;

    public:

        RunningAverageFilter(Type initialValue = 0)
        {
            memset(filterData, initialValue, Length*sizeof(Type));
            filterSum = 0;
            index = 0;
        }

        uint16_t append(Type val)
        {
            filterSum -= filterData[index];
            filterData[index] =  val;
            filterSum += filterData[index];

            if (++index >= Length)
            {
                index = 0;
            }

            return filterSum / Length;
        }

        void clear(Type initialValue = 0)
        {
            filterSum = 0;
            for (int i=0; i<Length; i++) {
                filterData[i] = initialValue;
                filterSum += initialValue;
            }
        }

        Type value() const
        {
            return filterSum / Length;
        }

        Type sum() const
        {
            return filterSum;
        }

        Type variance() const
        {
            int sum = 0;
            int mean = value();
            for (int i =0; i<Length; i++)
            {
                sum += (filterData[i] - mean)*(filterData[i] - mean);
            }

            return sum / Length;
        }

        Type operator[](uint16_t indx)
        {
            return filterData[indx];
        }
    };
} }

#endif /* running_average_filter_h */
