{
  "targets": [
    {
      "target_name": "parallel-port",
      "type": "shared_library",
      "include_dirs": [ "include" ],
      "sources": [
        "src/ParallelPortLinux.cpp",
        "src/ParallelPortWin32.cpp"
      ],
      "direct_dependent_settings": {
        "include_dirs": [ "include" ],
        "conditions": [
          ['OS=="win"', {
            'cflags_cc': [
              #FIXME
            ],
          }, { # OS != "win"
            'cflags_cc': [
              '-fexceptions'
            ],
          }]
        ]
      },

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
  ]
}
