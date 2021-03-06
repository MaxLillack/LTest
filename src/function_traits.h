/*
 *     The MIT License (MIT)
 *
 *     Copyright (c) 2014 Ingo Rössner
 *                        Dan Häberlein
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 */


#ifndef FUNCTION_TRAITS_H_INCLUDED
#define FUNCTION_TRAITS_H_INCLUDED

#include <type_traits>

template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())>
{};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
{
    typedef ReturnType ret;
};

template <>
struct function_traits<void(*)()>
{
    typedef void(*ret)();
};

template <>
struct function_traits<bool(*)()>
{
    typedef bool(*ret)();
};


template<typename Functor, typename ret>
struct FunctionTypeIs : enable_if<is_same<typename function_traits<Functor>::ret, ret>::value, string>
{};


template <typename T1, typename T2>
struct function_traits_for_ExpectType : public function_traits_for_ExpectType<T1, decltype(&T2::operator())>
{};

template <typename T, typename ClassType, typename ReturnType, typename... Args>
struct function_traits_for_ExpectType<T, ReturnType(ClassType::*)(Args...) const>
{
    typedef ReturnType ret;
};

template <typename T>
struct function_traits_for_ExpectType<T, void(*)(T)>
{
    typedef void(*ret)(T);
};

template <typename T>
struct function_traits_for_ExpectType<T, bool(*)(T)>
{
    typedef bool(*ret)(T);
};


template<typename T, typename Functor, typename ret, bool b>
struct ExpectType_ : enable_if<is_same<typename function_traits_for_ExpectType<T,Functor>::ret, ret>::value, void>
{};

template<typename T, typename Functor, typename ret>
struct ExpectType_<T, Functor, ret, false> : enable_if<false, void>
{};

template<typename Functor, typename ret, typename T>
struct ExpectType : ExpectType_<T, Functor,ret,!is_same<Functor, T>::value>
{};

#endif // FUNCTION_TRAITS_H_INCLUDED
