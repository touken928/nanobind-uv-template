# nbuv_apply_warnings(<target>): attach a conservative, per-compiler warning set
# to the given target. Safe to call on libraries or executables.

if(COMMAND nbuv_apply_warnings)
  return()
endif()

function(nbuv_apply_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Wpedantic -Wshadow)
  endif()
endfunction()
