{
  "targets": [
    {
    	"target_name": "gif2jpg",

    	"defines": [
    		"MAGICKCORE_QUANTUM_DEPTH=16",
    		"MAGICKCORE_HDRI_ENABLE=0"
    	],

      	"sources": [ 
      		"src/gif2jpg.cpp"
      	],

      	"conditions": [
      		['OS=="mac"', {
      			"xcode_settings": {
      				# The following settings fix the "dyld: lazy symbol binding failed: Symbol not found" "Expected in: dynamic lookup" error.
      				# https://github.com/peterbraden/node-opencv/issues/87

      				'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
			        'OTHER_CFLAGS': [ '-g', '-mmacosx-version-min=10.7', '-std=c++11', '-stdlib=libc++', '-O3', '-D__STDC_CONSTANT_MACROS', '-D_FILE_OFFSET_BITS=64', '-D_LARGEFILE_SOURCE', '-Wall' ],
			        'OTHER_CPLUSPLUSFLAGS': [ '-g', '-mmacosx-version-min=10.7', '-std=c++11', '-stdlib=libc++', '-O3', '-D__STDC_CONSTANT_MACROS', '-D_FILE_OFFSET_BITS=64', '-D_LARGEFILE_SOURCE', '-Wall' ]
      			},
      			"include_dirs": [
    				"/usr/local/include/ImageMagick-6"
    			],
      			"libraries": [
      				"-lMagick++-6.Q16",
					"-lMagickCore-6.Q16",
					"-lMagickWand-6.Q16"
      			]
      		}],
      		['OS=="linux"', {
      			"include_dirs": [
    				"/usr/local/include/ImageMagick-6"
    			],
    			"libraries": [
      				"-lMagick++-6.Q16",
					"-lMagickCore-6.Q16",
					"-lMagickWand-6.Q16"
      			]
      		}]
      	]
    }
  ]
}