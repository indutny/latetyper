{
  "variables": {
    "gypkg_deps": [
      # Place for `gypkg` dependencies
      "git://github.com/libuv/libuv@^1.7.0 => uv.gyp:libuv",
    ],
  },

  "targets": [ {
    "target_name": "latetyper",
    "type": "executable",

    "dependencies": [
      "<!@(gypkg deps <(gypkg_deps))",
      # Place for local dependencies
    ],

    "direct_dependent_settings": {
      "include_dirs": [
        # Place for public includes
        "include",
      ],
    },

    "include_dirs": [
      # Place for private includes
      ".",
    ],

    "sources": [
      # Place for source files
      "src/main.c",
    ],

    "libraries": [
      "-framework CoreFoundation",
      "-framework CoreGraphics",
    ],
  } ],
}
