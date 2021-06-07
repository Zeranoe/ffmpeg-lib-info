# ffmpeg-lib-info

Prints the license, version, and configuration for FFmpeg libraries.

## Building

```
cmake <src path>
cmake --build .
```

## Running

```
ffmpeg-lib-info <dll path>
```

## Example

```
>ffmpeg-lib-info "%ProgramFiles%\Mozilla Firefox\mozavcodec.dll"
avcodec_license:        LGPL version 2.1 or later
avcodec_version:        3815012
avcodec_configuration:  --disable-everything --disable-protocols --disable-demuxers --disable-muxers --disable-filters --disable-programs --disable-doc --disable-parsers --enable-parser=vp8 --enable-parser=vp9 --enable-decoder=vp8 --enable-decoder=vp9 --disable-static --enable-shared --disable-debug --disable-sdl2 --disable-libxcb --disable-securetransport --disable-iconv --disable-swresample --disable-swscale --disable-avdevice --disable-avfilter --disable-avformat --disable-d3d11va --disable-dxva2 --disable-vaapi --disable-vdpau --disable-videotoolbox --enable-decoder=flac --enable-asm --enable-x86asm --disable-cuda --disable-cuvid --enable-decoder=mp3 --toolchain=msvc
```

## License

MIT, see LICENSE for details.
