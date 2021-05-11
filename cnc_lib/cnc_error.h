/**
  * @file cnc_error.h defines error handling
  */
#ifndef CNC_ERROR_H
#define CNC_ERROR_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <string>

/**
 * @brief The Cnc_error class exception handling class
 */
class Cnc_error{
public:
    inline Cnc_error(std::string _msg) : msg(_msg){
    }

    inline std::string what() const {
        return (msg);
    }
private:
    std::string msg;
};

#endif // CNC_ERROR_H
