file(REMOVE_RECURSE
  "libcircle-a.a"
  "libcircle-a.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/circle-a.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
