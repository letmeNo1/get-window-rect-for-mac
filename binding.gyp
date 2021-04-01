{
  "targets": [
    {
      "target_name": "main",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ "main.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'link_settings': {
          'libraries': [
            '-framework carbon',
            ],
          },
        },
  ]
}