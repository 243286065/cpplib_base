#ifndef BASE_SRC_BASE_MARCOS_H
#define BASE_SRC_BASE_MARCOS_H

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#endif  // BASE_SRC_BASE_MARCOS_H