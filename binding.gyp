{
    'targets': [
        {
            'target_name': 'native-process-working-directory',
            'cflags!': ['-fno-exceptions'],
            'cflags_cc!': ['-fno-exceptions'],
            'conditions': [
                ['OS=="mac"', {
                    'include_dirs': [
                        "<!@(node -p \"require('node-addon-api').include\")",
                    ],
                    'xcode_settings': {
                        'OTHER_CFLAGS': [
                            '-DNAPI_DISABLE_CPP_EXCEPTIONS',
                            '-Wall',
                        ],
                    },
                    'sources': [
                        'src/darwin.cc'
                    ],
                }],
                ['OS=="win"', {
                    'defines': ['UNICODE'],
                    'include_dirs': [
                        "<!@(node -p \"require('node-addon-api').include\")",
                    ],
                    'msvs_settings': {
                        'VCCLCompilerTool': {
                            'ExceptionHandling': 1,
                        },
                    },
                    'sources': [
                        'src\\win32.cc',
                    ]
                }],
            ],
        },
    ]
}
