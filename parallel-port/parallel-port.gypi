{
      "include_dirs": [ "include" ],
      "sources": [
        "src/ParallelPortLinux.cpp",
        "src/ParallelPortWin32.cpp"
      ],

      # Compiler flags
      "conditions": [
        ['OS=="linux"', {
          'defines': [ '_LINUX' ]
        }],
        ['OS=="win"', {
          'defines': [ '_WIN32' ],
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
