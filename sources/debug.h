#ifndef debug_h
#define debug_h
///----------------------------------------------------------------------------|
/// "debug.h"
///----------------------------------------------------------------------------:

#define BANNER(v1,v2,v3) std::wcout << L##v1 << L"\n" \
                                    << L##v2 << L"\n" \
                                    << L##v3 << L"\n";
///---------------|
/// Конфиг релиза.|
///---------------:
#define     MYDEBUG

#ifdef      MYDEBUG
    #define ll(v)  std::cout << #v << " = " << v << "\n"; std::cin.get();
    #define  l(v)  std::cout << #v << " = " << v << "\n";

    #define DEBUG(v)    {          \
        std::string s(__FILE__);    \
        auto p = s.rfind('\\')+1;    \
        std::cout << #v               \
                  << " : MYASSERT! \"" \
                  << s.substr(p, s.size() - p) << "\" line: "\
                  << __LINE__ << '\n'; }
    #define MYASSERT(v) if(!(v)) DEBUG(v)
    #define IF_ERROR(v, d) if(v) DEBUG(d)
#else
    #define ll(v)
    #define  l(v)
    #define MYASSERT(v)
    #define IF_ERROR(v, d)
#endif

#endif // debug_h
