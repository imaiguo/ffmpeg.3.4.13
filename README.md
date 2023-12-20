

# Windows Build

下载安装msys2, 添加目录到PATH环境变量
参考文档 ttps://trac.ffmpeg.org/wiki/CompilationGuide/MSVC

修改msys2_shell.cmd文件第17行去掉rem如下, 可以让msys2继承cmd的环境变量
```bash
> set MSYS2_PATH_TYPE=inherit
```

## 安装依赖
```bash
> cmd
> vcvars64
> msys2_shell
> pacman -S pkg-config
> pacman -S diffutils
> pacman -S make
> pacman -S yasm
> pacman -S meson 
```

## 配置并构建
```bash
> cmd
> vcvars64
> msys2_shell
> export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/d/devtools/SDL.2.0.16/lib/pkgconfig:/d/devtools/openh264.2.4.0/lib/pkgconfig:/d/devtools/x265.3.4/lib/pkgconfig/
> pkg-config sdl2 --libs
> pkg-config openh264 --libs
> pkg-config x265 --libs
> mkdir build & cd build
> ../configure --target-os=win64 --arch=x86_64 --enable-shared --prefix=/d/devtools/ffmpeg.3.4.13  --extra-cflags="-MD" --toolchain=msvc  --disable-debug --enable-sdl --enable-libopenh264 --enable-libx265

> 修改生成的配置文件config.h从gbk转换为utf8编码带bom格式
> make -j 24
> make install
```

## debug构建
```bash
> ../configure --target-os=win64 --arch=x86_64  --enable-shared --prefix=/d/devtools/ffmpeg.3.4.13  --extra-cflags="-MD" --toolchain=msvc --disable-sdl2 --enable-debug
>
```

## 工具使用

生成rtsp流[视频文件/摄像头]
```bash
> ffmpeg.exe -re -i E:/视频/娱乐/动画片/小黄人大眼萌.mkv -c copy -f rtsp rtsp://127.0.0.1:8554/stream
> ffplay -f dshow -i video=" name " -s 1280x720
```

查看视频音频设备
```bash
> ffmpeg -list_devices true -f dshow -i dummy
``````

播放视频音频设备
```bash
> ffplay -f dshow -i video="Logi C270 HD WebCam"
> ffmpeg -f dshow -i video="Logi C270 HD WebCam" -vcodec h264 camera.mp4
> ffmpeg -f dshow -i video="Logi C270 HD WebCam" -vcodec mpeg4 camera.mp4
```

查看支持解码格式
```bash
> ffmpeg.exe -formats | grep h264
> ffmpeg.exe -formats | grep hevc
```

查看支持的编码格式
默认支持Encoder nvenc [NVIDIA NVENC H.264 encoder]
```bash
> ffmpeg.exe -h encoder=h264
> ffmpeg.exe -h encoder=hevc
> ffmpeg.exe -encoders | grep 264
> ffmpeg.exe -encoders | grep hevc
```
