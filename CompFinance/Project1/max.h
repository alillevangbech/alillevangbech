// Unchanged
#ifndef max_h
#define max_h

template <typename T> T  max(T a, T b)
{
            return a > b ? a: b;
}

template <typename T> T max(T a, T b , T c) // one can overload a template.
{
   return max(max(a,b),c);
}


#endif
