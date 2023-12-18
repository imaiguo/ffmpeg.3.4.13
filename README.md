

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
```

## 配置并构建
```bash
> cmd
> vcvars64
> msys2_shell
> export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/d/devtools/SDL.2.28.2/lib/pkgconfig
> pkg-config sdl2 --libs
-LD:/devtools/SDL.2.28.2/lib -lSDL2
> mkdir build & cd build
> ../configure --target-os=win64 --arch=x86_64  --enable-shared --prefix=/d/devtools/ffmpeg.3.4.13  --extra-cflags="-MD" --toolchain=msvc --disable-sdl2
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

```bash
> ffmpeg.exe -re -i E:/视频/娱乐/动画片/小黄人大眼萌.mkv -c copy -f rtsp rtsp://127.0.0.1:8554/stream
```
