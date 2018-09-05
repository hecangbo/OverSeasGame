rmdir /s/q assets
xcopy /s /y ..\..\..\Resources\csb\* .\assets\csb\
xcopy /s /y ..\..\..\Resources\res\* .\assets\res\
xcopy /s /y ..\..\..\Resources\Animation\* .\assets\Animation\
cocos luacompile -s ..\..\..\Resources\Script -d .\assets\Script -e -k 2dxLua -b XXTEA
::关闭字节码编译 --disable-compile
::cocos luacompile -s ..\..\..\Resources\Script -d .\assets\Script -e -k 2dxLua -b XXTEA --disable-compile