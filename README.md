gif2jpg
=======

A GIF to JPEG(s) converter for NodeJS using [Magick++](http://www.imagemagick.org/Magick++/) library.

Usage
-------
    var gif2jpg = require ("gif2jpg");
    var fs = require ("fs");
    
    var buffer = fs.readFileSync ("some.gif");
    gif2jpg.convert (buffer, function (error, buffers) {
        for (var i = 0; i < buffers.length; ++i) {
            // do something with each jpeg frame.
        }
    };

Installation
-------
First downlaod and install [ImageMagick](http://www.imagemagick.org/script/install-source.php). On Mac OSX you can use [Homebrew](http://brew.sh) to install ImageMagick. If you are installing form source make sure that you have [libjpeg](http://www.ijg.org) installed before building ImageMagick on your system.

    $ npm install gif2jpg

If you get a "cannot open shared object file: No such file or directory" error on Linux make sure that you have <code>/usr/local/lib</code> in your <code>LD_LIBRARY_PATH</code>.

[License](https://github.com/jacobsologub/gif2jpg/edit/master/LICENSE)
-------
The MIT License (MIT)

Copyright (c) 2014 [Jacob Sologub](https://github.com/jacobsologub)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
