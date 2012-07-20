{
      "include_dirs": [ "include" ],
      "sources": [
        "src/ParallelPortLinux.cpp",
        "src/ParallelPortWin32.cpp"
      ],

      # Compiler flags
      "conditions": [
        ['OS=="win"', {
          'cflags_cc': [
            #FIXME
          ]
        }, { # OS != "win"
          'cflags_cc': [
            '-fexceptions'
          ]
        }]
      ]
}
