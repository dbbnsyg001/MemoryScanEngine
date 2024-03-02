# MemoryScanEngine
Windows and ARM64 memory scan tool 

## 项目说明
1.ARM64Scanner 安卓端内存扫描工具,通过socket和ARM64ScannerForQt连接并进行交互  
2.ARM64ScannerForQt Windows端内存界面交互,通过socket和ARM64Scanner连接并进行交互  
3.MemScanner 通用命令行内存扫描工具,适用于Windows和Linux  
 
由于Cheat Engine用Pascal编写语言受众不广,ceserver大量C风格代码,外部开发人员难以改进和自定义,所以MemoryScanEngine诞生了

## 编译准备
### ARM64Scanner
1.`NDKr21` 及以上,推荐NDK r21  
2.`Cmake 3.2.2`及以上  
3.`ninja` https://github.com/ninja-build/ninja/releases 下载后放入cmake bin目录

### ARM64ScannerForQt
1.`msvc_2015_64`及以上,推荐msvc_2015_64  
2.`QT5`及以上,推荐QT5  

### MemScanner
1.Windows端:`msvc_2015_64`及以上,推荐msvc_2015_64  
1.Linux端:`gcc4.9`及以上  

## 使用方法
1.Android端启用ARM64Scanner,并且将socket重定向到端口 例如: `adb forward tcp:52736 tcp:52736`  
2.Windows端启用ARM64ScannerForQt,端口填入重定向的端口号连接,使用界面交互即可  

## Test
MemScanner是通用命令行工具,用于测试或者简单扫描使用

## 说明 
项目以精简易懂规范化为目录  
1.为了减少Socket的实现对开发和阅读者造成困扰和bug，统一使用HP Socket进行socket开发:`https://github.com/ldcsaa/HP-Socket`  
2.Qt界面使用widget继承模式，尽量保证通用UI功能集中在通用父类，主界面只用于数据业务交互  
