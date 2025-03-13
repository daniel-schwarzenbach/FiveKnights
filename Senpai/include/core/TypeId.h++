#pragma once
#include "./Base.h++"

namespace Senpai
{
   constexpr u32 get_unique_id(){
      static u32 id = 1u;
      return id++ ;
   }

   template<typename T>
   constexpr u32 type_id() {
      static const u32 id = get_unique_id();
      return id;
   }
} // namespace Senpai
