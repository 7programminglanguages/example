﻿
///
/// Copyright 2014 Neowiz Games, All rights reserved
///
/// author pudae(pudae@neowiz.com)
///

#pragma once

#include <functional>
#include <type_traits>

#include "expected.hpp"

namespace railway {

/// 우선 Expected<std::string> (Expected<std::string>) 형태에 한해서만 만들어 봄 
class Rail
{
public:
  explicit Rail(std::function<Expected<std::string>(std::string)> f)
      : f_(f)
  { }

  template <typename F,
            typename std::enable_if<
                std::is_same<typename std::result_of<F(Expected<std::string>)>::type, 
                             Expected<std::string>>::value>::type* = nullptr>
  Rail& operator>>(F&& f)
  {
    f_ = std::bind(f, std::bind(f_, std::placeholders::_1));
    return *this;
  } 

  template <typename F,
            typename std::enable_if<
                std::is_same<typename std::result_of<F(std::string)>::type, 
                             Expected<std::string>>::value>::type* = nullptr>
  Rail& operator>>(F&& f)
  {
    f_ = std::bind(railway::bind(f), std::bind(f_, std::placeholders::_1));
    return *this;
  } 

  template <typename F,
            typename std::enable_if<
                std::is_same<typename std::result_of<F(std::string)>::type, 
                             std::string>::value>::type* = nullptr>
  Rail& operator>>(F&& f)
  {
    f_ = std::bind(railway::try_catch(f), std::bind(f_, std::placeholders::_1));
    return *this;
  }

  template <typename F,
            typename std::enable_if<
                std::is_void<typename std::result_of<F(std::string)>::type>::value>::type* = nullptr>
  Rail& operator>>(F&& f)
  {
    f_ = std::bind(railway::try_catch(railway::tee(f)), 
                   std::bind(f_, std::placeholders::_1));
    return *this;
  }

  Expected<std::string> operator()(std::string s) { return f_(s); }

private:
  std::function<Expected<std::string>(std::string)> f_;
};

}  // end namespace railway
