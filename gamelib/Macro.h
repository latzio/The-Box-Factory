#ifndef MACRO_H_
#define MACRO_H_

#define CopyDefault(T)      T(const T&) = default; \
                            T& operator=(const T&) = default;

#define CopyCtor(T)         T(const T&); \
                            T& operator=(const T&);

#define MoveCtor(T)         T(const T&) = delete; \
                            T& operator=(const T&) = delete; \
                            T(T&&); \
                            T& operator=(T&&);

#define NoMoveOrCopy(T)     T(const T&) = delete; \
                            T& operator=(const T&) = delete; \
                            T(T&&) = delete; \
                            T& operator=(T&&) = delete;

#endif
